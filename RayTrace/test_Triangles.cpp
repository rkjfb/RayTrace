#include "pch.h"
#include "Tuple.h"
#include "Shape.h"
#include "Intersect.h"

using namespace ray;

//Feature: Triangles
//
//Scenario: Constructing a triangle
//  Given p1 ← point(0, 1, 0)
//    And p2 ← point(-1, 0, 0)
//    And p3 ← point(1, 0, 0)
//    And t ← triangle(p1, p2, p3)
//  Then t.p1 = p1
//    And t.p2 = p2
//    And t.p3 = p3
//    And t.e1 = vector(-1, -1, 0)
//    And t.e2 = vector(1, -1, 0)
//    And t.normal = vector(0, 0, -1)
TEST(Triangle, Ctor) {
	Point3 p1(0, 1, 0);
	Point3 p2(-1, 0, 0);
	Point3 p3 (1, 0, 0);
	Triangle t(p1, p2, p3);
	
	EXPECT_EQ(t.p1, p1);
	EXPECT_EQ(t.p2, p2);
	EXPECT_EQ(t.p3, p3);
	EXPECT_EQ(t.e1, Vec3(-1,-1,0));
	EXPECT_EQ(t.e2, Vec3(1,-1,0));
	EXPECT_EQ(t.normal, Vec3(0,0,-1));
}

//Scenario: Intersecting a ray parallel to the triangle
//  Given t ← triangle(point(0, 1, 0), point(-1, 0, 0), point(1, 0, 0))
//    And r ← ray(point(0, -1, -2), vector(0, 1, 0))
//  When xs ← local_intersect(t, r)
//  Then xs is empty
TEST(Triangle, IntersectParallel) {
	Point3 p1(0, 1, 0);
	Point3 p2(-1, 0, 0);
	Point3 p3(1, 0, 0);
	Triangle t(p1, p2, p3);

	Ray r(Point3(0, -1, -2), Vec3(0, 1, 0));

	IntersectionList xs;
	t.local_intersect(r, xs);
	EXPECT_EQ(xs.size(), 0);
}

//Scenario: A ray misses the p1-p3 edge
//  Given t ← triangle(point(0, 1, 0), point(-1, 0, 0), point(1, 0, 0))
//    And r ← ray(point(1, 1, -2), vector(0, 0, 1))
//  When xs ← local_intersect(t, r)
//  Then xs is empty
TEST(Triangle, MissP1P3) {
	Point3 p1(0, 1, 0);
	Point3 p2(-1, 0, 0);
	Point3 p3(1, 0, 0);
	Triangle t(p1, p2, p3);

	Ray r(Point3(1, 1, -2), Vec3(0, 0, 1));

	IntersectionList xs;
	t.local_intersect(r, xs);
	EXPECT_EQ(xs.size(), 0);
}

//Scenario: A ray misses the p1-p2 edge
//  Given t ← triangle(point(0, 1, 0), point(-1, 0, 0), point(1, 0, 0))
//    And r ← ray(point(-1, 1, -2), vector(0, 0, 1))
//  When xs ← local_intersect(t, r)
//  Then xs is empty
TEST(Triangle, MissP1P2) {
	Point3 p1(0, 1, 0);
	Point3 p2(-1, 0, 0);
	Point3 p3(1, 0, 0);
	Triangle t(p1, p2, p3);

	Ray r(Point3(-1, 1, -2), Vec3(0, 0, 1));

	IntersectionList xs;
	t.local_intersect(r, xs);
	EXPECT_EQ(xs.size(), 0);
}

//Scenario: A ray misses the p2-p3 edge
//  Given t ← triangle(point(0, 1, 0), point(-1, 0, 0), point(1, 0, 0))
//    And r ← ray(point(0, -1, -2), vector(0, 0, 1))
//  When xs ← local_intersect(t, r)
//  Then xs is empty
TEST(Triangle, MissP2P3) {
	Point3 p1(0, 1, 0);
	Point3 p2(-1, 0, 0);
	Point3 p3(1, 0, 0);
	Triangle t(p1, p2, p3);

	Ray r(Point3(0, -1, -2), Vec3(0, 0, 1));

	IntersectionList xs;
	t.local_intersect(r, xs);
	EXPECT_EQ(xs.size(), 0);
}

//Scenario: A ray strikes a triangle
//  Given t ← triangle(point(0, 1, 0), point(-1, 0, 0), point(1, 0, 0))
//    And r ← ray(point(0, 0.5, -2), vector(0, 0, 1))
//  When xs ← local_intersect(t, r)
//  Then xs.count = 1
//    And xs[0].t = 2
TEST(Triangle, Hit) {
	Point3 p1(0, 1, 0);
	Point3 p2(-1, 0, 0);
	Point3 p3(1, 0, 0);
	Triangle t(p1, p2, p3);

	Ray r(Point3(0, 0.5, -2), Vec3(0, 0, 1));

	IntersectionList xs;
	t.local_intersect(r, xs);
	EXPECT_EQ(xs.size(), 1);
	EXPECT_EQ(xs.at(0).t, 2);
}

//Scenario: Finding the normal on a triangle
//  Given t ← triangle(point(0, 1, 0), point(-1, 0, 0), point(1, 0, 0))
//  When n1 ← local_normal_at(t, point(0, 0.5, 0))
//    And n2 ← local_normal_at(t, point(-0.5, 0.75, 0))
//    And n3 ← local_normal_at(t, point(0.5, 0.25, 0))
//  Then n1 = t.normal
//    And n2 = t.normal
//    And n3 = t.normal
TEST(Triangle, Normal) {
	Point3 p1(0, 1, 0);
	Point3 p2(-1, 0, 0);
	Point3 p3(1, 0, 0);
	Triangle t(p1, p2, p3);

	Vec3 n = t.local_normal_at(Point3(0, 0.5, 0), Intersection(1, &t));
	EXPECT_EQ(n, t.normal);
	n = t.local_normal_at(Point3(-0.5, 0.75, 0), Intersection(1, &t));
	EXPECT_EQ(n, t.normal);
	n = t.local_normal_at(Point3(0.5, 0.25, 0), Intersection(1, &t));
	EXPECT_EQ(n, t.normal);
}
