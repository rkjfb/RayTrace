#include "pch.h"
#include "Intersect.h"

using namespace ray;

//Feature: Spheres
//
//Scenario: A ray intersects a sphere at two points
//  Given r ← ray(point(0, 0, -5), vector(0, 0, 1))
//    And s ← sphere()
//  When xs ← intersect(s, r)
//  Then xs.count = 2
//    And xs[0] = 4.0
//    And xs[1] = 6.0
TEST(Sphere, Intersect) {
	Ray r(Point3(0, 0, -5), Vec3(0, 0, 1));
	Sphere s;
	IntersectionList intersections;
	s.intersect(r, intersections);
	EXPECT_EQ(intersections.size(), 2);
	EXPECT_EQ(intersections.at(0).t, 4.0);
	EXPECT_EQ(intersections.at(1).t, 6.0);
}

//Scenario: A ray intersects a sphere at a tangent
//  Given r ← ray(point(0, 1, -5), vector(0, 0, 1))
//    And s ← sphere()
//  When xs ← intersect(s, r)
//  Then xs.count = 2
//    And xs[0] = 5.0
//    And xs[1] = 5.0
TEST(Sphere, IntersectTangent) {
	Ray r(Point3(0, 1, -5), Vec3(0, 0, 1));
	Sphere s;
	IntersectionList intersections;
	s.intersect(r, intersections);
	EXPECT_EQ(intersections.size(), 2);
	EXPECT_EQ(intersections.at(0).t, 5.0);
	EXPECT_EQ(intersections.at(1).t, 5.0);
}

//Scenario: A ray misses a sphere
//  Given r ← ray(point(0, 2, -5), vector(0, 0, 1))
//    And s ← sphere()
//  When xs ← intersect(s, r)
//  Then xs.count = 0
TEST(Sphere, IntersectMiss) {
	Ray r(Point3(0, 2, -5), Vec3(0, 0, 1));
	Sphere s;
	IntersectionList intersections;
	s.intersect(r, intersections);
	EXPECT_EQ(intersections.size(), 0);
}

//Scenario: A ray originates inside a sphere
//  Given r ← ray(point(0, 0, 0), vector(0, 0, 1))
//    And s ← sphere()
//  When xs ← intersect(s, r)
//  Then xs.count = 2
//    And xs[0] = -1.0
//    And xs[1] = 1.0
TEST(Sphere, IntersectInside) {
	Ray r(Point3(0, 0,0), Vec3(0, 0, 1));
	Sphere s;
	IntersectionList intersections;
	s.intersect(r, intersections);
	EXPECT_EQ(intersections.size(), 2);
	EXPECT_EQ(intersections.at(0).t, -1);
	EXPECT_EQ(intersections.at(1).t, 1);
}

//Scenario: A sphere is behind a ray
//  Given r ← ray(point(0, 0, 5), vector(0, 0, 1))
//    And s ← sphere()
//  When xs ← intersect(s, r)
//  Then xs.count = 2
//    And xs[0] = -6.0
//    And xs[1] = -4.0
TEST(Sphere, IntersectBehind) {
	Ray r(Point3(0, 0, 5), Vec3(0, 0, 1));
	Sphere s;
	IntersectionList intersections;
	s.intersect(r, intersections);
	EXPECT_EQ(intersections.size(), 2);
	EXPECT_EQ(intersections.at(0).t, -6);
	EXPECT_EQ(intersections.at(1).t, -4);
}

//Scenario: Intersect sets the object on the intersection
//  Given r ← ray(point(0, 0, -5), vector(0, 0, 1))
//    And s ← sphere()
//  When xs ← intersect(s, r)
//  Then xs.count = 2
//    And xs[0].object = s
//    And xs[1].object = s
TEST(Sphere, IntersectSetObject) {
	Ray r(Point3(0, 0, -5), Vec3(0, 0, 1));
	Sphere s;
	IntersectionList intersections;
	s.intersect(r, intersections);
	EXPECT_EQ(intersections.size(), 2);
	EXPECT_EQ(intersections.at(0).object, &s);
	EXPECT_EQ(intersections.at(1).object, &s);
}

//Scenario: A sphere's default transformation
//  Given s ← sphere()
//  Then s.transform = identity_matrix
TEST(Sphere, CtorTransformIdent) {
	Sphere s;
	EXPECT_EQ(s.transform, Matrix4::identity());
}

//Scenario: Changing a sphere's transformation
//  Given s ← sphere()
//    And t ← translation(2, 3, 4)
//  When set_transform(s, t)
//  Then s.transform = t
TEST(Sphere, CtorTransformTranslate) {
	Matrix4 translate = Matrix4::translate(2, 3, 4);
	Sphere s(translate);
	EXPECT_EQ(s.transform, translate);
}

//Scenario: Intersecting a scaled sphere with a ray
//  Given r ← ray(point(0, 0, -5), vector(0, 0, 1))
//    And s ← sphere()
//  When set_transform(s, scaling(2, 2, 2))
//    And xs ← intersect(s, r)
//  Then xs.count = 2
//    And xs[0].t = 3
//    And xs[1].t = 7
TEST(Sphere, IntersectScaled) {
	Ray r(Point3(0, 0, -5), Vec3(0, 0, 1));
	Sphere s(Matrix4::scale(2,2,2));
	IntersectionList intersections;
	s.intersect(r, intersections);
	EXPECT_EQ(intersections.size(), 2);
	EXPECT_EQ(intersections.at(0).t, 3);
	EXPECT_EQ(intersections.at(1).t, 7);
}

//Scenario: Intersecting a translated sphere with a ray
//  Given r ← ray(point(0, 0, -5), vector(0, 0, 1))
//    And s ← sphere()
//  When set_transform(s, translation(5, 0, 0))
//    And xs ← intersect(s, r)
//  Then xs.count = 0
TEST(Sphere, IntersectTranslateMiss) {
	Ray r(Point3(0, 0, -5), Vec3(0, 0, 1));
	Sphere s(Matrix4::translate(5,0,0));
	IntersectionList intersections;
	s.intersect(r, intersections);
	EXPECT_EQ(intersections.size(), 0);
}

//Scenario: The normal on a sphere at a point on the x axis
//  Given s ← sphere()
//  When n ← normal_at(s, point(1, 0, 0))
//  Then n = vector(1, 0, 0)
TEST(Sphere, NormalX) {
	Sphere s;
	Vec3 n = s.normal_at(Point3(1, 0, 0));
	EXPECT_EQ(n, Vec3(1,0,0));
}

//Scenario: The normal on a sphere at a point on the y axis
//  Given s ← sphere()
//  When n ← normal_at(s, point(0, 1, 0))
//  Then n = vector(0, 1, 0)
TEST(Sphere, NormalY) {
	Sphere s;
	Vec3 n = s.normal_at(Point3(0, 1, 0));
	EXPECT_EQ(n, Vec3(0, 1, 0));
}

//Scenario: The normal on a sphere at a point on the z axis
//  Given s ← sphere()
//  When n ← normal_at(s, point(0, 0, 1))
//  Then n = vector(0, 0, 1)
TEST(Sphere, NormalZ) {
	Sphere s;
	Vec3 n = s.normal_at(Point3(0, 0, 1));
	EXPECT_EQ(n, Vec3(0, 0, 1));
}

//Scenario: The normal on a sphere at a nonaxial point
//  Given s ← sphere()
//  When n ← normal_at(s, point(√3/3, √3/3, √3/3))
//  Then n = vector(√3/3, √3/3, √3/3)
TEST(Sphere, NormalNonAxial) {
	Sphere s;
	float tt = sqrtf(3) / 3;
	Vec3 n = s.normal_at(Point3(tt, tt, tt));
	EXPECT_EQ(n, Vec3(tt,tt,tt));
}

//Scenario: The normal is a normalized vector
//  Given s ← sphere()
//  When n ← normal_at(s, point(√3/3, √3/3, √3/3))
//  Then n = normalize(n)
TEST(Sphere, NormalNonAxialNorm) {
	Sphere s;
	float tt = sqrtf(3) / 3;
	Vec3 n = s.normal_at(Point3(tt, tt, tt));
	Vec3 result = n.norm();
	EXPECT_EQ(result, n);
}
//Scenario: Computing the normal on a translated sphere
//  Given s ← sphere()
//    And set_transform(s, translation(0, 1, 0))
//  When n ← normal_at(s, point(0, 1.70711, -0.70711))
//  Then n = vector(0, 0.70711, -0.70711)
TEST(Sphere, NormalTranslate) {
	Sphere s(Matrix4::translate(0,1,0));
	Vec3 n = s.normal_at(Point3(0, 1.70711f, -0.70711f));
	EXPECT_EQ(n, Vec3(0, 0.70711f, -0.70711f));
}

//Scenario: Computing the normal on a transformed sphere
//  Given s ← sphere()
//    And m ← scaling(1, 0.5, 1) * rotation_z(π/5)
//    And set_transform(s, m)
//  When n ← normal_at(s, point(0, √2/2, -√2/2))
//  Then n = vector(0, 0.97014, -0.24254)
TEST(Sphere, NormalTransform) {
	Matrix4 t = Matrix4::scale(1, 0.5, 1) * Matrix4::rotateZ(pi / 5);
	Sphere s(t);
	float st = sqrtf(2) / 2;
	Vec3 n = s.normal_at(Point3(0, st,-st));
	EXPECT_EQ(n, Vec3(0, 0.97014f, -0.24254f));
}

//Scenario: A sphere has a default material
//  Given s ← sphere()
//  When m ← s.material
//  Then m = material()
TEST(Sphere, Material) {
	Sphere s;
	EXPECT_EQ(s.material, Material());
}

//Scenario: A sphere may be assigned a material
//  Given s ← sphere()
//    And m ← material()
//    And m.ambient ← 1
//  When s.material ← m
//  Then s.material = m
TEST(Sphere, MaterialAssign) {
	Material m;
	m.ambient = 1;
	Sphere s;
	s.material = m;
	EXPECT_EQ(s.material, m);
}

//Scenario: A helper for producing a sphere with a glassy material
//  Given s ← glass_sphere()
//  Then s.transform = identity_matrix
//    And s.material.transparency = 1.0
//    And s.material.refractive_index = 1.5
// TODO