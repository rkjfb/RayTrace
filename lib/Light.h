#pragma once
#include "Tuple.h"
#include "Color.h"
namespace ray {
class PointLight
{
public:
	PointLight() = default;
	PointLight(const Point3& pos, const Color& c) : position(pos), intensity(c) {}

	friend std::ostream& operator<<(std::ostream& os, const PointLight& rhs) {
		return os << "PointLight(" << rhs.position << ", " << rhs.intensity << ")";
	}

	bool operator==(const PointLight& rhs) const {
		return position == rhs.position && intensity == rhs.intensity;
	}
	bool operator!=(const PointLight& rhs) const {
		return !operator==(rhs);
	}

	Point3 position;
	Color intensity;
};
} // namespace ray

