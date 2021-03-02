#include "pch.h"
#include "Tuple.h"

namespace ray {
	bool IsEqual(double left, double right) {
		return abs(left - right) < RAY_EPSILON;
	}
} // namespace ray
