#pragma once
#include "Tuple.h"
#include "Color.h"
namespace ray {
class PointLight
{
public:
	PointLight() = default;
	PointLight(const Point3& pos, const Color& c) : position(pos), intensity(c) {}
	Point3 position;
	Color intensity;
};
} // namespace ray

