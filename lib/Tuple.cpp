#include "pch.h"
#include "Tuple.h"

namespace ray {
	bool IsEqual(float left, float right) {
		return abs(left - right) < RAY_EPSILON;
	}
} // namespace ray
