#include "pch.h"

#include "World.h"

using namespace ray;

//Feature: World
//
//Scenario: Creating a world
//  Given w ← world()
//  Then w contains no objects
//    And w has no light source
// Not making this..

//Scenario: The default world
//  Given light ← point_light(point(-10, 10, -10), color(1, 1, 1))
//    And s1 ← sphere() with:
//      | material.color     | (0.8, 1.0, 0.6)        |
//      | material.diffuse   | 0.7                    |
//      | material.specular  | 0.2                    |
//    And s2 ← sphere() with:
//      | transform | scaling(0.5, 0.5, 0.5) |
//  When w ← default_world()
//  Then w.light = light
//    And w contains s1
//    And w contains s2
TEST(World, Ctor) {
	World w;
	EXPECT_EQ(w.light(), PointLight(Point3(-10,10,-10), Color::white()));

	Material m;
	m.pattern = std::make_unique<Solid>(0.8f, 1, 0.6f);
	m.diffuse = 0.7f;
	m.specular = 0.2f;

	Sphere s1;
	s1.material = m;

	const auto& shapes = w.shapes();
	EXPECT_EQ(*shapes[0].get(), s1);

	Sphere s2(Matrix4::scale(0.5f, 0.5f, 0.5f));
	EXPECT_EQ(*shapes[1].get(), s2);
}

//Scenario: Intersect a world with a ray
//  Given w ← default_world()
//    And r ← ray(point(0, 0, -5), vector(0, 0, 1))
//  When xs ← intersect_world(w, r)
//  Then xs.count = 4
//    And xs[0].t = 4
//    And xs[1].t = 4.5
//    And xs[2].t = 5.5
//    And xs[3].t = 6
TEST(World, Intersect) {
	World w;

	std::vector<Intersection> intersections;
	w.intersect(Ray(Point3(0, 0, -5), Vec3(0, 0, 1)), intersections);

	EXPECT_EQ(intersections.size(), 4);
	EXPECT_EQ(intersections[0].t, 4);
	EXPECT_EQ(intersections[1].t, 4.5);
	EXPECT_EQ(intersections[2].t, 5.5);
	EXPECT_EQ(intersections[3].t, 6);
}

//Scenario: Shading an intersection
//  Given w ← default_world()
//    And r ← ray(point(0, 0, -5), vector(0, 0, 1))
//    And shape ← the first object in w
//    And i ← intersection(4, shape)
//  When comps ← prepare_computations(i, r)
//    And c ← shade_hit(w, comps)
//  Then c = color(0.38066, 0.47583, 0.2855)
TEST(World, Shade) {
	World w;
	Ray r(Point3(0, 0, -5), Vec3(0, 0, 1));
	Intersection i(4, w.shapes()[0].get());
	IntersectionInfo info = i.info(r);
	Color c = w.shade_slow(info);

	EXPECT_EQ(c, Color(0.38066f, 0.47583f, 0.2855f)); 
}

//Scenario: Shading an intersection from the inside
//  Given w ← default_world()
//    And w.light ← point_light(point(0, 0.25, 0), color(1, 1, 1))
//    And r ← ray(point(0, 0, 0), vector(0, 0, 1))
//    And shape ← the second object in w
//    And i ← intersection(0.5, shape)
//  When comps ← prepare_computations(i, r)
//    And c ← shade_hit(w, comps)
//  Then c = color(0.90498, 0.90498, 0.90498)
TEST(World, ShadeInside) {
	PointLight light(Point3(0, 0.25f, 0), Color(1, 1, 1));
	World w(light);
	Ray r(Point3(0, 0, 0), Vec3(0, 0, 1));
	Intersection i(0.5, w.shapes()[1].get());
	IntersectionInfo info = i.info(r);
	Color c = w.shade_slow(info);

	EXPECT_EQ(c, Color(0.90498f, 0.90498f, 0.90498f));
}

//Scenario: The color when a ray misses
//  Given w ← default_world()
//    And r ← ray(point(0, 0, -5), vector(0, 1, 0))
//  When c ← color_at(w, r)
//  Then c = color(0, 0, 0)
TEST(World, ColorAtMiss) {
	World w;
	Ray r(Point3(0, 0, -5), Vec3(0, 1, 0));
	Color c = w.color_at_slow(r);

	EXPECT_EQ(c, Color::black());
}

//Scenario: The color when a ray hits
//  Given w ← default_world()
//    And r ← ray(point(0, 0, -5), vector(0, 0, 1))
//  When c ← color_at(w, r)
//  Then c = color(0.38066, 0.47583, 0.2855)
TEST(World, ColorAt) {
	World w;
	Ray r(Point3(0, 0, -5), Vec3(0, 0, 1));
	Color c = w.color_at_slow(r);

	EXPECT_EQ(c, Color(0.38066f, 0.47583f, 0.2855f));
}

//Scenario: The color with an intersection behind the ray
//  Given w ← default_world()
//    And outer ← the first object in w
//    And outer.material.ambient ← 1
//    And inner ← the second object in w
//    And inner.material.ambient ← 1
//    And r ← ray(point(0, 0, 0.75), vector(0, 0, -1))
//  When c ← color_at(w, r)
//  Then c = inner.material.color
TEST(World, ColorAtBehind) {

	std::vector<std::unique_ptr<Shape>> vec = World::make_default_shapes();
	vec[0]->material.ambient = 1;
	vec[1]->material.ambient = 1;
	auto weak_s2 = vec[1].get();

	World w(PointLight(Point3(-10, 10, -10), Color::white()), std::move(vec));

	Ray r(Point3(0, 0, 0.75f), Vec3(0, 0, -1));
	Color c = w.color_at_slow(r);

	EXPECT_EQ(c, weak_s2->material.pattern->pattern_at(Point3()));
}

//Scenario: There is no shadow when nothing is collinear with point and light
//  Given w ← default_world()
//    And p ← point(0, 10, 0)
//   Then is_shadowed(w, p) is false
TEST(World, ShadowColinear) {
	World w;
	Point3 p(0, 10, 0);

	EXPECT_EQ(w.is_shadowed_slow(p), false);
}

//Scenario: The shadow when an object is between the point and the light
//  Given w ← default_world()
//    And p ← point(10, -10, 10)
//   Then is_shadowed(w, p) is true
TEST(World, ShadowBlocked) {
	World w;
	Point3 p(10, -10, 10);

	EXPECT_EQ(w.is_shadowed_slow(p), true);
}

//Scenario: There is no shadow when an object is behind the light
//  Given w ← default_world()
//    And p ← point(-20, 20, -20)
//   Then is_shadowed(w, p) is false
TEST(World, ShadowBehindLight) {
	World w;
	Point3 p(-20, 20, -20);

	EXPECT_EQ(w.is_shadowed_slow(p), false);
}

//Scenario: There is no shadow when an object is behind the point
//  Given w ← default_world()
//    And p ← point(-2, 2, -2)
//   Then is_shadowed(w, p) is false
TEST(World, ShadowBehindPoint) {
	World w;
	Point3 p(-2, 2, -2);

	EXPECT_EQ(w.is_shadowed_slow(p), false); 
}

//Scenario: shade_hit() is given an intersection in shadow
//  Given w ← world()
//    And w.light ← point_light(point(0, 0, -10), color(1, 1, 1))
//    And s1 ← sphere()
//    And s1 is added to w
//    And s2 ← sphere() with:
//      | transform | translation(0, 0, 10) |
//    And s2 is added to w
//    And r ← ray(point(0, 0, 5), vector(0, 0, 1))
//    And i ← intersection(4, s2)
//  When comps ← prepare_computations(i, r)
//    And c ← shade_hit(w, comps)
//  Then c = color(0.1, 0.1, 0.1)
TEST(World, ShadowWorld) {
	std::vector<std::unique_ptr<Shape>> vec = World::make_default_shapes();

	vec[1]->transform = Matrix4::translate(0, 0, 10);
	Shape* weak_s2 = vec[1].get();

	PointLight light(Point3(0, 0, -10), Color(1, 1, 1));
	World w(light, std::move(vec));

	Ray r(Point3(0, 0, 5), Vec3(0, 0, 1));
	Intersection i(4, weak_s2);
	IntersectionInfo info = i.info(r);
	Color c = w.shade_slow(info);

	EXPECT_EQ(c, Color(0.1f, 0.1f, 0.1f));
}

//Scenario: The reflected color for a nonreflective material
//  Given w ← default_world()
//    And r ← ray(point(0, 0, 0), vector(0, 0, 1))
//    And shape ← the second object in w
//    And shape.material.ambient ← 1
//    And i ← intersection(1, shape)
//  When comps ← prepare_computations(i, r)
//    And color ← reflected_color(w, comps)
//  Then color = color(0, 0, 0)
TEST(World, ReflectNonReflective) {
	std::vector<std::unique_ptr<Shape>> vec = World::make_default_shapes();

	vec[1]->material.ambient = 1;
	vec[1]->transform = Matrix4::translate(0, 0, 10);
	Shape* weak_s2 = vec[1].get();

	PointLight light(Point3(0, 0, -10), Color(1, 1, 1));
	World w(light, std::move(vec));

	Ray r(Point3(0, 0, 0), Vec3(0, 0, 1));
	Intersection i(1, weak_s2);
	IntersectionInfo info = i.info(r);
	Color c = w.reflected_color_slow(info,1);

	EXPECT_EQ(c, Color::black());
}

//Scenario: The reflected color for a reflective material
//  Given w ← default_world()
//    And shape ← plane() with:                 
//      | material.reflective | 0.5                   |
//      | transform           | translation(0, -1, 0) |   
//    And shape is added to w
//    And r ← ray(point(0, 0, -3), vector(0, -√2/2, √2/2))
//    And i ← intersection(√2, shape)
//  When comps ← prepare_computations(i, r)
//    And color ← reflected_color(w, comps)
//  Then color = color(0.19032, 0.2379, 0.14274)

/* 
* per jamis: https://forum.raytracerchallenge.com/thread/82/stuck-test-strike-reflective-surface
reflect - ray origin = p(0.0000, -0.9999, -2.0000)
reflect - ray direction = v(0.0000, 0.7071, 0.7071)
col = v(0.3807, 0.4759, 0.2855)
result of multiply - color = v(0.1903, 0.2379, 0.1428)
*/
TEST(World, ReflectReflective) {
	std::vector<std::unique_ptr<Shape>> vec = World::make_default_shapes();

	auto plane = std::make_unique<Plane>();
	Plane* plane_weak = plane.get();
	plane->material.reflective = 0.5;
	plane->transform = Matrix4::translate(0, -1, 0);
	vec.push_back(std::move(plane));

	PointLight light(Point3(-10, 10, -10), Color(1, 1, 1));
	World w(light, std::move(vec));

	double ss = sqrt(2);
	Ray r(Point3(0, 0, -3), Vec3(0, -ss/2, ss/2));
	Intersection i(ss, plane_weak);
	IntersectionInfo info = i.info(r);
	Color c = w.reflected_color_slow(info,1);

	EXPECT_EQ(c, Color(0.190332, 0.237915, 0.142749));
}

//Scenario: shade_hit() with a reflective material
//  Given w ← default_world()
//    And shape ← plane() with:
//      | material.reflective | 0.5                   |
//      | transform           | translation(0, -1, 0) |
//    And shape is added to w
//    And r ← ray(point(0, 0, -3), vector(0, -√2/2, √2/2))
//    And i ← intersection(√2, shape)
//  When comps ← prepare_computations(i, r)
//    And color ← shade_hit(w, comps)
//  Then color = color(0.87677, 0.92436, 0.82918)
TEST(World, ReflectShadeHit) {
	std::vector<std::unique_ptr<Shape>> vec = World::make_default_shapes();

	auto plane = std::make_unique<Plane>();
	Plane* plane_weak = plane.get();
	plane->material.reflective = 0.5;
	plane->transform = Matrix4::translate(0, -1, 0);
	vec.push_back(std::move(plane));

	PointLight light(Point3(-10, 10, -10), Color(1, 1, 1));
	World w(light, std::move(vec));

	double ss = sqrt(2);
	Ray r(Point3(0, 0, -3), Vec3(0, -ss / 2, ss / 2));
	Intersection i(ss, plane_weak);
	IntersectionInfo info = i.info(r);
	Color c = w.shade_slow(info);

	EXPECT_EQ(c, Color(0.876757, 0.92434, 0.829174));
}

//Scenario: color_at() with mutually reflective surfaces
//  Given w ← world()
//    And w.light ← point_light(point(0, 0, 0), color(1, 1, 1))
//    And lower ← plane() with:
//      | material.reflective | 1                     |
//      | transform           | translation(0, -1, 0) |
//    And lower is added to w
//    And upper ← plane() with:
//      | material.reflective | 1                    |
//      | transform           | translation(0, 1, 0) |
//    And upper is added to w
//    And r ← ray(point(0, 0, 0), vector(0, 1, 0))
//  Then color_at(w, r) should terminate successfully
TEST(World, ReflectParallel) {
	std::vector<std::unique_ptr<Shape>> vec;

	{
		auto lower_plane = std::make_unique<Plane>();
		lower_plane->material.reflective = 1;
		lower_plane->transform = Matrix4::translate(0, -1, 0);
		vec.push_back(std::move(lower_plane));
	}

	{
		auto upper_plane = std::make_unique<Plane>();
		upper_plane->material.reflective = 1;
		upper_plane->transform = Matrix4::translate(0, 1, 0);
		vec.push_back(std::move(upper_plane));
	}

	PointLight light(Point3(0, 0, 0), Color(1, 1, 1));
	World w(light, std::move(vec));

	Ray r(Point3(0, 0, 0), Vec3(0, 1, 0));
	Color c = w.color_at_slow(r);

	EXPECT_EQ(c, Color(11.4, 11.4, 11.4));
}

//Scenario: The reflected color at the maximum recursive depth
//  Given w ← default_world()
//    And shape ← plane() with:
//      | material.reflective | 0.5                   |
//      | transform           | translation(0, -1, 0) |
//    And shape is added to w
//    And r ← ray(point(0, 0, -3), vector(0, -√2/2, √2/2))
//    And i ← intersection(√2, shape)
//  When comps ← prepare_computations(i, r)
//    And color ← reflected_color(w, comps, 0)    
//  Then color = color(0, 0, 0)
TEST(World, ReflectMaxRecurse) {
	std::vector<std::unique_ptr<Shape>> vec = World::make_default_shapes();

	auto plane = std::make_unique<Plane>();
	Plane* plane_weak = plane.get();
	plane->material.reflective = 0.5;
	plane->transform = Matrix4::translate(0, -1, 0);
	vec.push_back(std::move(plane));

	PointLight light(Point3(-10, 10, -10), Color(1, 1, 1));
	World w(light, std::move(vec));

	double ss = sqrt(2);
	Ray r(Point3(0, 0, -3), Vec3(0, -ss / 2, ss / 2));
	Intersection i(ss, plane_weak);
	IntersectionInfo info = i.info(r);
	Color c = w.reflected_color_slow(info, 0);

	EXPECT_EQ(c, Color::black());
}

//Scenario: The refracted color with an opaque surface
//  Given w ← default_world()
//    And shape ← the first object in w
//    And r ← ray(point(0, 0, -5), vector(0, 0, 1))
//    And xs ← intersections(4:shape, 6:shape)
//  When comps ← prepare_computations(xs[0], r, xs)
//    And c ← refracted_color(w, comps, 5)
//  Then c = color(0, 0, 0)
//
//Scenario: The refracted color at the maximum recursive depth
//  Given w ← default_world()
//    And shape ← the first object in w
//    And shape has:
//      | material.transparency     | 1.0 |
//      | material.refractive_index | 1.5 |
//    And r ← ray(point(0, 0, -5), vector(0, 0, 1))
//    And xs ← intersections(4:shape, 6:shape)
//  When comps ← prepare_computations(xs[0], r, xs)
//    And c ← refracted_color(w, comps, 0)
//  Then c = color(0, 0, 0)
//
//Scenario: The refracted color under total internal reflection
//  Given w ← default_world()
//    And shape ← the first object in w
//    And shape has:
//      | material.transparency     | 1.0 |
//      | material.refractive_index | 1.5 |
//    And r ← ray(point(0, 0, √2/2), vector(0, 1, 0))
//    And xs ← intersections(-√2/2:shape, √2/2:shape)
//  # NOTE: this time you're inside the sphere, so you need
//  # to look at the second intersection, xs[1], not xs[0]
//  When comps ← prepare_computations(xs[1], r, xs)
//    And c ← refracted_color(w, comps, 5)
//  Then c = color(0, 0, 0)
//
//Scenario: The refracted color with a refracted ray
//  Given w ← default_world()
//    And A ← the first object in w
//    And A has:
//      | material.ambient | 1.0            |
//      | material.pattern | test_pattern() |
//    And B ← the second object in w
//    And B has:
//      | material.transparency     | 1.0 |
//      | material.refractive_index | 1.5 |
//    And r ← ray(point(0, 0, 0.1), vector(0, 1, 0))
//    And xs ← intersections(-0.9899:A, -0.4899:B, 0.4899:B, 0.9899:A)
//  When comps ← prepare_computations(xs[2], r, xs)
//    And c ← refracted_color(w, comps, 5)
//  Then c = color(0, 0.99888, 0.04725)
//
//Scenario: shade_hit() with a transparent material
//  Given w ← default_world()
//    And floor ← plane() with:
//      | transform                 | translation(0, -1, 0) |
//      | material.transparency     | 0.5                   |
//      | material.refractive_index | 1.5                   |
//    And floor is added to w
//    And ball ← sphere() with:
//      | material.color     | (1, 0, 0)                  |
//      | material.ambient   | 0.5                        |
//      | transform          | translation(0, -3.5, -0.5) |
//    And ball is added to w
//    And r ← ray(point(0, 0, -3), vector(0, -√2/2, √2/2))
//    And xs ← intersections(√2:floor)
//  When comps ← prepare_computations(xs[0], r, xs)
//    And color ← shade_hit(w, comps, 5)
//  Then color = color(0.93642, 0.68642, 0.68642)
//
//Scenario: shade_hit() with a reflective, transparent material
//  Given w ← default_world()
//    And r ← ray(point(0, 0, -3), vector(0, -√2/2, √2/2))
//    And floor ← plane() with:
//      | transform                 | translation(0, -1, 0) |
//      | material.reflective       | 0.5                   |
//      | material.transparency     | 0.5                   |
//      | material.refractive_index | 1.5                   |
//    And floor is added to w
//    And ball ← sphere() with:
//      | material.color     | (1, 0, 0)                  |
//      | material.ambient   | 0.5                        |
//      | transform          | translation(0, -3.5, -0.5) |
//    And ball is added to w
//    And xs ← intersections(√2:floor)
//  When comps ← prepare_computations(xs[0], r, xs)
//    And color ← shade_hit(w, comps, 5)
//  Then color = color(0.93391, 0.69643, 0.69243)
