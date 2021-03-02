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

	friend std::ostream& operator<<(std::ostream& os, const Ray& rhs) {
		return os << "Ray(" << rhs.origin << ", " << rhs.direction << ")";
	}

	bool operator==(const Ray& rhs) const {
		return origin == rhs.origin && direction == rhs.direction;
	}
	bool operator!=(const Ray& rhs) const {
		return !operator==(rhs);
	}

	Point3 position(double t) const {
		return origin + direction * t;
	}
};

} // namespace ray
