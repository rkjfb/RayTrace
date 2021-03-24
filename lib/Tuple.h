#pragma once
#include <cassert>
#include <ostream>
#include <array>

namespace ray {

	static constexpr double RAY_EPSILON = 0.00001f;
	// available in c++20 as std::number::pi
	static constexpr double pi = 3.14159265358979323846f;
// Equality
bool IsEqual(double left, double right);

class Vec3
{
public:
	double x = 0.0f;
	double y = 0.0f;
	double z = 0.0f;
	double w = 0.0f;

	Vec3() = default;
	Vec3(double inx, double iny, double inz, double inw = 0.0f) :x(inx), y(iny), z(inz), w(inw) {}
	Vec3(std::array<double, 4> a) :x(a[0]), y(a[1]), z(a[2]), w(a[3]) {}

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

	Vec3 operator*(double s) const {
		return Vec3(s* x, s* y, s* z, s*w);
	}

	double magnitude() const {
		return sqrt(x * x + y * y + z * z);
	}

	Vec3 norm() const {
		double magmul = 1 / magnitude();
		return Vec3(static_cast<double>(x * magmul), static_cast<double>(y * magmul), static_cast<double>(z * magmul));
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
		double d = static_cast<double>(dot(n));
		return *this - n * 2 * d;
	}

	static Vec3 zero() {
		return Vec3(0, 0, 0);
	}
};

class Point3
{
public:
	double x = 0.0f;
	double y = 0.0f;
	double z = 0.0f;

	Point3() = default;
	Point3(double inx, double iny, double inz) : x(inx), y(iny), z(inz) {}
	Point3(std::array<double, 3> a) : x(a[0]), y(a[1]), z(a[2]) {}

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

	Point3 operator*(const double& s) const {
		return Point3(s * x, s * y, s * z);
	}

	static Point3 zero() {
		return Point3(0, 0, 0);
	}
};

// Axis-aligned bound box.
class Bounds {
public:
	Bounds() {
		// start with inverted max bounds, so we'll replace all bounds on first iteration.
		double m = std::numeric_limits<double>::max();
		min = Point3(m, m, m);
		max = Point3(-m, -m, -m);
	}

	Bounds(const Point3& inmin, const Point3& inmax) : min(inmin), max(inmax) {}
	Point3 min;
	Point3 max;

	// Add p to bounds.
	void add(const Point3& p) {
		min.x = std::min(min.x, p.x);
		min.y = std::min(min.y, p.y);
		min.z = std::min(min.z, p.z);

		max.x = std::max(max.x, p.x);
		max.y = std::max(max.y, p.y);
		max.z = std::max(max.z, p.z);
	}
};

} // namespace ray
