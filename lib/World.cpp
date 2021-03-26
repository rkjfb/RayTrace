#include "pch.h"
#include <memory_resource>
#include <unordered_map>

#include "World.h"

using namespace ray;

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
std::vector<std::unique_ptr<Shape>> World::spatialize(std::vector<std::unique_ptr<Shape>>&& shapes) {

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
	auto huge_it = std::partition(shapes.begin(), shapes.end(), [&] (const auto& s) {
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
	std::array<Bucket,8> bucket;

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

	// Check if bucketing produced a meaningful divide. If not, reconstruct shapes and return it..
	int64_t buckets = std::count_if(std::begin(bucket), std::end(bucket), [](auto& b) { return b.min_shape != nullptr; });
	if (buckets < 2) {
		shapes.insert(shapes.end(), std::make_move_iterator(huge.begin()), std::make_move_iterator(huge.end()));
		return shapes;
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

std::vector<std::unique_ptr<Shape>> World::make_default_shapes()
{
	std::vector<std::unique_ptr<Shape>> vec;

	Material m;
	m.pattern = std::make_unique<Solid>(0.8, 1, 0.6);
	m.diffuse = 0.7f;
	m.specular = 0.2f;

	auto s1 = std::make_unique<Sphere>();
	s1->material = m;
	vec.emplace_back(std::move(s1));

	auto s2 = std::make_unique<Sphere>(Matrix4::scale(0.5f, 0.5f, 0.5f));
	vec.emplace_back(std::move(s2));

	return vec;
}

void World::intersect(const Ray& r, IntersectionList& list) const {
	// bugbug: add some sort of spatial structure here, so we don't need to visit O(n) shapes.
	for (const auto& s : _shapes) {
		s->intersect(r, list);
	}
	list.sort();
}

Color World::color_at(const Ray& ray, int remaining) const {
	IntersectionList list;
	intersect(ray, list);
	const Intersection* hit = list.hit();
	if (hit == nullptr) {
		return Color::black();
	}
	IntersectionInfo info = list.info(ray, hit);
	return shade(info, remaining);
}

bool World::is_shadowed(const Point3& point) const {
	Vec3 v = _light.position - point;
	double distance = v.magnitude();
	Vec3 direction = v.norm();
	// aim from point to light
	Ray ray(point, direction);

	IntersectionList list;
	intersect(ray, list);

	const Intersection* hit = list.hit_ignoretransparent();
	//const Intersection* hit = list.hit();

	// if you hit something and its closer than the light, you're in the shadows.
	if (hit != nullptr && hit->t < distance) {
		return true;
	}
	return false;
}

Color World::shade(const IntersectionInfo& info, int remaining) const {
	const Material& material = info.object->material;
	bool in_shadow = is_shadowed(info.over_point);
	Color surface = material.lighting(_light, *info.object, info.over_point, info.eye, info.normal, in_shadow);
	Color reflected = reflected_color(info, remaining);
	Color refracted = refracted_color(info, remaining);

	if (material.reflective > 0 && material.transparency > 0) {
		double reflectance = info.schlick();
		return surface + reflected * reflectance + refracted * (1 - reflectance);
	}

	return surface + reflected + refracted;
}

Color World::reflected_color(const IntersectionInfo& info, int remaining) const {

	if (remaining <= 0) {
		return Color::black();
	}

	double reflective = info.object->material.reflective;

	// fully absorbtive -> no reflection color
	if (IsEqual(reflective, 0)) {
		return Color::black();
	}

	Ray reflect_ray(info.over_point, info.reflect);
	Color c = color_at(reflect_ray, remaining - 1);
	return c * reflective;
}

Color World::refracted_color(const IntersectionInfo& info, int remaining) const {

	// check recursion
	if (remaining <= 0) {
		return Color::black();
	}

	// no transparency -> no refraction color
	double transparency = info.object->material.transparency;
	if (IsEqual(transparency, 0)) {
		return Color::black();
	}

	// total internal reflection (using Snell's law) -> no refraction color
	double n_ratio = info.n1 / info.n2;
	double cos_i = info.eye.dot(info.normal);
	// trig identity
	double sin2_t = n_ratio * n_ratio * (1 - cos_i * cos_i);
	if (sin2_t > 1) {
		return Color::black();
	}

	double cos_t = sqrt(1 - sin2_t);
	Vec3 direction = info.normal * (n_ratio * cos_i - cos_t) - info.eye * n_ratio;
	Ray refract(info.under_point, direction);

	return color_at(refract, remaining - 1) * transparency;
}
