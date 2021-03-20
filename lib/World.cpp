#include "pch.h"
#include "World.h"
#include <memory_resource>

using namespace ray;

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
	const Intersection* hit = list.hit();
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
	Color reflection = reflected_color(info, remaining);
	Color refraction = refracted_color(info, remaining);
	return surface + reflection + refraction;
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
