#pragma once
#include "Matrix.h"
#include "Material.h"
namespace ray {
class Sphere
{
public:
	Matrix4 transform;
	Material material;
	Sphere() = default;
	Sphere(const Matrix4& t) : transform(t) {}

	friend std::ostream& operator<<(std::ostream& os, const Sphere& rhs) {
		return os << "Sphere(" << rhs.transform << ", " << rhs.material << ")";
	}

	bool operator==(const Sphere& rhs) const {
		return transform == rhs.transform && material == rhs.material;
	}
	bool operator!=(const Sphere& rhs) const {
		return !operator==(rhs);
	}

	Vec3 normal_at(Point3 world_point) const {
		Matrix4 inv = transform.inverse();
		Point3 object_point = inv * world_point;

		Vec3 object_normal = object_point - Point3(0, 0, 0);
		Matrix4 transpose = inv.transpose();
		Vec3 world_normal = transpose * object_normal;
		world_normal.w = 0;
		return world_normal.norm();
	}
};

} // namespace ray