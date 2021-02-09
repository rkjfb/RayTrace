#pragma once
class Tuple
{
public:
	Tuple(float inx, float iny, float inz, float inw) {
		x = inx;
		y = iny;
		z = inz;
		w = inw;
	}
	
	bool IsEqual(float left, float right) {
		return abs(left - right) < 0.0001;
	}

	bool IsVec3() {
		return IsEqual(w, 0.0f);
	}

	bool IsPoint3() {
		return !IsVec3();
	}

	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float w = 0.0f;
};

