#pragma once
#include "Shape.h"
#include "Tuple.h"

namespace ray {
	class Intersection
	{
	public:
		const Sphere* object;
		float t;

		Intersection(float time, const Sphere& o) : t(time), object(&o) {}

		// vector re-use to avoid high heap cpu time.
		// FUTURE: Make an Intersections object that wraps the vector and maybe has non-linear search (positive heap?) and hit caching.
		static void intersect(const Sphere& s, const Ray& inr, std::vector<Intersection>& out) {
			out.clear();

			Matrix4 inv = s.transform.inverse();
			Ray r = inv * inr;
			Vec3 sphere_ray = r.origin - Point3();
			double a = r.direction.dot(r.direction);
			double b = 2 * r.direction.dot(sphere_ray);
			double c = sphere_ray.dot(sphere_ray) - 1;
			double discriminant = b * b - 4 * a * c;
			if (discriminant < 0) {
				return;
			}

			double mul = 1 / (2 * a);
			double sqrtdisc = sqrt(discriminant);
			float t1 = static_cast<float>((-b - sqrtdisc) * mul);
			out.emplace_back(Intersection(t1, s));
			float t2 = static_cast<float>((-b + sqrtdisc) * mul);
			out.emplace_back(Intersection(t2, s));
		}

		// Returns the smallest positive hit. As a raw pointer, straight into list.
		// Returns nullptr on miss.
		static const Intersection* hit(const std::vector<Intersection>& list) {
			const Intersection* ret = nullptr;
			float minpositive = FLT_MAX;
			for (const auto& i : list) {
				if (i.t >= 0 && i.t < minpositive) {
					ret = &i;
					minpositive = i.t;
				}
			}

			return ret;
		}
	};
}
