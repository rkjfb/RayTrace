#pragma once
#include "Shape.h"
#include "Tuple.h"
#include <algorithm>

namespace ray {
	class IntersectionInfo {
	public:
		double t = 0;
		const Sphere* object = nullptr;
		Point3 point;
		Point3 over_point;
		Vec3 eye;
		Vec3 normal;
		bool inside = false;
	};

	class Intersection
	{
	public:
		const Sphere* object;
		double t;

		Intersection(double time, const Sphere& o) : t(time), object(&o) {}

		// vector re-use to avoid high heap cpu time.
		// FUTURE: Make an Intersections object that wraps the vector and maybe has non-linear search (positive heap?) and hit caching.
		static void intersect(const Sphere& s, const Ray& inr, std::vector<Intersection>& out) {
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
			double t1 = static_cast<double>((-b - sqrtdisc) * mul);
			out.emplace_back(Intersection(t1, s));
			double t2 = static_cast<double>((-b + sqrtdisc) * mul);
			out.emplace_back(Intersection(t2, s));
		}

		// Returns the smallest positive hit. As a raw pointer, straight into list.
		// Returns nullptr on miss.
		static const Intersection* hit(const std::vector<Intersection>& list) {
			const Intersection* ret = nullptr;
			double minpositive = FLT_MAX;
			for (const auto& i : list) {
				if (i.t >= 0 && i.t < minpositive) {
					ret = &i;
					minpositive = i.t;
				}
			}

			return ret;
		}

		static void sort(std::vector<Intersection>& v) {
			struct {
				bool operator()(const Intersection& a, const Intersection& b) const { 
					return a.t < b.t; 
				}
			} customLess;
			std::sort(v.begin(), v.end(), customLess);
		}

		IntersectionInfo info(const Ray& ray) const {
			IntersectionInfo info;
			info.t = t;
			info.object = object;
			info.point = ray.position(info.t);
			info.eye = -ray.direction;
			info.normal = info.object->normal_at(info.point);
			if (info.normal.dot(info.eye) < 0) {
				info.inside = true;
				info.normal = -info.normal;
			}
			info.over_point = info.point + info.normal * ray::RAY_EPSILON;
			return info;
		}
	};
}

