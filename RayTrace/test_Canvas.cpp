#include "pch.h"

#include "Canvas.h"

using namespace ray;

//Feature: Canvas
//
//Scenario: Creating a canvas
//  Given c ← canvas(10, 20)
//  Then c.width = 10
//    And c.height = 20
//    And every pixel of c is color(0, 0, 0)
TEST(Canvas, Dimension) {
	Canvas c(10, 20);
	EXPECT_EQ(c.width(), 10);
	EXPECT_EQ(c.height(), 20);
}

//Scenario: Writing pixels to a canvas
//  Given c ← canvas(10, 20)
//    And red ← color(1, 0, 0)
//  When write_pixel(c, 2, 3, red)
//  Then pixel_at(c, 2, 3) = red
TEST(Canvas, WritePixel) {
	Canvas c(10, 20);
	Color red(1, 0, 0);
	auto pixels = c.pixels();
	pixels[2][15] = red;
	EXPECT_EQ(pixels[2][15], red);
}

//Scenario: Constructing the PPM header
//  Given c ← canvas(5, 3)
//  When ppm ← canvas_to_ppm(c)
//  Then lines 1-3 of ppm are
//    """
//    P3
//    5 3
//    255
//    """
TEST(Canvas, PpmHeader) {
	Canvas c(5,3);
	std::ostringstream out;
	c.write_ppm(out);
	std::istringstream in(out.str());
	std::string s;
	std::getline(in, s);
	EXPECT_EQ("P3", s);
	std::getline(in, s);
	EXPECT_EQ("5 3", s);
	std::getline(in, s);
	EXPECT_EQ("255", s);
}

//Scenario: Constructing the PPM pixel data
//  Given c ← canvas(5, 3)
//    And c1 ← color(1.5, 0, 0)
//    And c2 ← color(0, 0.5, 0)
//    And c3 ← color(-0.5, 0, 1)
//  When write_pixel(c, 0, 0, c1)
//    And write_pixel(c, 2, 1, c2)
//    And write_pixel(c, 4, 2, c3)
//    And ppm ← canvas_to_ppm(c)
//  Then lines 4-6 of ppm are
//    """
//    255 0 0 0 0 0 0 0 0 0 0 0 0 0 0
//    0 0 0 0 0 0 0 128 0 0 0 0 0 0 0
//    0 0 0 0 0 0 0 0 0 0 0 0 0 0 255
//    """
//
//Scenario: Splitting long lines in PPM files
//  Given c ← canvas(10, 2)
//  When every pixel of c is set to color(1, 0.8, 0.6)
//    And ppm ← canvas_to_ppm(c)
//  Then lines 4-7 of ppm are
//    """
//    255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204
//    153 255 204 153 255 204 153 255 204 153 255 204 153
//    255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204
//    153 255 204 153 255 204 153 255 204 153 255 204 153
//    """
//
//Scenario: PPM files are terminated by a newline character
//  Given c ← canvas(5, 3)
//  When ppm ← canvas_to_ppm(c)
//  Then ppm ends with a newline character
