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

		static std::vector<std::unique_ptr<Shape>> make_default_shapes();

		const std::vector<std::unique_ptr<Shape>>& shapes() const {
			return _shapes;
		}

		const PointLight& light() {
			return _light;
		}

		// void intersect(const Ray& r, std::vector<Intersection>& intersections) const {
		void intersect(const Ray & r, std::pmr::vector<Intersection>& intersections) const {
			intersections.clear();
			for (const auto& s : _shapes) {
				s->intersect(r, intersections);
			}
			Intersection::sort(intersections);
		}

		Color shade_slow(const IntersectionInfo& info) const {
			std::pmr::vector<Intersection> intersections; // bugbug
			return shade(info, intersections);
		}

		// intersections for speed
		Color shade(const IntersectionInfo& info, std::pmr::vector<Intersection>& intersections, int remaining = 5) const {
			const Material& material = info.object->material;
			bool in_shadow = is_shadowed(info.over_point, intersections);
			Color surface = material.lighting(_light, *info.object, info.over_point, info.eye, info.normal, in_shadow);
			Color reflection = reflected_color_slow(info, remaining);
			return surface + reflection;
		}

		Color color_at_slow(const Ray& ray, int remaining = 5) const;
		Color color_at(const Ray& ray, int remaining = 5) const;

		bool is_shadowed_slow(const Point3& point) const {
			std::pmr::vector<Intersection> intersections; //bugbug
			return is_shadowed(point, intersections);
		}

		// intersections is only passed to get speed up from avoiding heap allocations.
		bool is_shadowed(const Point3& point, std::pmr::vector<Intersection>& intersections) const {
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

		Color reflected_color_slow(const IntersectionInfo& info, int remaining) const {

			if (remaining <= 0) {
				return Color::black();
			}

			double reflective = info.object->material.reflective;

			if (IsEqual(reflective, 0)) {
				return Color::black();
			}

			Ray reflect_ray(info.over_point, info.reflect);
			Color c = color_at_slow(reflect_ray, remaining - 1);
			return c * reflective;
		}


	private:
		std::vector<std::unique_ptr<Shape>> _shapes;
		PointLight _light;
	};
} // namespace ray
