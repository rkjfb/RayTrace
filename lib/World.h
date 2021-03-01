#pragma once

#include "Shape.h"
#include "Intersect.h"

namespace ray {
	class World
	{
	public:

		World() :
			_light(Point3(-10, 10, -10), Color(1, 1, 1))
		{
			Material m;
			m.color = Color(0.8f, 1, 0.6f);
			m.diffuse = 0.7f;
			m.specular = 0.2f;

			Sphere s1;
			s1.material = m;
			_spheres.emplace_back(s1);

			Sphere s2(Matrix4::scale(0.5f, 0.5f, 0.5f));
			_spheres.emplace_back(s2);
		}

		const std::vector<Sphere>& shapes() {
			return _spheres;
		}

		const PointLight& light() {
			return _light;
		}

		void intersect(const Ray& r, std::vector<Intersection>& intersections) {
			for (const auto& s : _spheres) {
				Intersection::intersect(s, r, intersections);
			}
			Intersection::sort(intersections);
		}

	private:
		std::vector<Sphere> _spheres;
		PointLight _light;
	};
} // namespace ray
