#include "pch.h"
#include "Light.h"

using namespace ray;

//Feature: Lights
//
//Scenario: A point light has a position and intensity
//  Given intensity ← color(1, 1, 1)
//    And position ← point(0, 0, 0)
//  When light ← point_light(position, intensity)
//  Then light.position = position
//    And light.intensity = intensity
TEST(Light, Ctor) {
	PointLight light(Point3(0, 0, 0), Color(1, 1, 1));
	EXPECT_EQ(light.position, Point3(0,0,0));
	EXPECT_EQ(light.intensity, Color(1,1,1));
}
