#include "pch.h"
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

void IntersectionList::sort() {
	struct {
		bool operator()(const Intersection& a, const Intersection& b) const {
			return a.t < b.t;
		}
	} customLess;
	std::sort(intersections.begin(), intersections.end(), customLess);
}

IntersectionInfo Intersection::info(const Ray& ray) const {
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
	info.reflect = ray.direction.reflect(info.normal);
	info.over_point = info.point + info.normal * ray::RAY_EPSILON;
	return info;
}
