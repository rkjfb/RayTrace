#include "pch.h"
#include "Pattern.h"
#include "Shape.h"

using namespace ray;

Color Pattern::pattern_at_shape(const Shape& shape, const Point3& p) const {
	Point3 object_point = shape.transform.inverse_multiply(p);
	Point3 pattern_point = transform.inverse_multiply(object_point);
	return pattern_at(pattern_point);
}

