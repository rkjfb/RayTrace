#pragma once
#include "Matrix.h"
#include "Material.h"

namespace ray {
	class Intersection;

	class Shape {
	public:
		Matrix4 transform;
		Material material;
		Shape() = default;
		Shape(const Matrix4& t) : transform(t) {}

		friend std::ostream& operator<<(std::ostream& os, const Shape& rhs) {
			return os << "Shape(" << rhs.transform << ", " << rhs.material << ")";
		}

		bool operator==(const Shape& rhs) const {
			return transform == rhs.transform && material == rhs.material;
		}
		bool operator!=(const Shape& rhs) const {
			return !operator==(rhs);
		}

		virtual Vec3 local_normal_at(const Point3& world_point) const = 0;

		Vec3 normal_at(const Point3& world_point) const {
			Matrix4 inv = transform.inverse();
			Point3 local_point = inv * world_point;

			Vec3 local_normal = local_normal_at(local_point);

			Matrix4 transpose = inv.transpose();
			Vec3 world_normal = transpose * local_normal;
			world_normal.w = 0;
			return world_normal.norm();
		}

		virtual void local_intersect(const Ray& inr, std::vector<Intersection>& out) const = 0;

		void intersect(const Ray& inr, std::vector<Intersection>& out) const {
			Matrix4 inv = transform.inverse();
			Ray local_ray = inv * inr;

			local_intersect(local_ray, out);
		}

	};

	class Sphere : public Shape
	{
	public:
		Sphere() = default;
		Sphere(const Matrix4& t) : Shape(t) {}

		friend std::ostream& operator<<(std::ostream& os, const Sphere& rhs) {
			return os << "Sphere(" << rhs.transform << ", " << rhs.material << ")";
		}

		bool operator==(const Sphere& rhs) const {
			return Shape::operator==(rhs);
		}

		bool operator!=(const Sphere& rhs) const {
			return !operator==(rhs);
		}

		Vec3 local_normal_at(const Point3& local_point) const override {
			return local_point - Point3(0, 0, 0);
		}

		void local_intersect(const Ray& local_ray, std::vector<Intersection>& out) const override;
	};

	// Used for unit testing Shape.
	class TestShape : public Shape
	{
	public:
		TestShape() = default;
		TestShape(const Matrix4& t) : Shape(t) {}

		// unit test intermediate info
		mutable Point3 saved_point;
		mutable Ray saved_ray;

		friend std::ostream& operator<<(std::ostream& os, const TestShape& rhs) {
			return os << "TestShape(" << rhs.transform << ", " << rhs.material << ")";
		}

		bool operator==(const TestShape& rhs) const {
			return Shape::operator==(rhs);
		}

		bool operator!=(const TestShape& rhs) const {
			return !operator==(rhs);
		}

		Vec3 local_normal_at(const Point3& local_point) const override {
			saved_point = local_point;
			return Vec3(local_point.x, local_point.y, local_point.z);
		}

		void local_intersect(const Ray& local_ray, std::vector<Intersection>& out) const override
		{
			saved_ray = local_ray;
		}
	};

} // namespace ray