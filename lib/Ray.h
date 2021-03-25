#pragma once
#include "Tuple.h"
namespace ray {

class Ray
{
public:
	Point3 origin;

	Ray() = default;
	Ray(const Point3& o, const Vec3& d) :origin(o), directionv(d) {
		if (!IsEqual(0, d.x)) {
			inverse_direction.x = 1 / d.x;
		}
		else {
			inverse_direction.x = std::numeric_limits<double>::max();
		}
		if (!IsEqual(0, d.y)) {
			inverse_direction.y = 1 / d.y;
		}
		else {
			inverse_direction.y = std::numeric_limits<double>::max();
		}
		if (!IsEqual(0, d.z)) {
			inverse_direction.z = 1 / d.z;
		}
		else {
			inverse_direction.z = std::numeric_limits<double>::max();
		}
	}

	friend std::ostream& operator<<(std::ostream& os, const Ray& rhs) {
		return os << "Ray(" << rhs.origin << ", " << rhs.directionv << ")";
	}

	bool operator==(const Ray& rhs) const {
		return origin == rhs.origin && directionv == rhs.directionv;
	}
	bool operator!=(const Ray& rhs) const {
		return !operator==(rhs);
	}

	Point3 position(double t) const {
		return origin + directionv * t;
	}

	const Vec3& direction() const { return directionv; }
	const Vec3& invdirection() const { return inverse_direction; }

private:
	Vec3 directionv;
	// each component is 1/directionv's component. Allows replacing divides with multiplies.
	Vec3 inverse_direction;
};

} // namespace ray
