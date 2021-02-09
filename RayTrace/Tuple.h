#pragma once
class Tuple
{
protected:
	// Can only instantiate a point or a vector.
	Tuple(float inx, float iny, float inz, float inw) {
		x = inx;
		y = iny;
		z = inz;
		w = inw;
	}
public:
	bool IsEqual(float left, float right) const {
		return abs(left - right) < 0.0001;
	}

	bool IsVec3() const {
		return IsEqual(w, 0.0f);
	}

	bool IsPoint3() const {
		return !IsVec3();
	}

	bool operator==(const Tuple& rhs) const {
		return IsEqual(x, rhs.x) && IsEqual(y, rhs.y) && IsEqual(z, rhs.z) && IsEqual(w, rhs.w);
	}
	bool operator!=(const Tuple& rhs) const {
		return !operator==(rhs);
	}

	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float w = 0.0f;
};

class Vec3 : public Tuple
{
public:
	Vec3(float inx, float iny, float inz) :
		Tuple(inx, iny, inz, 0.0f) {}

	Vec3 operator+(const Vec3& rhs) const {
		return Vec3(x + rhs.x, y + rhs.y, z + rhs.z);
	}
	Vec3 operator-(const Vec3& rhs) const {
		return Vec3(x - rhs.x, y -rhs.y, z - rhs.z);
	}

	static Vec3 zero() {
		return Vec3(0, 0, 0);
	}

};

class Point3 : public Tuple
{
public:
	Point3(float inx, float iny, float inz) :
		Tuple(inx, iny, inz, 1.0f) {}

	Vec3 operator-(const Point3& rhs) const {
		return Vec3(x - rhs.x, y - rhs.y, z - rhs.z);
	}

	Point3 operator+(const Vec3& rhs) const {
		return Point3(x + rhs.x, y + rhs.y, z + rhs.z);
	}
	Point3 operator-(const Vec3& rhs) const {
		return Point3(x - rhs.x, y - rhs.y, z - rhs.z);
	}

};