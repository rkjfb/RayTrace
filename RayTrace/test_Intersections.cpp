#include "pch.h"
#include "Tuple.h"
#include "Intersect.h"

using namespace ray;

//Feature: Intersections
//
//Scenario: An intersection encapsulates t and object
//  Given s ← sphere()
//  When i ← intersection(3.5, s)
//  Then i.t = 3.5
//    And i.object = s
TEST(Intersect, Ctor) {
	Sphere s;
	Intersection i(3.5, &s);
	EXPECT_EQ(i.t, 3.5);
	EXPECT_EQ(i.object, &s);
}

//Scenario: Precomputing the state of an intersection
//  Given r ← ray(point(0, 0, -5), vector(0, 0, 1))
//    And shape ← sphere()
//    And i ← intersection(4, shape)
//  When comps ← prepare_computations(i, r)
//  Then comps.t = i.t
//    And comps.object = i.object
//    And comps.point = point(0, 0, -1)
//    And comps.eyev = vector(0, 0, -1)
//    And comps.normalv = vector(0, 0, -1)
TEST(Intersect, Info) {
	Ray ray(Point3(0, 0, -5), Vec3(0, 0, 1));
	Sphere s;
	Intersection i(4, &s);
	IntersectionInfo info = i.infox(ray);
	EXPECT_EQ(info.t, 4);
	EXPECT_EQ(info.object, &s);
	EXPECT_EQ(info.point, Point3(0,0,-1));
	EXPECT_EQ(info.eye, Vec3(0,0,-1));
	EXPECT_EQ(info.normal, Vec3(0,0,-1));
}

//Scenario: Precomputing the reflection vector
//  Given shape ← plane()
//    And r ← ray(point(0, 1, -1), vector(0, -√2/2, √2/2)) 
//    And i ← intersection(√2, shape)                      
//  When comps ← prepare_computations(i, r)
//  Then comps.reflectv = vector(0, √2/2, √2/2)                
TEST(Intersect, InfoReflective) {
	Plane shape;
	double ss = sqrt(2);
	Ray ray(Point3(0, 1, -1), Vec3(0, -ss/2, ss/2));
	Intersection i(ss, &shape);
	IntersectionInfo info = i.infox(ray);
	EXPECT_EQ(info.reflect, Vec3(0, ss/2, ss/2));
}

//Scenario: The hit, when an intersection occurs on the outside
//  Given r ← ray(point(0, 0, -5), vector(0, 0, 1))
//    And shape ← sphere()
//    And i ← intersection(4, shape)
//  When comps ← prepare_computations(i, r)
//  Then comps.inside = false
TEST(Intersect, Outside) {
	Ray ray(Point3(0, 0,-5), Vec3(0, 0,1));
	Sphere s;
	Intersection i(4, &s);
	IntersectionInfo info = i.infox(ray);
	EXPECT_EQ(info.inside, false);
}

//Scenario: The hit, when an intersection occurs on the inside
//  Given r ← ray(point(0, 0, 0), vector(0, 0, 1))
//    And shape ← sphere()
//    And i ← intersection(1, shape)
//  When comps ← prepare_computations(i, r)
//  Then comps.point = point(0, 0, 1)
//    And comps.eyev = vector(0, 0, -1)
//    And comps.inside = true
//      # normal would have been (0, 0, 1), but is inverted!
//    And comps.normalv = vector(0, 0, -1)
TEST(Intersect, Inside) {
	Ray ray(Point3(0, 0, 0), Vec3(0, 0, 1));
	Sphere s;
	Intersection i(1, &s);
	IntersectionInfo info = i.infox(ray);
	EXPECT_EQ(info.point, Point3(0,0,1));
	EXPECT_EQ(info.eye, Vec3(0,0,-1));
	EXPECT_EQ(info.inside, true);
	EXPECT_EQ(info.normal, Vec3(0,0,-1));
}

//Scenario: The hit should offset the point
//  Given r ← ray(point(0, 0, -5), vector(0, 0, 1))
//    And shape ← sphere() with:
//      | transform | translation(0, 0, 1) |
//    And i ← intersection(5, shape)
//  When comps ← prepare_computations(i, r)
//  Then comps.over_point.z < -EPSILON/2
//    And comps.point.z > comps.over_point.z
TEST(Intersect, OffsetPoint) {
	Ray ray(Point3(0, 0, -5), Vec3(0, 0, 1));
	Sphere shape;
	shape.transform = Matrix4::translate(0, 0, 1);
	Intersection i(5, &shape);
	IntersectionInfo info = i.infox(ray);
	EXPECT_GT(ray::RAY_EPSILON / 2, info.point.z);
	EXPECT_GT(info.point.z, info.over_point.z);
}

//Scenario: The under point is offset below the surface
//  Given r ← ray(point(0, 0, -5), vector(0, 0, 1))
//    And shape ← glass_sphere() with:
//      | transform | translation(0, 0, 1) |
//    And i ← intersection(5, shape)
//    And xs ← intersections(i)
//  When comps ← prepare_computations(i, r, xs)
//  Then comps.under_point.z > EPSILON/2
//    And comps.point.z < comps.under_point.z
TEST(Intersect, UnderPoint) {
	Ray ray(Point3(0, 0, -5), Vec3(0, 0, 1));
	auto shape = Sphere::glass();
	shape->transform = Matrix4::translate(0, 0, 1);
	IntersectionList xs;
	xs.append(Intersection(5, shape.get()));
	IntersectionInfo info = xs.info(ray, &xs.at(0));
	EXPECT_GT(info.under_point.z, ray::RAY_EPSILON / 2);
	EXPECT_GT(info.under_point.z, info.point.z);
}

//Scenario: Aggregating intersections
//  Given s ← sphere()
//    And i1 ← intersection(1, s)
//    And i2 ← intersection(2, s)
//  When xs ← intersections(i1, i2)
//  Then xs.count = 2
//    And xs[0].t = 1
//    And xs[1].t = 2
//
//Scenario: The hit, when all intersections have positive t
//  Given s ← sphere()
//    And i1 ← intersection(1, s)
//    And i2 ← intersection(2, s)
//    And xs ← intersections(i2, i1)
//  When i ← hit(xs)
//  Then i = i1
TEST(Intersect, HitPositive) {
	Sphere s;
	IntersectionList intersections;
	intersections.append(Intersection(1, &s));
	intersections.append(Intersection(2, &s));
	const Intersection* hit = intersections.hit();
	EXPECT_NEAR(hit->t, 1, RAY_EPSILON);
}

//
//Scenario: The hit, when some intersections have negative t
//  Given s ← sphere()
//    And i1 ← intersection(-1, s)
//    And i2 ← intersection(1, s)
//    And xs ← intersections(i2, i1)
//  When i ← hit(xs)
//  Then i = i2
TEST(Intersect, HitSkipNegative) {
	Sphere s;
	IntersectionList intersections;
	intersections.append(Intersection(-1, &s));
	intersections.append(Intersection(1, &s));
	const Intersection* hit = intersections.hit();
	EXPECT_NEAR(hit->t, 1, RAY_EPSILON);
}

//Scenario: The hit, when all intersections have negative t
//  Given s ← sphere()
//    And i1 ← intersection(-2, s)
//    And i2 ← intersection(-1, s)
//    And xs ← intersections(i2, i1)
//  When i ← hit(xs)
//  Then i is nothing
TEST(Intersect, HitMiss) {
	Sphere s;
	IntersectionList intersections;
	intersections.append(Intersection(-2, &s));
	intersections.append(Intersection(-1, &s));
	const Intersection* hit = intersections.hit();
	EXPECT_EQ(hit, nullptr);
}

//Scenario: The hit is always the lowest nonnegative intersection
//  Given s ← sphere()
//  And i1 ← intersection(5, s)
//  And i2 ← intersection(7, s)
//  And i3 ← intersection(-3, s)
//  And i4 ← intersection(2, s)
//  And xs ← intersections(i1, i2, i3, i4)
//When i ← hit(xs)
//Then i = i4
TEST(Intersect, HitLowest) {
	Sphere s;
	IntersectionList intersections;
	intersections.append(Intersection(5, &s));
	intersections.append(Intersection(7, &s));
	intersections.append(Intersection(-3, &s));
	intersections.append(Intersection(2, &s));
	intersections.sort();
	const Intersection* hit = intersections.hit();
	EXPECT_NEAR(hit->t, 2, RAY_EPSILON);
}

//Scenario Outline: Finding n1 and n2 at various intersections
//  Given A ← glass_sphere() with:
//      | transform                 | scaling(2, 2, 2) |
//      | material.refractive_index | 1.5              |
//    And B ← glass_sphere() with:
//      | transform                 | translation(0, 0, -0.25) |
//      | material.refractive_index | 2.0                      |
//    And C ← glass_sphere() with:
//      | transform                 | translation(0, 0, 0.25) |
//      | material.refractive_index | 2.5                     |
//    And r ← ray(point(0, 0, -4), vector(0, 0, 1))
//    And xs ← intersections(2:A, 2.75:B, 3.25:C, 4.75:B, 5.25:C, 6:A)
//  When comps ← prepare_computations(xs[<index>], r, xs)  
//  Then comps.n1 = <n1>
//    And comps.n2 = <n2>             
//
//  Examples:
//    | index | n1  | n2  |
//    | 0     | 1.0 | 1.5 |                 
//    | 1     | 1.5 | 2.0 |
//    | 2     | 2.0 | 2.5 |
//    | 3     | 2.5 | 2.5 |
//    | 4     | 2.5 | 1.5 |
//    | 5     | 1.5 | 1.0 |
TEST(Intersect, RefractiveIndex) {
	auto a = Sphere::glass();
	a->transform = Matrix4::scale(2, 2, 2);
	a->material.refractive_index = 1.5;
	auto b = Sphere::glass();
	b->transform = Matrix4::translate(0,0,-0.25);
	b->material.refractive_index = 2;
	auto c = Sphere::glass();
	c->transform = Matrix4::translate(0,0,0.25);
	c->material.refractive_index = 2.5;
	Ray r(Point3(0, 0, -4), Vec3(0, 0, 1));

	IntersectionList intersections;
	intersections.append(Intersection(2, a.get()));
	intersections.append(Intersection(2.75, b.get()));
	intersections.append(Intersection(3.25, c.get()));
	intersections.append(Intersection(4.75, b.get()));
	intersections.append(Intersection(5.25, c.get()));
	intersections.append(Intersection(6, a.get()));

	auto i0 = intersections.info(r, &intersections.at(0));
	EXPECT_EQ(i0.n1, 1);
	EXPECT_EQ(i0.n2, 1.5);

	auto i1 = intersections.info(r, &intersections.at(1));
	EXPECT_EQ(i1.n1, 1.5);
	EXPECT_EQ(i1.n2, 2);

	auto i2 = intersections.info(r, &intersections.at(2));
	EXPECT_EQ(i2.n1, 2);
	EXPECT_EQ(i2.n2, 2.5);

	auto i3 = intersections.info(r, &intersections.at(3));
	EXPECT_EQ(i3.n1, 2.5);
	EXPECT_EQ(i3.n2, 2.5);

	auto i4 = intersections.info(r, &intersections.at(4));
	EXPECT_EQ(i4.n1, 2.5);
	EXPECT_EQ(i4.n2, 1.5);

	auto i5 = intersections.info(r, &intersections.at(5));
	EXPECT_EQ(i5.n1, 1.5);
	EXPECT_EQ(i5.n2, 1);
}

//Scenario: The Schlick approximation under total internal reflection
//  Given shape ← glass_sphere()
//    And r ← ray(point(0, 0, √2/2), vector(0, 1, 0))
//    And xs ← intersections(-√2/2:shape, √2/2:shape)
//  When comps ← prepare_computations(xs[1], r, xs)
//    And reflectance ← schlick(comps)
//  Then reflectance = 1.0
TEST(Intersect, SchlickInternalReflect) {
	auto shape = Sphere::glass();
	double ss = sqrt(2);
	Ray r(Point3(0, 0, ss / 2), Vec3(0, 1, 0));

	IntersectionList intersections;
	intersections.append(Intersection(-ss/2, shape.get()));
	intersections.append(Intersection(ss/2, shape.get()));
	IntersectionInfo info = intersections.info(r, &intersections.at(1));
	double reflectance = info.schlick();
	EXPECT_NEAR(reflectance, 1, RAY_EPSILON);
}

//Scenario: The Schlick approximation with a perpendicular viewing angle
//  Given shape ← glass_sphere()
//    And r ← ray(point(0, 0, 0), vector(0, 1, 0))
//    And xs ← intersections(-1:shape, 1:shape)
//  When comps ← prepare_computations(xs[1], r, xs)
//    And reflectance ← schlick(comps)
//  Then reflectance = 0.04
TEST(Intersect, Schlick90) {
	auto shape = Sphere::glass();
	Ray r(Point3(0, 0, 0), Vec3(0, 1, 0));

	IntersectionList intersections;
	intersections.append(Intersection(-1, shape.get()));
	intersections.append(Intersection(1, shape.get()));
	IntersectionInfo info = intersections.info(r, &intersections.at(1));
	double reflectance = info.schlick();
	EXPECT_NEAR(reflectance, 0.042579994960, RAY_EPSILON);
}

//Scenario: The Schlick approximation with small angle and n2 > n1
//  Given shape ← glass_sphere()
//    And r ← ray(point(0, 0.99, -2), vector(0, 0, 1))
//    And xs ← intersections(1.8589:shape)
//  When comps ← prepare_computations(xs[0], r, xs)
//    And reflectance ← schlick(comps)
//  Then reflectance = 0.48873
TEST(Intersect, SchlickSmall) {
	auto shape = Sphere::glass();
	Ray r(Point3(0, 0.99, -2), Vec3(0, 0, 1));

	IntersectionList intersections;
	intersections.append(Intersection(1.8589, shape.get()));
	IntersectionInfo info = intersections.info(r, &intersections.at(0));
	double reflectance = info.schlick();
	EXPECT_NEAR(reflectance, 0.4901048433862, RAY_EPSILON);
}

//Scenario: An intersection can encapsulate `u` and `v`
//  Given s ← triangle(point(0, 1, 0), point(-1, 0, 0), point(1, 0, 0))
//  When i ← intersection_with_uv(3.5, s, 0.2, 0.4)
//  Then i.u = 0.2
//    And i.v = 0.4
TEST(Intersect, CtorUV) {
	auto shape = std::make_unique<Triangle>(Point3(0, 1, 0), Point3(-1, 0, 0), Point3(1, 0, 0));
	Intersection i(3.5, shape.get(), 0.2, 0.4);
	EXPECT_EQ(i.u, 0.2);
	EXPECT_EQ(i.v, 0.4);
}
