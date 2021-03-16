#pragma once
#include "Shape.h"
#include "Tuple.h"
#include <algorithm>

namespace ray {
	class IntersectionInfo {
	public:
		double t = 0;
		const Shape* object = nullptr;
		Point3 point;
		Point3 over_point;
		Vec3 eye;
		Vec3 normal;
		Vec3 reflect;
		bool inside = false;
	};

	class Intersection
	{
	public:
		const Shape* object;
		double t;

		Intersection(double time, const Shape* shape) : t(time), object(shape) {}

		// Returns the smallest positive hit. As a raw pointer, straight into list.
		// Returns nullptr on miss.
		// list is expected to be sorted.
		static const Intersection* hit(const std::pmr::vector<Intersection>& list);
		static void sort(std::pmr::vector<Intersection>& v);
		IntersectionInfo info(const Ray& ray) const;
	};
}

