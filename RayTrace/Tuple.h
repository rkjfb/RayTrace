#pragma once
namespace ray {

// Equality
bool IsEqual(float left, float right);

class Vec3
{
public:
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float w = 0.0f;

	Vec3(float inx, float iny, float inz, float inw = 0.0f) {
		x = inx;
		y = iny;
		z = inz;
		w = inw;
	}

	friend std::ostream& operator<<(std::ostream& os, const Vec3& v) {
		return os << "Vec3(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
	}

	bool operator==(const Vec3& rhs) const {
		return IsEqual(x, rhs.x) && IsEqual(y, rhs.y) && IsEqual(z, rhs.z) && IsEqual(w, rhs.w);
	}
	bool operator!=(const Vec3& rhs) const {
		return !operator==(rhs);
	}

	Vec3 operator+(const Vec3& rhs) const {
		return Vec3(x + rhs.x, y + rhs.y, z + rhs.z);
	}
	Vec3 operator-(const Vec3& rhs) const {
		return Vec3(x - rhs.x, y -rhs.y, z - rhs.z);
	}

	Vec3 operator-() const {
		return Vec3(-x, -y, -z);
	}

	Vec3 operator*(float s) const {
		return Vec3(s* x, s* y, s* z, s*w);
	}

	static Vec3 zero() {
		return Vec3(0, 0, 0);
	}
};

class Point3
{
public:
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;

	Point3(float inx, float iny, float inz)
	{
		x = inx;
		y = iny;
		z = inz;
	}

	friend std::ostream& operator<<(std::ostream& os, const Point3& rhs) {
		return os << "Point3(" << rhs.x << ", " << rhs.y << ", " << rhs.z << ")";
	}

	bool operator==(const Point3& rhs) const {
		return IsEqual(x, rhs.x) && IsEqual(y, rhs.y) && IsEqual(z, rhs.z);
	}
	bool operator!=(const Point3& rhs) const {
		return !operator==(rhs);
	}

	Vec3 operator-(const Point3& rhs) const {
		return Vec3(x - rhs.x, y - rhs.y, z - rhs.z);
	}

	Point3 operator-() const {
		return Point3(-x, -y, -z);
	}

	Point3 operator+(const Vec3& rhs) const {
		return Point3(x + rhs.x, y + rhs.y, z + rhs.z);
	}
	Point3 operator-(const Vec3& rhs) const {
		return Point3(x - rhs.x, y - rhs.y, z - rhs.z);
	}

	Point3 operator*(const float& s) const {
		return Point3(s * x, s * y, s * z);
	}

	Point3 zero() const {
		return Point3(0, 0, 0);
	}
};
} // namespace ray
