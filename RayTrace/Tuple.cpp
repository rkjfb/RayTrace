#include "pch.h"
#include "Tuple.h"

namespace ray {
	bool IsEqual(float left, float right) {
		return abs(left - right) < 0.0001;
	}
} // namespace ray
