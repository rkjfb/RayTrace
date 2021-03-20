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

		// populates, and sorts, list with all intersection with r.
		void intersect(const Ray & r, IntersectionList& list) const {
			for (const auto& s : _shapes) {
				s->intersect(r, list);
			}
			list.sort();
		}

		// todo: replace recursion with iteration.
		Color color_at(const Ray& ray, int remaining = 5) const;
		bool is_shadowed(const Point3& point) const;
		Color shade(const IntersectionInfo& info, int remaining = 5) const;
		Color reflected_color(const IntersectionInfo& info, int remaining) const;
		Color refracted_color(const IntersectionInfo& info, int remaining) const;

	private:
		std::vector<std::unique_ptr<Shape>> _shapes;
		PointLight _light;
	};
} // namespace ray
