#pragma once

#include "Shape.h"
#include "Intersect.h"
#include "Color.h"

namespace ray {
	class World
	{
	public:
		World(const PointLight& light, const std::vector<Sphere>& shapes) :
			_light(light)
		{
			for (const auto& s : shapes) {
				_spheres.push_back(s);
			}
		}

		// only light: get 2 stock shapes
		World(const PointLight& light) :
			_light(light)
		{
			std::vector<Sphere> shapes;

			Material m;
			m.color = Color(0.8f, 1, 0.6f);
			m.diffuse = 0.7f;
			m.specular = 0.2f;

			Sphere s1;
			s1.material = m;
			shapes.emplace_back(s1);

			Sphere s2(Matrix4::scale(0.5f, 0.5f, 0.5f));
			shapes.emplace_back(s2);
		}

		// default: get light + 2 stock shapes
		World() : World(PointLight(Point3(-10, 10, -10), Color(1, 1, 1)))
		{
		}

		// TODO: return const.
		std::vector<Sphere>& shapes() {
			return _spheres;
		}

		const PointLight& light() {
			return _light;
		}

		void intersect(const Ray& r, std::vector<Intersection>& intersections) const {
			for (const auto& s : _spheres) {
				Intersection::intersect(s, r, intersections);
			}
			Intersection::sort(intersections);
		}

		Color shade(const IntersectionInfo& info) const {
			const Material& material = info.object->material;
			return material.lighting(_light, info.point, info.eye, info.normal);
		}

		Color color_at_slow(const Ray& ray) const {
			std::vector<Intersection> intersections;
			return color_at(ray, intersections);
		}

		// intersections is supplied to dodge the allocation, no functional value.
		Color color_at(const Ray& ray, std::vector<Intersection>& intersections) const {
			intersections.clear();
			intersect(ray, intersections);
			if (intersections.size() == 0) {
				return Color::black();
			}
			IntersectionInfo info = Intersection::hit(intersections)->info(ray);
			return shade(info);
		}

	private:
		std::vector<Sphere> _spheres;
		PointLight _light;
	};
} // namespace ray
