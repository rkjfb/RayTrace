#include "pch.h"
#include "Shape.h"
#include "Intersect.h"

using namespace ray;

void Sphere::local_intersect(const Ray& local_ray, std::vector<Intersection>& out) const {
	Vec3 sphere_ray = local_ray.origin - Point3();
	double a = local_ray.direction.dot(local_ray.direction);
	double b = 2 * local_ray.direction.dot(sphere_ray);
	double c = sphere_ray.dot(sphere_ray) - 1;
	double discriminant = b * b - 4 * a * c;
	if (discriminant < 0) {
		return;
	}

	double mul = 1 / (2 * a);
	double sqrtdisc = sqrt(discriminant);
	double t1 = static_cast<double>((-b - sqrtdisc) * mul);
	out.emplace_back(Intersection(t1, this));
	double t2 = static_cast<double>((-b + sqrtdisc) * mul);
	out.emplace_back(Intersection(t2, this));
}

void Plane::local_intersect(const Ray& local_ray, std::vector<Intersection>& out) const {
	if (IsEqual(local_ray.direction.y, 0)) {
		// Ray is perpendicular to plane, no intersection.
		return;
	}

	double t = -local_ray.origin.y / local_ray.direction.y;
	out.emplace_back(Intersection(t, this));
}
