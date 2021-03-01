#include "pch.h"

#include "Camera.h"
#include "World.h"
#include "Canvas.h"

using namespace ray;

//Feature: Camera
//
//Scenario: Constructing a camera
//  Given hsize ← 160
//    And vsize ← 120
//    And field_of_view ← π/2
//  When c ← camera(hsize, vsize, field_of_view)
//  Then c.hsize = 160
//    And c.vsize = 120
//    And c.field_of_view = π/2
//    And c.transform = identity_matrix
TEST(Camera, Ctor) {
	Camera c(160, 120, pi / 2);
	EXPECT_EQ(c.hsize, 160);
	EXPECT_EQ(c.vsize, 120);
	EXPECT_EQ(c.fov, pi / 2);
	EXPECT_EQ(c.transform, Matrix4::identity());
}

//Scenario: The pixel size for a horizontal canvas
//  Given c ← camera(200, 125, π/2)
//  Then c.pixel_size = 0.01
TEST(Camera, HorizontalCanvas) {
	Camera c(200,125, pi / 2);
	EXPECT_EQ(c.pixel_size, 0.01f);
}

//Scenario: The pixel size for a vertical canvas
//  Given c ← camera(125, 200, π/2)
//  Then c.pixel_size = 0.01
TEST(Camera, VerticalCanvas) {
	Camera c(125, 200, pi / 2);
	EXPECT_EQ(c.pixel_size, 0.01f); 
}

//Scenario: Constructing a ray through the center of the canvas
//  Given c ← camera(201, 101, π/2)
//  When r ← ray_for_pixel(c, 100, 50)
//  Then r.origin = point(0, 0, 0)
//    And r.direction = vector(0, 0, -1)
TEST(Camera, RayPixel) {
	Camera c(201,101, pi / 2);
	Ray r = c.ray(100, 50);
	EXPECT_EQ(r, Ray(Point3(0,0,0), Vec3(0,0,-1)));
}

//Scenario: Constructing a ray through a corner of the canvas
//  Given c ← camera(201, 101, π/2)
//  When r ← ray_for_pixel(c, 0, 0)
//  Then r.origin = point(0, 0, 0)
//    And r.direction = vector(0.66519, 0.33259, -0.66851)
TEST(Camera, RayCorner) {
	Camera c(201, 101, pi / 2);
	Ray r = c.ray(0, 0);
	EXPECT_EQ(r, Ray(Point3(0, 0, 0), Vec3(0.66519f, 0.33259f, -0.66851f)));
}

//Scenario: Constructing a ray when the camera is transformed
//  Given c ← camera(201, 101, π/2)
//  When c.transform ← rotation_y(π/4) * translation(0, -2, 5)
//    And r ← ray_for_pixel(c, 100, 50)
//  Then r.origin = point(0, 2, -5)
//    And r.direction = vector(√2/2, 0, -√2/2)
TEST(Camera, RayTransformed) {
	Camera c(201, 101, pi / 2);
	c.transform = Matrix4::rotateY(pi / 4) * Matrix4::translate(0, -2, 5);
	Ray r = c.ray(100, 50);
	float ss = sqrtf(2) / 2;
	EXPECT_EQ(r, Ray(Point3(0, 2, -5), Vec3(ss, 0, -ss)));
}

//Scenario: Rendering a world with a camera
//  Given w ← default_world()
//    And c ← camera(11, 11, π/2)
//    And from ← point(0, 0, -5)
//    And to ← point(0, 0, 0)
//    And up ← vector(0, 1, 0)
//    And c.transform ← view_transform(from, to, up)
//  When image ← render(c, w)
//  Then pixel_at(image, 5, 5) = color(0.38066, 0.47583, 0.2855)
TEST(Camera, Render) {
	World w;
	Camera c(11, 11, pi / 2);
	Point3 from(0, 0, -5);
	Point3 to(0, 0, 0);
	Vec3 up(0, 1, 0);
	c.transform = Matrix4::view(from, to, up);
	Canvas canvas = c.render(w);
	auto& pixels = canvas.pixels();
	EXPECT_EQ(pixels[5][5], Color(0.38066f, 0.47583f, 0.2855f));
}
