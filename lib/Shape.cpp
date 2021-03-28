#include "pch.h"
#include <stdexcept>
#include <unordered_map>

#include "Shape.h"
#include "Intersect.h"

using namespace ray;

Vec3 Shape::normal_at(const Point3& world_point) const {
	Point3 local_point = world_to_object(world_point);
	Vec3 local_normal = local_normal_at(local_point);
	return normal_to_world(local_normal);
}

void Shape::intersect(const Ray& inr, IntersectionList& out) const {
	Ray local_ray = transform.inverse_multiply(inr);

	local_intersect(local_ray, out);
}

Point3 Shape::world_to_object(Point3 point) const {
	if (parent != nullptr) {
		point = parent->world_to_object(point);
	}
	return transform.inverse_multiply(point);
}

Vec3 Shape::normal_to_world(Vec3 normal) const {
	Matrix4 inv = transform.inverse();
	Matrix4 transpose = inv.transpose();
	normal = transpose * normal;
	normal.w = 0;
	normal = normal.norm();

	if (parent != nullptr) {
		normal = parent->normal_to_world(normal);
	}

	return normal;
}

void Sphere::local_intersect(const Ray& local_ray, IntersectionList& out) const {
	Vec3 sphere_ray = local_ray.origin - Point3();
	double a = local_ray.direction().dot(local_ray.direction());
	double b = 2 * local_ray.direction().dot(sphere_ray);
	double c = sphere_ray.dot(sphere_ray) - 1;
	double discriminant = b * b - 4 * a * c;
	if (discriminant < 0) {
		return;
	}

	double mul = 1 / (2 * a);
	double sqrtdisc = sqrt(discriminant);
	double t1 = static_cast<double>((-b - sqrtdisc) * mul);
	out.append(Intersection(t1, this));
	double t2 = static_cast<double>((-b + sqrtdisc) * mul);
	out.append(Intersection(t2, this));
}

void Plane::local_intersect(const Ray& local_ray, IntersectionList& out) const {
	if (IsEqual(local_ray.direction().y, 0)) {
		// Ray is perpendicular to plane, no intersection.
		return;
	}

	double t = -local_ray.origin.y * local_ray.invdirection().y;
	out.append(Intersection(t, this));
}

std::pair<double, double> Cube::check_axis(double bmin, double bmax, double origin, double invdirection) {
	double minnum = bmin - origin;
	double maxnum = bmax - origin;

	double tmin = minnum * invdirection;
	double tmax = maxnum * invdirection;

	if (tmin > tmax) {
		std::swap(tmin, tmax);
	}

	return { tmin, tmax };
}

void Cube::local_intersect(const Ray& local_ray, IntersectionList& out) const {
	auto [xmin, xmax] = check_axis(-1, 1, local_ray.origin.x, local_ray.invdirection().x);
	auto [ymin, ymax] = check_axis(-1, 1, local_ray.origin.y, local_ray.invdirection().y);
	auto [zmin, zmax] = check_axis(-1, 1, local_ray.origin.z, local_ray.invdirection().z);

	// overall min, is largest of the per axis min.
	double tmin = std::max(xmin, std::max(ymin, zmin));
	double tmax = std::min(xmax, std::min(ymax, zmax));

	if (tmin < tmax) {
		out.append(Intersection(tmin, this));
		out.append(Intersection(tmax, this));
	}
}

Vec3 Cube::local_normal_at(const Point3& local_point) const {
	double ax = abs(local_point.x);
	double ay = abs(local_point.y);
	double az = abs(local_point.z);

	double maxdim = std::max(ax, std::max(ay, az));

	if (maxdim == ax) {
		return Vec3(local_point.x, 0, 0);
	} else if (maxdim == ay) {
		return Vec3(0, local_point.y, 0);
	}

	assert(maxdim == az);
	return Vec3(0,0,local_point.z);
}

void Cylinder::local_intersect(const Ray& local_ray, IntersectionList& out) const {
	double a = local_ray.direction().x * local_ray.direction().x + local_ray.direction().z * local_ray.direction().z;
	if (!IsEqual(a, 0)) {
		double b = 2 * (local_ray.origin.x * local_ray.direction().x + local_ray.origin.z * local_ray.direction().z);
		double c = local_ray.origin.x * local_ray.origin.x + local_ray.origin.z * local_ray.origin.z - 1;
		double discriminant = b * b - 4 * a * c;
		if (discriminant < 0) {
			// no intersection.
			return;
		}

		double mul = 1 / (2 * a);
		double sqrtdisc = sqrt(discriminant);
		double t0 = static_cast<double>((-b - sqrtdisc) * mul);
		double t1 = static_cast<double>((-b + sqrtdisc) * mul);

		if (t0 > t1) {
			std::swap(t0, t1);
		}

		double y0 = local_ray.origin.y + t0 * local_ray.direction().y;
		if (minimum < y0 && y0 < maximum) {
			out.append(Intersection(t0, this));
		}

		double y1 = local_ray.origin.y + t1 * local_ray.direction().y;
		if (minimum < y1 && y1 < maximum) {
			out.append(Intersection(t1, this));
		}
	}

	intersect_caps(local_ray, out);
}

void Cylinder::intersect_caps(const Ray& local_ray, IntersectionList& out) const {
	if (!closed) {
		return;
	}

	double t = (minimum - local_ray.origin.y) * local_ray.invdirection().y;
	if (check_cap(local_ray, t)) {
		out.append(Intersection(t, this));
	}

	t = (maximum - local_ray.origin.y) * local_ray.invdirection().y;
	if (check_cap(local_ray, t)) {
		out.append(Intersection(t, this));
	}
}

bool Cylinder::check_cap(const Ray& ray, double t) const {
	double x = ray.origin.x + ray.direction().x * t;
	double z = ray.origin.z + ray.direction().z * t;

	return (x * x + z * z) <= 1;
}

Vec3 Cylinder::local_normal_at(const Point3& local_point) const {
	double dist = local_point.x * local_point.x + local_point.z * local_point.z;

	// top cap
	if (dist < 1 && local_point.y >= maximum - RAY_EPSILON) {
		return Vec3(0, 1, 0);
	}

	// bottom cap.
	if (dist < 1 && local_point.y <= minimum + RAY_EPSILON) {
		return Vec3(0, -1, 0);
	}

	return Vec3(local_point.x, 0, local_point.z);
}

void Cone::local_intersect(const Ray& local_ray, IntersectionList& out) const {
	double a = local_ray.direction().x * local_ray.direction().x - local_ray.direction().y * local_ray.direction().y  + local_ray.direction().z * local_ray.direction().z;
	double b = 2 * (local_ray.origin.x * local_ray.direction().x - local_ray.origin.y * local_ray.direction().y + local_ray.origin.z * local_ray.direction().z);
	double c = local_ray.origin.x * local_ray.origin.x - local_ray.origin.y * local_ray.origin.y + local_ray.origin.z * local_ray.origin.z;

	if (IsEqual(a, 0)) {
		if (!IsEqual(b,0)) {
			double t = -c / (2 * b);
			double y = local_ray.origin.y + t * local_ray.direction().y;
			if (minimum < y && y < maximum) {
				out.append(Intersection(t, this));
			}
		}
	}
	else
	{
		double discriminant = b * b - 4 * a * c;
		if (discriminant < 0) {
			// no intersection.
			return;
		}

		double mul = 1 / (2 * a);
		double sqrtdisc = sqrt(discriminant);
		double t0 = static_cast<double>((-b - sqrtdisc) * mul);
		double t1 = static_cast<double>((-b + sqrtdisc) * mul);

		if (t0 > t1) {
			std::swap(t0, t1);
		}

		double y0 = local_ray.origin.y + t0 * local_ray.direction().y;
		if (minimum < y0 && y0 < maximum) {
			out.append(Intersection(t0, this));
		}

		double y1 = local_ray.origin.y + t1 * local_ray.direction().y;
		if (minimum < y1 && y1 < maximum) {
			out.append(Intersection(t1, this));
		}
	}

	intersect_caps(local_ray, out);
}

void Cone::intersect_caps(const Ray& local_ray, IntersectionList& out) const {
	if (!closed) {
		return;
	}

	// cone radius = cone Y = minimum for cap.
	double t = (minimum - local_ray.origin.y) * local_ray.invdirection().y;
	if (check_cap(local_ray, t, abs(minimum))) {
		out.append(Intersection(t, this));
	}

	t = (maximum - local_ray.origin.y) * local_ray.invdirection().y;
	if (check_cap(local_ray, t, abs(maximum))) {
		out.append(Intersection(t, this));
	}
}

bool Cone::check_cap(const Ray& ray, double t, double radius) const {
	double x = ray.origin.x + ray.direction().x * t;
	double z = ray.origin.z + ray.direction().z * t;

	return (x * x + z * z) <= radius;
}

Vec3 Cone::local_normal_at(const Point3& local_point) const {
	double dist = local_point.x * local_point.x + local_point.z * local_point.z;

	// top cap
	if (dist < 1 && local_point.y >= maximum - RAY_EPSILON) {
		return Vec3(0, 1, 0);
	}

	// bottom cap.
	if (dist < 1 && local_point.y <= minimum + RAY_EPSILON) {
		return Vec3(0, -1, 0);
	}

	double y = sqrt(local_point.x * local_point.x + local_point.z * local_point.z);
	if (local_point.y > 0) {
		y = -y;
	}

	return Vec3(local_point.x, y, local_point.z);
}

// Moller-Trumbore algorithm
// https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/moller-trumbore-ray-triangle-intersection
void Triangle::local_intersect(const Ray& local_ray, IntersectionList& out) const {
	Vec3 dir_cross_e2 = local_ray.direction().cross(e2);
	double det = e1.dot(dir_cross_e2);
	if (IsEqual(det, 0)) {
		// Ray is parallel to plane.
		return;
	}

	double f = 1 / det;

	Vec3 p1_to_origin = local_ray.origin - p1;
	double u = f * p1_to_origin.dot(dir_cross_e2);
	if (u < 0 || u > 1) {
		// Miss p1-p3 edge
		return;
	}

	Vec3 origin_cross_e1 = p1_to_origin.cross(e1);
	double v = f * local_ray.direction().dot(origin_cross_e1);
	if (v < 0 || (u + v) > 1) {
		// Miss other 2 edges.
		return;
	}

	double t = f * e2.dot(origin_cross_e1);
	out.append(Intersection(t, this));
}

Vec3 Triangle::local_normal_at(const Point3& local_point) const {
	return normal;
}

void Group::local_intersect(const Ray& local_ray, IntersectionList& out) const {
	if (_shapes.size() == 0) {
		// nothing to intersect.
		return;
	}

	{
		// Optimization from: https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection

		const Bounds& b = _bounds;
		// todo: copy and paste from cube.
		auto [tmin, tmax] = Cube::check_axis(b.min.x, b.max.x, local_ray.origin.x, local_ray.invdirection().x);
		auto [ymin, ymax] = Cube::check_axis(b.min.y, b.max.y, local_ray.origin.y, local_ray.invdirection().y);

		if (tmin > ymax || ymin > tmax) {
			return;
		}

		tmin = std::max(tmin, ymin);
		tmax = std::min(tmax, ymax);
		auto [zmin, zmax] = Cube::check_axis(b.min.z, b.max.z, local_ray.origin.z, local_ray.invdirection().z);

		if (tmin > zmax || zmin > tmax) {
			return;
		}
	}

	for (auto& shape : _shapes) {
		shape->intersect(local_ray, out);
	}
}

Vec3 Group::local_normal_at(const Point3& local_point) const {
	throw new std::runtime_error("its always an error to call group local_normal_at, should be called on children");
	return Vec3(0, 0, 1);
}

void Group::add(std::unique_ptr<Shape> shape) {
	shape->parent = this;

	Bounds b = shape->bounds();
	std::array<Point3, 8> corners = b.corners();

	for (const auto& p : corners) {
		Point3 w = shape->transform * p;
		_bounds.add(w);
	}

	_shapes.push_back(std::move(shape));
}

// In large scenes (1000 shapes), intersections are currently ~90% of cpu time. The goal is to get this smaller. Manually bucketing gets a 4x improvement. This tries to automate the process.
// tries to group shapes by space to reduce intersections. ideally from O(n) to O(logn). Algorithm:
// 1. compute cumulative bounds
// 2. for each shape: if bounds > 50% total, put in "huge" bucket
//		(we're going to be intersecting this shape a lot no matter what ..)
// 3. find center point, creating 8 buckets
// 4. foreach bucket: find the shape that adds the least space to a bucket center point 
//		(if new bounds>50% of cumulative, its a miss, so we might not use all 8 buckets)
// 4.1. if we only have 1 bucket, give up.
// 5. foreach shape: add to bucket which results in the smallest increase in volume
// 6. foreach bucket: if it has >5 items, recurse into bucket
// 7. add back huge
std::vector<std::unique_ptr<Shape>> NoopGroup::spatialize(std::vector<std::unique_ptr<Shape>>&& shapes) {

	if (shapes.size() < 8) {
		// No point in bucketizing small lists.
		return shapes;
	}

	std::unordered_map<Shape*, Bounds> txbounds;

	// Bounds are stored in shape-local space, need to transform them into parent space to make them comparable.
	// Also calculate cumulative bounds.
	Bounds cumulative;
	for (const auto& s : shapes) {
		Bounds shape_bounds;
		for (const auto& p : s->bounds().corners()) {
			Point3 t = s->transform * p;
			shape_bounds.add(t);
		}
		cumulative.add(shape_bounds);
		txbounds[s.get()] = shape_bounds;
	}

	// Set aside huge area shapes.
	std::vector<std::unique_ptr<Shape>> huge;
	double huge_area = cumulative.area() / 2;
	auto huge_it = std::partition(shapes.begin(), shapes.end(), [&](const auto& s) {
		return txbounds[s.get()].area() < huge_area;
		});
	if (huge_it != shapes.end()) {
		std::move(huge_it, shapes.end(), std::back_inserter(huge));
		shapes.erase(huge_it, shapes.end());
	}

	// Center of cumulative bounds.
	Point3 center = cumulative.center();

	// Buckets
	struct Bucket {
		Point3 bcenter;
		double min_area;
		Shape* min_shape;
		std::vector<std::unique_ptr<Shape>> vec;
		Bounds bounds;
	};
	std::array<Bucket, 8> bucket;

	// Center of buckets
	std::array<Point3, 8> corners = cumulative.corners();
	for (int i = 0; i < corners.size(); i++) {
		Bounds b;
		b.add(corners[i]);
		b.add(center);
		bucket[i].bcenter = b.center();
	}

	// Find best shape for bucket
	// Note that there is a weird case where 1 shape is best for 2 buckets.
	for (auto& b : bucket) {
		b.min_area = std::numeric_limits<double>::max();
		b.min_shape = nullptr;
		for (const auto& s : shapes) {
			Bounds bounds;
			bounds.add(b.bcenter);
			bounds.add(txbounds[s.get()]);
			double area = bounds.area();
			if (area < b.min_area && area < huge_area) {
				b.min_area = bounds.area();
				b.min_shape = s.get();
			}
		}
	}

	// Look for duplicate min_shapes
	// and update bounds.
	for (int i = 0; i < bucket.size(); i++) {
		Shape* s = bucket[i].min_shape;
		if (s != nullptr) {
			bucket[i].bounds = txbounds[s];
			for (int j = i + 1; j < bucket.size(); j++) {
				if (bucket[j].min_shape == s) {
					bucket[j].min_shape = nullptr;
				}
			}
		}
	}

	// Check if bucketing produced a meaningful divide. If not, reconstruct shapes and return it..
	int64_t buckets = std::count_if(std::begin(bucket), std::end(bucket), [](auto& b) { return b.min_shape != nullptr; });
	if (buckets < 2) {
		shapes.insert(shapes.end(), std::make_move_iterator(huge.begin()), std::make_move_iterator(huge.end()));
		return shapes;
	}

	// Foreach shape, add to bucket where it adds the least bounds.
	while (!shapes.empty()) {
		auto shape = std::move(shapes.back());
		shapes.pop_back();
		Bucket* min_bucket = nullptr;
		double min_area = std::numeric_limits<double>::max();
		for (auto& b : bucket) {
			if (b.min_shape == nullptr) {
				continue;
			}

			Bounds bounds = b.bounds;
			bounds.add(txbounds[shape.get()]);
			if (bounds.area() < min_area) {
				min_bucket = &b;
				min_area = bounds.area();
			}
		}

		min_bucket->vec.push_back(std::move(shape));
	}

	// Recurse into buckets
	for (auto& b : bucket) {
		if (b.min_shape == nullptr) {
			continue;
		}

		b.vec = spatialize(std::move(b.vec));
	}

	// rebuild bucketized shapes.
	assert(shapes.size() == 0);
	std::vector<std::unique_ptr<Shape>> spatialized;
	spatialized.insert(spatialized.end(), std::make_move_iterator(huge.begin()), std::make_move_iterator(huge.end()));

	for (auto& b : bucket) {
		if (b.min_shape == nullptr) {
			continue;
		}

		if (b.vec.empty()) {
			continue;
		}

		auto group = std::make_unique<NoopGroup>();

		while (!b.vec.empty()) {
			auto shape = std::move(b.vec.back());
			b.vec.pop_back();
			group->add(std::move(shape));
		}

		spatialized.push_back(std::move(group));
	}

	return spatialized;
}