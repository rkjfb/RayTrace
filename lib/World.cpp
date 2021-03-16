#include "pch.h"
#include "World.h"
#include <memory_resource>

using namespace ray;

// only light: get 2 stock shapes
World::World(const PointLight& light) :
	_light(light)
{
	_shapes = make_default_shapes();
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

// intersections is supplied to dodge the allocation, no functional value.
Color World::color_at(const Ray& ray, int remaining) const {
	IntersectionList list;
	intersect(ray, list);
	const Intersection* hit = list.hit();
	if (hit == nullptr) {
		return Color::black();
	}
	IntersectionInfo info = hit->info(ray);
	return shade(info, remaining);
}
