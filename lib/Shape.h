#pragma once
#include <memory_resource>  
#include "Matrix.h"
#include "Material.h"

// todo: lots of duplicate code (eg. cone&cylinder, cube&aabb)

namespace ray {
	class Intersection;
	class IntersectionList;

	class Shape {
	public:
		Matrix4 transform;
		Material material;
		Shape* parent = nullptr;

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

		virtual Bounds bounds() const = 0;

		virtual Vec3 local_normal_at(const Point3& world_point, const Intersection& hit) const = 0;

		Vec3 normal_at(const Point3& world_point, const Intersection& hit) const;
		virtual void local_intersect(const Ray& inr, IntersectionList& out) const = 0;
		virtual void intersect(const Ray& inr, IntersectionList& out) const;
		Point3 world_to_object(Point3 point) const;
		Vec3 normal_to_world(Vec3 normal) const;
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

		Bounds bounds() const override {
			return Bounds(Point3(-1, -1, -1), Point3(1, 1, 1));
		}

		Vec3 local_normal_at(const Point3& local_point, const Intersection& hit) const override {
			return local_point - Point3(0, 0, 0);
		}

		void local_intersect(const Ray& local_ray, IntersectionList& out) const override;

		// Returns a glass sphere.
		static std::unique_ptr<Sphere> glass() {
			auto ret = std::make_unique<Sphere>();
			ret->material = Material::glass();
			return ret;
		}
	};

	class Plane : public Shape
	{
	public:
		Plane() = default;
		Plane(const Matrix4& t) : Shape(t) {}

		friend std::ostream& operator<<(std::ostream& os, const Plane& rhs) {
			return os << "Sphere(" << rhs.transform << ", " << rhs.material << ")";
		}

		bool operator==(const Plane& rhs) const {
			return Shape::operator==(rhs);
		}

		bool operator!=(const Plane& rhs) const {
			return !operator==(rhs);
		}

		Bounds bounds() const override {
			double m = std::numeric_limits<double>::max();
			return Bounds(Point3(-m, 0, -m), Point3(m, 0, m));
		}

		Vec3 local_normal_at(const Point3& local_point, const Intersection& hit) const override {
			return Vec3(0,1,0);
		}

		void local_intersect(const Ray& local_ray, IntersectionList& out) const override;
	};

	class Cube : public Shape
	{
	public:
		Cube() = default;
		Cube(const Matrix4& t) : Shape(t) {}

		friend std::ostream& operator<<(std::ostream& os, const Cube& rhs) {
			return os << "Cube(" << rhs.transform << ", " << rhs.material << ")";
		}

		bool operator==(const Cube& rhs) const {
			return Cube::operator==(rhs);
		}

		bool operator!=(const Cube& rhs) const {
			return !operator==(rhs);
		}

		Bounds bounds() const override {
			return Bounds(Point3(-1, -1, -1), Point3(1, 1, 1));
		}

		Vec3 local_normal_at(const Point3& local_point, const Intersection& hit) const override;
		void local_intersect(const Ray& local_ray, IntersectionList& out) const override;

		static std::pair<double, double> check_axis(double bmin, double bmax, double origin, double direction);
	};

	class Cylinder : public Shape
	{
	public:
		double minimum = -std::numeric_limits<double>::max();
		double maximum = std::numeric_limits<double>::max();
		bool closed = false;

		Cylinder() = default;
		Cylinder(const Matrix4& t) : Shape(t) {}

		friend std::ostream& operator<<(std::ostream& os, const Cylinder& rhs) {
			return os << "Cylinder(" << rhs.transform << ", " << rhs.material << ")";
		}

		bool operator==(const Cylinder& rhs) const {
			return Cylinder::operator==(rhs);
		}

		bool operator!=(const Cylinder& rhs) const {
			return !operator==(rhs);
		}

		Bounds bounds() const override {
			return Bounds(Point3(-1, minimum, -1), Point3(1, maximum, 1));
		}

		Vec3 local_normal_at(const Point3& local_point, const Intersection& hit) const override;
		void local_intersect(const Ray& local_ray, IntersectionList& out) const override;

	private:
		void intersect_caps(const Ray& local_ray, IntersectionList& out) const;
		bool check_cap(const Ray& ray, double t) const;

	};

	class Cone : public Shape
	{
	public:
		double minimum = -std::numeric_limits<double>::max();
		double maximum = std::numeric_limits<double>::max();
		bool closed = false;

		Cone() = default;
		Cone(const Matrix4& t) : Shape(t) {}

		friend std::ostream& operator<<(std::ostream& os, const Cone& rhs) {
			return os << "Cone(" << rhs.transform << ", " << rhs.material << ")";
		}

		bool operator==(const Cone& rhs) const {
			return Cone::operator==(rhs);
		}

		bool operator!=(const Cone& rhs) const {
			return !operator==(rhs);
		}

		Bounds bounds() const override {
			return Bounds(Point3(-1, minimum, -1), Point3(1, maximum, 1));
		}

		Vec3 local_normal_at(const Point3& local_point, const Intersection& hit) const override;
		void local_intersect(const Ray& local_ray, IntersectionList& out) const override;

	private:
		void intersect_caps(const Ray& local_ray, IntersectionList& out) const;
		bool check_cap(const Ray& ray, double t, double radius) const;

	};

	class Triangle : public Shape
	{
	public:
		Point3 p1, p2, p3;
		Vec3 e1, e2, normal;

		Triangle(const Point3& inp1, const Point3& inp2, const Point3& inp3) : p1(inp1), p2(inp2), p3(inp3) {
			e1 = p2 - p1;
			e2 = p3 - p1;
			normal = e2.cross(e1).norm();
		}

		friend std::ostream& operator<<(std::ostream& os, const Triangle& rhs) {
			return os << "Triangle(" << rhs.transform << ", " << rhs.material << ")";
		}

		bool operator==(const Triangle& rhs) const {
			return Shape::operator==(rhs) && p1 == rhs.p1 && p2 == rhs.p2 && p3 == rhs.p3 && e1 == rhs.e1 && e2 == rhs.e2 && normal == rhs.normal;
		}

		bool operator!=(const Triangle& rhs) const {
			return !operator==(rhs);
		}

		Bounds bounds() const override {
			Bounds b;
			b.add(p1);
			b.add(p2);
			b.add(p3);
			return b;
		}

		Vec3 local_normal_at(const Point3& local_point, const Intersection& hit) const override;
		void local_intersect(const Ray& local_ray, IntersectionList& out) const override;
	};

	class SmoothTriangle : public Triangle {
	public:
		Vec3 n1, n2, n3;
		SmoothTriangle(const Point3& inp1, const Point3& inp2, const Point3& inp3, const Vec3& in1, const Vec3& in2, const Vec3& in3) : Triangle(inp1, inp2, inp3), n1(in1), n2(in2), n3(in3) {}
		Vec3 local_normal_at(const Point3& local_point, const Intersection& hit) const override;
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

		Bounds bounds() const override {
			return Bounds(Point3(-1, -1, -1), Point3(1, 1, 1));
		}

		Vec3 local_normal_at(const Point3& local_point, const Intersection& hit) const override {
			saved_point = local_point;
			return Vec3(local_point.x, local_point.y, local_point.z);
		}

		void local_intersect(const Ray& local_ray, IntersectionList& out) const override
		{
			saved_ray = local_ray;
		}
	};

	class Group : public Shape
	{
	public:
		Bounds _bounds;

		Group() = default;
		Group(const Matrix4& t) : Shape(t) {}

		friend std::ostream& operator<<(std::ostream& os, const Group& rhs) {
			return os << "Group(" << rhs.transform << ", " << rhs.material << ")";
		}

		bool operator==(const Group& rhs) const {
			return Group::operator==(rhs);
		}

		bool operator!=(const Group& rhs) const {
			return !operator==(rhs);
		}

		Bounds bounds() const override { return _bounds; }
		Vec3 local_normal_at(const Point3& local_point, const Intersection& hit) const override;
		void local_intersect(const Ray& local_ray, IntersectionList& out) const override;

		size_t size() {
			return _shapes.size();
		}

		void add(std::unique_ptr<Shape> shape);

		const std::vector<std::unique_ptr<Shape>>& shapes() const {
			return _shapes;
		}

		// removes group shape array
		std::vector<std::unique_ptr<Shape>> extract_shapes() {
			return std::move(_shapes);
		}

	private:
		std::vector<std::unique_ptr<Shape>> _shapes;
	};

	// A group without a transform, used by spatialize.
	class NoopGroup : public Group {
		void intersect(const Ray& local_ray, IntersectionList& out) const override {
			return local_intersect(local_ray, out);
		}

	public:
		// Inserts NoopGroups into shapes to get spatial locality. Reducing intersection 20x in some scenes.
		static std::vector<std::unique_ptr<Shape>> spatialize(std::vector<std::unique_ptr<Shape>>&& shapes);
	};




} // namespace ray
