#pragma once
#include "Shape.h"
#include "Tuple.h"
#include <algorithm>

namespace ray {
	class IntersectionInfo {
	public:
		double t = 0;
		// refractive index from
		double n1 = 1;
		// refractive index to
		double n2 = 1;
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
		IntersectionInfo infox(const Ray& ray) const;
	};

	class IntersectionList
	{
	public:
		IntersectionList() {
			// take all of buffer in a single allocation.
			size_t newsize = sizeof(buffer) / sizeof(Intersection);
			intersections.reserve(newsize);
		}

		// Returns the smallest positive hit. As a raw pointer, straight into list.
		// Returns nullptr on miss.
		const Intersection* hit();
		void sort();
		void append(Intersection&& intersection) {
			intersections.push_back(std::move(intersection));
		}

		// Return info at hit. List is needed for refractions.
		IntersectionInfo info(const Ray& ray, const Intersection* hit) const;

		size_t size() {
			return intersections.size();
		}

		void clear() {
			return intersections.clear();
		}

		const Intersection& at(size_t index) {
			return intersections[index];
		}

	private:
		// avoid heap allocations for small vectors. 
		// 128 was found by trial and error in profiler for chapter 7 scene.
		char buffer[128];
		std::pmr::monotonic_buffer_resource pool{ std::data(buffer), std::size(buffer) };
		std::pmr::vector<Intersection> intersections{ &pool };
	};

}

