#pragma once
#include <cassert>
#include <ostream>
#include <array>

namespace ray {

	static constexpr float RAY_EPISILON = 0.0001f;
	// available in c++20 as std::number::pi
	static constexpr float pi = 3.14159265358979323846f;
// Equality
bool IsEqual(float left, float right);

class Vec3
{
public:
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float w = 0.0f;

	Vec3() = default;

	Vec3(float inx, float iny, float inz, float inw = 0.0f) {
		x = inx;
		y = iny;
		z = inz;
		w = inw;
	}

	Vec3(std::array<float, 4> a) {
		x = a[0];
		y = a[1];
		z = a[2];
		w = a[3];
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

	double magnitude() const {
		return sqrt(x * x + y * y + z * z);
	}

	Vec3 norm() const {
		double magmul = 1 / magnitude();
		return Vec3(static_cast<float>(x * magmul), static_cast<float>(y * magmul), static_cast<float>(z * magmul));
	}

	double dot(const Vec3& v) const {
		return x * v.x + y * v.y + z * v.z;
	}

	Vec3 cross(const Vec3& v) const {
		return Vec3(y *v.z -z*v.y, z * v.x-x*v.z, x * v.y -y*v.x);
	}

	// n should be normalized.
    Vec3 reflect(const Vec3& n) const {
		assert(n == n.norm());
		float d = static_cast<float>(dot(n));
		return *this - n * 2 * d;
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

	Point3() = default;
	Point3(float inx, float iny, float inz)
	{
		x = inx;
		y = iny;
		z = inz;
	}
	Point3(std::array<float, 3> a) {
		x = a[0];
		y = a[1];
		z = a[2];
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

	static Point3 zero() {
		return Point3(0, 0, 0);
	}
};
} // namespace ray
