#include "pch.h"
#include "Shape.h"
#include "Intersect.h"

using namespace ray;

//Feature: Planes
//
//Scenario: The normal of a plane is constant everywhere
//  Given p ← plane()
//  When n1 ← local_normal_at(p, point(0, 0, 0))
//    And n2 ← local_normal_at(p, point(10, 0, -10))
//    And n3 ← local_normal_at(p, point(-5, 0, 150))
//  Then n1 = vector(0, 1, 0)
//    And n2 = vector(0, 1, 0)
//    And n3 = vector(0, 1, 0)
TEST(Plane, ConstantNormal) {
	Plane plane;
	EXPECT_EQ(plane.local_normal_at(Point3(0, 0, 0)), Vec3(0, 1, 0));
	EXPECT_EQ(plane.local_normal_at(Point3(10, 0, -10)), Vec3(0, 1, 0));
	EXPECT_EQ(plane.local_normal_at(Point3(-5, 0, 150)), Vec3(0, 1, 0));
}

//Scenario: Intersect with a ray parallel to the plane
//  Given p ← plane()
//    And r ← ray(point(0, 10, 0), vector(0, 0, 1))
//  When xs ← local_intersect(p, r)
//  Then xs is empty
TEST(Plane, IntersectParallel) {
	Plane plane;
	Ray r(Point3(0, 10, 0), Vec3(0, 0, 1));
	IntersectionList intersections;
	plane.local_intersect(r, intersections);
	EXPECT_EQ(intersections.size(), 0);
}

//Scenario: Intersect with a coplanar ray
//  Given p ← plane()
//    And r ← ray(point(0, 0, 0), vector(0, 0, 1))
//  When xs ← local_intersect(p, r)
//  Then xs is empty
TEST(Plane, IntersectCoplanar) {
	Plane plane;
	Ray r(Point3(0, 0, 0), Vec3(0, 0, 1));
	IntersectionList intersections;
	plane.local_intersect(r, intersections);
	EXPECT_EQ(intersections.size(), 0);
}

//Scenario: A ray intersecting a plane from above
//  Given p ← plane()
//    And r ← ray(point(0, 1, 0), vector(0, -1, 0))
//  When xs ← local_intersect(p, r)
//  Then xs.count = 1
//    And xs[0].t = 1
//    And xs[0].object = p
TEST(Plane, IntersectAbove) {
	Plane plane;
	Ray r(Point3(0, 1, 0), Vec3(0, -1, 0));
	char buffer[64] = {};
	IntersectionList intersections;
	plane.local_intersect(r, intersections);
	EXPECT_EQ(intersections.size(), 1);
	EXPECT_EQ(intersections.at(0).t, 1);
	EXPECT_EQ(intersections.at(0).object, &plane);
}

//Scenario: A ray intersecting a plane from below
//  Given p ← plane()
//    And r ← ray(point(0, -1, 0), vector(0, 1, 0))
//  When xs ← local_intersect(p, r)
//  Then xs.count = 1
//    And xs[0].t = 1
//    And xs[0].object = p
TEST(Plane, IntersectBelow) {
	Plane plane;
	Ray r(Point3(0, -1, 0), Vec3(0, 1, 0));
	IntersectionList intersections;
	plane.local_intersect(r, intersections);
	EXPECT_EQ(intersections.size(), 1);
	EXPECT_EQ(intersections.at(0).t, 1);
	EXPECT_EQ(intersections.at(0).object, &plane);
}
