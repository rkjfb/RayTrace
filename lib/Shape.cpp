#include "pch.h"
#include "Shape.h"
#include "Intersect.h"

using namespace ray;

void Sphere::local_intersect(const Ray& local_ray, IntersectionList& out) const {
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
	out.append(Intersection(t1, this));
	double t2 = static_cast<double>((-b + sqrtdisc) * mul);
	out.append(Intersection(t2, this));
}

void Plane::local_intersect(const Ray& local_ray, IntersectionList& out) const {
	if (IsEqual(local_ray.direction.y, 0)) {
		// Ray is perpendicular to plane, no intersection.
		return;
	}

	double t = -local_ray.origin.y / local_ray.direction.y;
	out.append(Intersection(t, this));
}

std::pair<double, double> Cube::check_axis(double origin, double direction) const {
	double minnum = -1 - origin;
	double maxnum = 1 - origin;

	double tmin, tmax;

	if (!IsEqual(direction, 0)) {
		tmin = minnum / direction;
		tmax = maxnum / direction;
	}
	else
	{
		tmin = minnum * std::numeric_limits<double>::max();
		tmax = maxnum * std::numeric_limits<double>::max();
	}

	if (tmin > tmax) {
		std::swap(tmin, tmax);
	}

	return { tmin, tmax };
}

void Cube::local_intersect(const Ray& local_ray, IntersectionList& out) const {
	auto [xmin, xmax] = check_axis(local_ray.origin.x, local_ray.direction.x);
	auto [ymin, ymax] = check_axis(local_ray.origin.y, local_ray.direction.y);
	auto [zmin, zmax] = check_axis(local_ray.origin.z, local_ray.direction.z);

	// overall min, is largest of the per axis min.
	double tmin = std::max(xmin, std::max(ymin, zmin));
	double tmax = std::min(xmax, std::min(ymax, zmax));

	if (tmin < tmax) {
		out.append(Intersection(tmin, this));
		out.append(Intersection(tmax, this));
	}
}

Vec3 Cube::local_normal_at(const Point3& local_point) const {
	double ax = abs(local_point.x);
	double ay = abs(local_point.y);
	double az = abs(local_point.z);

	double maxdim = std::max(ax, std::max(ay, az));

	if (maxdim == ax) {
		return Vec3(local_point.x, 0, 0);
	} else if (maxdim == ay) {
		return Vec3(0, local_point.y, 0);
	}

	assert(maxdim == az);
	return Vec3(0,0,local_point.z);
}

void Cylinder::local_intersect(const Ray& local_ray, IntersectionList& out) const {
	double a = local_ray.direction.x * local_ray.direction.x + local_ray.direction.z * local_ray.direction.z;
	if (IsEqual(a, 0)) {
		// ray parallel to y axis, not hit currently, since cylinder wall is infinitely thin.
		return;
	}

	double b = 2 * (local_ray.origin.x * local_ray.direction.x + local_ray.origin.z * local_ray.direction.z);
	double c = local_ray.origin.x * local_ray.origin.x + local_ray.origin.z * local_ray.origin.z - 1;
	double discriminant = b * b - 4 * a * c;
	if (discriminant < 0) {
		// no intersection.
		return;
	}

	double mul = 1 / (2 * a);
	double sqrtdisc = sqrt(discriminant);
	double t0 = static_cast<double>((-b - sqrtdisc) * mul);
	out.append(Intersection(t0, this));
	double t1 = static_cast<double>((-b + sqrtdisc) * mul);
	out.append(Intersection(t1, this));
}

Vec3 Cylinder::local_normal_at(const Point3& local_point) const {
	return Vec3(local_point.x, 0, local_point.z);
}

