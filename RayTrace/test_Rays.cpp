#include "pch.h"
#include "Ray.h"
#include "Matrix.h"

using namespace ray;

//Feature: Rays
//
//Scenario: Creating and querying a ray
//  Given origin ← point(1, 2, 3)
//    And direction ← vector(4, 5, 6)
//  When r ← ray(origin, direction)
//  Then r.origin = origin
//    And r.direction = direction
TEST(Ray, Ctor) {
	Point3 p(1, 2, 3);
	Vec3 v(4, 5, 6);
	Ray r(p, v);
	EXPECT_EQ(r.origin, p);
	EXPECT_EQ(r.direction, v);
}

//Scenario: Computing a point from a distance
//  Given r ← ray(point(2, 3, 4), vector(1, 0, 0))
//  Then position(r, 0) = point(2, 3, 4)
//    And position(r, 1) = point(3, 3, 4)
//    And position(r, -1) = point(1, 3, 4)
//    And position(r, 2.5) = point(4.5, 3, 4)
TEST(Ray, Position) {
	Point3 p(2, 3,4);
	Vec3 v(1,0,0);
	Ray r(p, v);
	EXPECT_EQ(r.position(0), Point3(2, 3, 4));
	EXPECT_EQ(r.position(1), Point3(3, 3, 4));
	EXPECT_EQ(r.position(-1), Point3(1, 3, 4));
	EXPECT_EQ(r.position(2.5), Point3(4.5, 3, 4));
}

//Scenario: Translating a ray
//  Given r ← ray(point(1, 2, 3), vector(0, 1, 0))
//    And m ← translation(3, 4, 5)
//  When r2 ← transform(r, m)
//  Then r2.origin = point(4, 6, 8)
//    And r2.direction = vector(0, 1, 0)
TEST(Ray, Translate) {
	Point3 p(1, 2, 3);
	Vec3 v(0, 1, 0);
	Ray r(p, v);
	Matrix4 m = Matrix4::translate(3, 4, 5);
	Ray result = m * r;
	EXPECT_EQ(result.origin, Point3(4, 6, 8));
	EXPECT_EQ(result.direction, Vec3(0,1,0));
}

//Scenario: Scaling a ray
//  Given r ← ray(point(1, 2, 3), vector(0, 1, 0))
//    And m ← scaling(2, 3, 4)
//  When r2 ← transform(r, m)
//  Then r2.origin = point(2, 6, 12)
//    And r2.direction = vector(0, 3, 0)
TEST(Ray, Scale) {
	Point3 p(1, 2, 3);
	Vec3 v(0, 1, 0);
	Ray r(p, v);
	Matrix4 m = Matrix4::scale(2, 3, 4);
	Ray result = m * r;
	EXPECT_EQ(result.origin, Point3(2, 6, 12));
	EXPECT_EQ(result.direction, Vec3(0, 3, 0));
}
