#pragma once

#include "Shape.h"
#include "Intersect.h"
#include "Color.h"

namespace ray {
	class World
	{
	public:
		// specify all the things, nothing added.
		World(const PointLight& light, std::vector<std::unique_ptr<Shape>>&& shapes) :
			_light(light),
			_shapes(std::move(shapes)) {}

		// default: get default light + 2 stock shapes
		World() : World(PointLight(Point3(-10, 10, -10), Color(1, 1, 1))) {}

		// supply light + get 2 stock shapes
		World(const PointLight& light);

		const std::vector<std::unique_ptr<Shape>>& shapes() const {
			return _shapes;
		}

		const PointLight& light() {
			return _light;
		}

		void intersect(const Ray& r, std::vector<Intersection>& intersections) const {
			intersections.clear();
			for (const auto& s : _shapes) {
				s->intersect(r, intersections);
			}
			Intersection::sort(intersections);
		}

		Color shade_slow(const IntersectionInfo& info) const {
			std::vector<Intersection> intersections;
			return shade(info, intersections);
		}

		// intersections for speed
		Color shade(const IntersectionInfo& info, std::vector<Intersection>& intersections) const {
			const Material& material = info.object->material;
			bool in_shadow = is_shadowed(info.over_point, intersections);
			return material.lighting(_light, *info.object, info.over_point, info.eye, info.normal, in_shadow);
		}

		Color color_at_slow(const Ray& ray) const {
			std::vector<Intersection> intersections;
			return color_at(ray, intersections);
		}

		// intersections is supplied to dodge the allocation, no functional value.
		Color color_at(const Ray& ray, std::vector<Intersection>& intersections) const {
			intersect(ray, intersections);
			const Intersection* hit = Intersection::hit(intersections);
			if (hit == nullptr) {
				return Color::black();
			}
			IntersectionInfo info = hit->info(ray);
			return shade(info, intersections);
		}

		bool is_shadowed_slow(const Point3& point) const {
			std::vector<Intersection> intersections;
			return is_shadowed(point, intersections);
		}

		// intersections is only passed to get speed up from avoiding heap allocations.
		bool is_shadowed(const Point3& point, std::vector<Intersection>& intersections) const {
			Vec3 v = _light.position - point;
			double distance = v.magnitude();
			Vec3 direction = v.norm();
			// aim from point to light
			Ray r(point, direction);
			// see what you hit
			intersect(r, intersections);
			const Intersection* hit = Intersection::hit(intersections);
			// if you hit something and its closer than the light, you're in the shadows.
			if (hit != nullptr && hit->t < distance) {
				return true;
			}
			return false;
		}

	private:
		std::vector<std::unique_ptr<Shape>> _shapes;
		PointLight _light;
	};
} // namespace ray
