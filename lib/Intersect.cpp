#include "pch.h"
#include "Intersect.h"
#include "Shape.h"
using namespace ray;

const Intersection* Intersection::hit(const std::vector<Intersection>& list) {
	for (const auto& i : list) {
		if (i.t >= 0) {
			return &i;
		}
	}

	return nullptr;
}

void Intersection::sort(std::vector<Intersection>& v) {
	struct {
		bool operator()(const Intersection& a, const Intersection& b) const {
			return a.t < b.t;
		}
	} customLess;
	std::sort(v.begin(), v.end(), customLess);
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
	info.over_point = info.point + info.normal * ray::RAY_EPSILON;
	return info;
}
