#include "pch.h"

#include "Shape.h"
#include "Intersect.h"

using namespace ray;


//Feature: Smooth Triangles
//
//Background:
//  Given p1 ← point(0, 1, 0)
//    And p2 ← point(-1, 0, 0)
//    And p3 ← point(1, 0, 0)
//    And n1 ← vector(0, 1, 0)
//    And n2 ← vector(-1, 0, 0)
//    And n3 ← vector(1, 0, 0)
//  When tri ← smooth_triangle(p1, p2, p3, n1, n2, n3)
TEST(SmoothTriangle, Ctor) {
	Point3 p1(0, 1, 0);
	Point3 p2(-1, 0, 0);
	Point3 p3(1, 0, 0);
	Vec3 n1(0, 1, 0);
	Vec3 n2(-1, 0, 0);
	Vec3 n3(1, 0,0);

	SmoothTriangle tri(p1, p2, p3, n1, n2, n3);
	EXPECT_EQ(tri.p1, p1);
	EXPECT_EQ(tri.p2, p2);
	EXPECT_EQ(tri.p3, p3);
	EXPECT_EQ(tri.n1, n1);
	EXPECT_EQ(tri.n2, n2);
	EXPECT_EQ(tri.n3, n3);
}

//Scenario: Constructing a smooth triangle
//  Then tri.p1 = p1
//    And tri.p2 = p2
//    And tri.p3 = p3
//    And tri.n1 = n1
//    And tri.n2 = n2
//    And tri.n3 = n3
//
//Scenario: An intersection with a smooth triangle stores u/v
//  When r ← ray(point(-0.2, 0.3, -2), vector(0, 0, 1))
//    And xs ← local_intersect(tri, r)
//  Then xs[0].u = 0.45
//    And xs[0].v = 0.25
TEST(SmoothTriangle, IntersectUV) {
	Point3 p1(0, 1, 0);
	Point3 p2(-1, 0, 0);
	Point3 p3(1, 0, 0);
	Vec3 n1(0, 1, 0);
	Vec3 n2(-1, 0, 0);
	Vec3 n3(1, 0, 0);

	SmoothTriangle tri(p1, p2, p3, n1, n2, n3);
	Ray r(Point3(-0.2, 0.3, -2), Vec3(0, 0, 1));
	IntersectionList xs;
	tri.local_intersect(r, xs);
	
	EXPECT_NEAR(xs.at(0).u, 0.45, RAY_EPSILON);
	EXPECT_NEAR(xs.at(0).v, 0.25, RAY_EPSILON);
}

//Scenario: A smooth triangle uses u/v to interpolate the normal
//  When i ← intersection_with_uv(1, tri, 0.45, 0.25)
//    And n ← normal_at(tri, point(0, 0, 0), i)
//  Then n = vector(-0.5547, 0.83205, 0)
TEST(SmoothTriangle, IntersectUVNormal) {
	Point3 p1(0, 1, 0);
	Point3 p2(-1, 0, 0);
	Point3 p3(1, 0, 0);
	Vec3 n1(0, 1, 0);
	Vec3 n2(-1, 0, 0);
	Vec3 n3(1, 0, 0);

	SmoothTriangle tri(p1, p2, p3, n1, n2, n3);
	Intersection i(1, &tri, 0.45, 0.25);

	Vec3 n = tri.normal_at(Point3(), i);
	EXPECT_EQ(n, Vec3(-0.5547, 0.83205, 0));
}

//Scenario: Preparing the normal on a smooth triangle
//  When i ← intersection_with_uv(1, tri, 0.45, 0.25)
//    And r ← ray(point(-0.2, 0.3, -2), vector(0, 0, 1))
//    And xs ← intersections(i)
//    And comps ← prepare_computations(i, r, xs)
//  Then comps.normalv = vector(-0.5547, 0.83205, 0)
TEST(SmoothTriangle, IntersectUVNormalInfo) {
	Point3 p1(0, 1, 0);
	Point3 p2(-1, 0, 0);
	Point3 p3(1, 0, 0);
	Vec3 n1(0, 1, 0);
	Vec3 n2(-1, 0, 0);
	Vec3 n3(1, 0, 0);

	SmoothTriangle tri(p1, p2, p3, n1, n2, n3);
	Ray r(Point3(-0.2, 0.3, -2), Vec3(0, 0, 1));
	IntersectionList xs;
	xs.append(Intersection(1, &tri, 0.45, 0.25));

	IntersectionInfo info = xs.info(r, &xs.at(0));
	EXPECT_EQ(info.normal, Vec3(-0.5547, 0.83205, 0));
}
