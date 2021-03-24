#include "pch.h"
#include <stdexcept>

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
	double a = local_ray.direction.dot(local_ray.direction);
	double b = 2 * local_ray.direction.dot(sphere_ray);
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
	if (IsEqual(local_ray.direction.y, 0)) {
		// Ray is perpendicular to plane, no intersection.
		return;
	}

	double t = -local_ray.origin.y / local_ray.direction.y;
	out.append(Intersection(t, this));
}

std::pair<double, double> Cube::check_axis(double bmin, double bmax, double origin, double direction) {
	double minnum = bmin - origin;
	double maxnum = bmax - origin;

	double tmin, tmax;

	if (!IsEqual(direction, 0)) {
		tmin = minnum / direction;
		tmax = maxnum / direction;
	}
	else
	{
		tmin = minnum * std::numeric_limits<double>::max();
		tmax = maxnum * std::numeric_limits<double>::max();
	}

	if (tmin > tmax) {
		std::swap(tmin, tmax);
	}

	return { tmin, tmax };
}

void Cube::local_intersect(const Ray& local_ray, IntersectionList& out) const {
	auto [xmin, xmax] = check_axis(-1, 1, local_ray.origin.x, local_ray.direction.x);
	auto [ymin, ymax] = check_axis(-1, 1, local_ray.origin.y, local_ray.direction.y);
	auto [zmin, zmax] = check_axis(-1, 1, local_ray.origin.z, local_ray.direction.z);

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
	double a = local_ray.direction.x * local_ray.direction.x + local_ray.direction.z * local_ray.direction.z;
	if (!IsEqual(a, 0)) {
		double b = 2 * (local_ray.origin.x * local_ray.direction.x + local_ray.origin.z * local_ray.direction.z);
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

		double y0 = local_ray.origin.y + t0 * local_ray.direction.y;
		if (minimum < y0 && y0 < maximum) {
			out.append(Intersection(t0, this));
		}

		double y1 = local_ray.origin.y + t1 * local_ray.direction.y;
		if (minimum < y1 && y1 < maximum) {
			out.append(Intersection(t1, this));
		}
	}

	intersect_caps(local_ray, out);
}

void Cylinder::intersect_caps(const Ray& local_ray, IntersectionList& out) const {
	if (!closed || IsEqual(local_ray.direction.y, 0)) {
		return;
	}

	double t = (minimum - local_ray.origin.y) / local_ray.direction.y;
	if (check_cap(local_ray, t)) {
		out.append(Intersection(t, this));
	}

	t = (maximum - local_ray.origin.y) / local_ray.direction.y;
	if (check_cap(local_ray, t)) {
		out.append(Intersection(t, this));
	}
}

bool Cylinder::check_cap(const Ray& ray, double t) const {
	double x = ray.origin.x + ray.direction.x * t;
	double z = ray.origin.z + ray.direction.z * t;

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
	double a = local_ray.direction.x * local_ray.direction.x - local_ray.direction.y * local_ray.direction.y  + local_ray.direction.z * local_ray.direction.z;
	double b = 2 * (local_ray.origin.x * local_ray.direction.x - local_ray.origin.y * local_ray.direction.y + local_ray.origin.z * local_ray.direction.z);
	double c = local_ray.origin.x * local_ray.origin.x - local_ray.origin.y * local_ray.origin.y + local_ray.origin.z * local_ray.origin.z;

	if (IsEqual(a, 0)) {
		if (!IsEqual(b,0)) {
			double t = -c / (2 * b);
			double y = local_ray.origin.y + t * local_ray.direction.y;
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

		double y0 = local_ray.origin.y + t0 * local_ray.direction.y;
		if (minimum < y0 && y0 < maximum) {
			out.append(Intersection(t0, this));
		}

		double y1 = local_ray.origin.y + t1 * local_ray.direction.y;
		if (minimum < y1 && y1 < maximum) {
			out.append(Intersection(t1, this));
		}
	}

	intersect_caps(local_ray, out);
}

void Cone::intersect_caps(const Ray& local_ray, IntersectionList& out) const {
	if (!closed || IsEqual(local_ray.direction.y, 0)) {
		return;
	}

	// cone radius = cone Y = minimum for cap.
	double t = (minimum - local_ray.origin.y) / local_ray.direction.y;
	if (check_cap(local_ray, t, abs(minimum))) {
		out.append(Intersection(t, this));
	}

	t = (maximum - local_ray.origin.y) / local_ray.direction.y;
	if (check_cap(local_ray, t, abs(maximum))) {
		out.append(Intersection(t, this));
	}
}

bool Cone::check_cap(const Ray& ray, double t, double radius) const {
	double x = ray.origin.x + ray.direction.x * t;
	double z = ray.origin.z + ray.direction.z * t;

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

void Group::local_intersect(const Ray& local_ray, IntersectionList& out) const {
	if (_shapes.size() == 0) {
		// nothing to intersect.
		return;
	}

	{
		Bounds b = bounds();
		// todo: copy and paste from cube.
		auto [xmin, xmax] = Cube::check_axis(b.min.x, b.max.x, local_ray.origin.x, local_ray.direction.x);
		auto [ymin, ymax] = Cube::check_axis(b.min.y, b.max.y, local_ray.origin.y, local_ray.direction.y);
		auto [zmin, zmax] = Cube::check_axis(b.min.z, b.max.z, local_ray.origin.z, local_ray.direction.z);

		// overall min, is largest of the per axis min.
		double tmin = std::max(xmin, std::max(ymin, zmin));
		double tmax = std::min(xmax, std::min(ymax, zmax));

		if (tmin > tmax) {
			// bounding box empty, no need to check children.
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

// bugbug: don't want to compute this dynamically - grow bounds on shape add()?
Bounds Group::bounds() const {
	Bounds bounds;

	for (auto& shape : _shapes) {
		Bounds b = shape->bounds();
		std::array<Point3, 8> corners = {
			Point3(b.min.x, b.min.y, b.min.z),
			Point3(b.min.x, b.min.y, b.max.z),
			Point3(b.min.x, b.max.y, b.min.z),
			Point3(b.min.x, b.max.y, b.max.z),

			Point3(b.max.x, b.max.y, b.max.z),
			Point3(b.max.x, b.max.y, b.min.z),
			Point3(b.max.x, b.min.y, b.max.z),
			Point3(b.max.x, b.min.y, b.min.z),
		};

		for (const auto& p : corners) {
			Point3 w = shape->transform * p;
			bounds.add(w);
		}
	}

	return bounds;
}
