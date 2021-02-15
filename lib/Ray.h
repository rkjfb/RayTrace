#pragma once
#include "Tuple.h"
namespace ray {

class Ray
{
public:
	Point3 origin;
	Vec3 direction;

	Ray() = default;
	Ray(const Point3& o, const Vec3& d) :origin(o), direction(d) {}

	Point3 position(float t) {
		return origin + direction * t;
	}
};

} // namespace ray
