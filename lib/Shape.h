#pragma once
#include "Matrix.h"

namespace ray {
class Sphere
{
public:
	Matrix4 transform;
	Sphere() = default;
	Sphere(const Matrix4& t) : transform(t) {}
};

} // namespace ray