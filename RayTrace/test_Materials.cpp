#include "pch.h"
#include "Material.h"
#include "Shape.h"

using namespace ray;

//Feature: Materials
//
//Background:
//  Given m ← material()
//    And position ← point(0, 0, 0)
//
//Scenario: The default material
//  Given m ← material()
//  Then m.color = color(1, 1, 1)
//    And m.ambient = 0.1
//    And m.diffuse = 0.9
//    And m.specular = 0.9
//    And m.shininess = 200.0
TEST(Material, Ctor) {
	Material mat;
	EXPECT_EQ(mat.pattern->pattern_at(Point3()), Color::white());
	EXPECT_EQ(mat.ambient, 0.1);
	EXPECT_EQ(mat.diffuse, 0.9);
	EXPECT_EQ(mat.specular, 0.9);
	EXPECT_EQ(mat.shininess, 200);
}

//Scenario: Reflectivity for the default material
//  Given m ← material()
//  Then m.reflective = 0.0
TEST(Material, CtorReflective) {
	Material mat;
	EXPECT_EQ(mat.reflective, 0);
}

//Scenario: Transparency and Refractive Index for the default material
//  Given m ← material()
//  Then m.transparency = 0.0
//    And m.refractive_index = 1.0
TEST(Material, CtorTransparent) {
	Material mat;
	EXPECT_EQ(mat.transparency, 0);
	EXPECT_EQ(mat.refractive_index, 1);
}

//Scenario: Lighting with the eye between the light and the surface
//  Given eyev ← vector(0, 0, -1)
//    And normalv ← vector(0, 0, -1)
//    And light ← point_light(point(0, 0, -10), color(1, 1, 1))
//  When result ← lighting(m, light, position, eyev, normalv)
//  Then result = color(1.9, 1.9, 1.9)
TEST(Material, Lighting) {
	Material mat;
	Point3 position;
	Vec3 eyev(0, 0, -1);
	Vec3 normalv(0, 0, -1);
	PointLight light(Point3(0, 0, -10), Color(1, 1, 1));
	Sphere sphere;
	Color result = mat.lighting(light, sphere, position, eyev, normalv, false);
	EXPECT_EQ(result, Color(1.9f, 1.9f, 1.9f));
}

//Scenario: Lighting with the eye between light and surface, eye offset 45°
//  Given eyev ← vector(0, √2/2, -√2/2)
//    And normalv ← vector(0, 0, -1)
//    And light ← point_light(point(0, 0, -10), color(1, 1, 1))
//  When result ← lighting(m, light, position, eyev, normalv)
//  Then result = color(1.0, 1.0, 1.0)
TEST(Material, Lighting45deg) {
	Material mat;
	Point3 position;
	float st = sqrtf(2) / 2;
	Vec3 eyev(0, st, -st);
	Vec3 normalv(0, 0, -1);
	PointLight light(Point3(0, 0, -10), Color(1, 1, 1));
	Sphere sphere;
	Color result = mat.lighting(light, sphere, position, eyev, normalv, false);
	EXPECT_EQ(result, Color(1, 1, 1));
}

//Scenario: Lighting with eye opposite surface, light offset 45°
//  Given eyev ← vector(0, 0, -1)
//    And normalv ← vector(0, 0, -1)
//    And light ← point_light(point(0, 10, -10), color(1, 1, 1))
//  When result ← lighting(m, light, position, eyev, normalv)
//  Then result = color(0.7364, 0.7364, 0.7364)
TEST(Material, Lighting45degEyeBack) {
	Material mat;
	Point3 position;
	Vec3 eyev(0, 0, -1);
	Vec3 normalv(0, 0, -1);
	PointLight light(Point3(0, 10, -10), Color(1, 1, 1));
	Sphere sphere;
	Color result = mat.lighting(light, sphere, position, eyev, normalv, false);
	EXPECT_EQ(result, Color(0.7364f, 0.7364f, 0.7364f));
}

//Scenario: Lighting with eye in the path of the reflection vector
//  Given eyev ← vector(0, -√2/2, -√2/2)
//    And normalv ← vector(0, 0, -1)
//    And light ← point_light(point(0, 10, -10), color(1, 1, 1))
//  When result ← lighting(m, light, position, eyev, normalv)
//  Then result = color(1.6364, 1.6364, 1.6364)
TEST(Material, LightingReflectPath) {
	Material mat;
	Point3 position;
	float st = sqrtf(2) / 2;
	Vec3 eyev(0, -st, -st);
	Vec3 normalv(0, 0, -1);
	PointLight light(Point3(0, 10, -10), Color(1, 1, 1));
	Sphere sphere;
	Color result = mat.lighting(light, sphere, position, eyev, normalv, false);
	EXPECT_EQ(result, Color(1.6364f, 1.6364f, 1.6364f));
}

//Scenario: Lighting with the light behind the surface
//  Given eyev ← vector(0, 0, -1)
//    And normalv ← vector(0, 0, -1)
//    And light ← point_light(point(0, 0, 10), color(1, 1, 1))
//  When result ← lighting(m, light, position, eyev, normalv)
//  Then result = color(0.1, 0.1, 0.1)
TEST(Material, LightingEyeSurfLight) {
	Material mat;
	Point3 position;
	Vec3 eyev(0, 0, -1);
	Vec3 normalv(0, 0, -1);
	PointLight light(Point3(0, 0, 10), Color(1, 1, 1)); 
	Sphere sphere;
	Color result = mat.lighting(light, sphere, position, eyev, normalv, false);
	EXPECT_EQ(result, Color(0.1f, 0.1f, 0.1f));
}

//Scenario: Lighting with the surface in shadow
//  Given eyev ← vector(0, 0, -1)
//    And normalv ← vector(0, 0, -1)
//    And light ← point_light(point(0, 0, -10), color(1, 1, 1))
//    And in_shadow ← true
//  When result ← lighting(m, light, position, eyev, normalv, in_shadow)
//  Then result = color(0.1, 0.1, 0.1)
TEST(Material, InShadow) {
	Material mat;
	Point3 position;
	Vec3 eye(0, 0, -1);
	Vec3 normal(0, 0, -1);
	PointLight light(Point3(0, 0, -10), Color(1, 1, 1));
	bool in_shadow = true;
	Sphere sphere;
	Color result = mat.lighting(light, sphere, position, eye, normal, in_shadow);
	EXPECT_EQ(result, Color(0.1f, 0.1f, 0.1f));
}

//Scenario: Lighting with a pattern applied
//  Given m.pattern ← stripe_pattern(color(1, 1, 1), color(0, 0, 0))
//    And m.ambient ← 1
//    And m.diffuse ← 0
//    And m.specular ← 0
//    And eyev ← vector(0, 0, -1)
//    And normalv ← vector(0, 0, -1)
//    And light ← point_light(point(0, 0, -10), color(1, 1, 1))
//  When c1 ← lighting(m, light, point(0.9, 0, 0), eyev, normalv, false)
//    And c2 ← lighting(m, light, point(1.1, 0, 0), eyev, normalv, false)
//  Then c1 = color(1, 1, 1)
//    And c2 = color(0, 0, 0)
TEST(Material, Pattern) {
	Material mat;
	mat.pattern = std::make_unique<Stripe>(Color::white(), Color::black());
	mat.ambient = 1;
	mat.diffuse = 0;
	mat.specular = 0;
	Vec3 eye(0, 0, -1);
	Vec3 normal(0, 0, -1);
	PointLight light(Point3(0, 0, -10), Color::white());
	Sphere sphere;

	Color c1 = mat.lighting(light, sphere, Point3(0.9, 0, 0), eye, normal, false);
	EXPECT_EQ(c1, Color::white());

	Color c2 = mat.lighting(light, sphere, Point3(1.1, 0, 0), eye, normal, false);
	EXPECT_EQ(c2, Color::black());
}
