#include "pch.h"
#include <set>

#include "Intersect.h"
#include "Shape.h"
using namespace ray;

const Intersection* IntersectionList::hit() {
	for (const auto& i : intersections) {
		if (i.t >= 0) {
			return &i;
		}
	}

	return nullptr;
}

const Intersection* IntersectionList::hit_ignoretransparent() {
	for (const auto& i : intersections) {
		if (i.object->material.transparency > 0.9) {
			// ignore very transparent things, like water..
			continue;
		}

		if (i.t >= 0) {
			return &i;
		}
	}

	return nullptr;
}

void IntersectionList::sort() {
	struct {
		bool operator()(const Intersection& a, const Intersection& b) const {
			return a.t < b.t;
		}
	} customLess;
	std::sort(intersections.begin(), intersections.end(), customLess);
}

IntersectionInfo Intersection::infox(const Ray& ray) const {
	IntersectionInfo info;
	info.t = t;
	info.object = object;
	info.point = ray.position(info.t);
	info.eye = -ray.direction();
	info.normal = info.object->normal_at(info.point);
	if (info.normal.dot(info.eye) < 0) {
		info.inside = true;
		info.normal = -info.normal;
	}
	info.reflect = ray.direction().reflect(info.normal);
	info.over_point = info.point + info.normal * ray::RAY_EPSILON;
	info.under_point = info.point - info.normal * ray::RAY_EPSILON;
	return info;
}

IntersectionInfo IntersectionList::info(const Ray& ray, const Intersection* hit) const {
	IntersectionInfo info = hit->infox(ray);

	char buffer[64];
	std::pmr::monotonic_buffer_resource pool{ std::data(buffer), std::size(buffer) };
	std::pmr::vector<const Shape*> containers{ &pool };

	for (const auto& i : intersections) {
		if (&i == hit) {
			if (containers.empty()) {
				info.n1 = 1;
			}
			else
			{
				const Shape* last = *containers.rbegin();
				info.n1 = last->material.refractive_index;
			}
		}

		auto it = std::find(containers.begin(), containers.end(), i.object);
		if (it != containers.end()) {
			containers.erase(it);
		}
		else
		{
			containers.emplace_back(i.object);
		}

		if (&i == hit) {
			if (containers.empty()) {
				info.n2 = 1;
			}
			else
			{
				const Shape* last = *containers.rbegin();
				info.n2 = last->material.refractive_index;
			}

			break;
		}
	}

	return info;
}
