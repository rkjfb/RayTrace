#include "pch.h"

#include "Wavefront.h"

using namespace ray;


//Feature: OBJ File Parser
//
//Scenario: Ignoring unrecognized lines
//  Given gibberish ← a file containing:
//    """
//    There was a young lady named Bright
//    who traveled much faster than light.
//    She set out one day
//    in a relative way,
//    and came back the previous night.
//    """
//  When parser ← parse_obj_file(gibberish)
//  Then parser should have ignored 5 lines
TEST(Wavefront, Garbage) {
	Wavefront w(
R"literal(There was a young lady named Bright
  who traveled much faster than light.
She set out one day
in a relative way,
and came back the previous night.)literal");
	EXPECT_EQ(w.ignored, 5);
}

//Scenario: Vertex records
//  Given file ← a file containing:
//    """
//    v -1 1 0
//    v -1.0000 0.5000 0.0000
//    v 1 0 0
//    v 1 1 0
//    """
//  When parser ← parse_obj_file(file)
//  Then parser.vertices[1] = point(-1, 1, 0)
//    And parser.vertices[2] = point(-1, 0.5, 0)
//    And parser.vertices[3] = point(1, 0, 0)
//    And parser.vertices[4] = point(1, 1, 0)
TEST(Wavefront, Vertex) {
	Wavefront w(
		R"literal(v -1 1 0
		v -1.0000 0.5000 0.0000
		v 1 0 0
		v 1 1 0)literal");
	EXPECT_EQ(w.vertices[1], Point3(-1, 1, 0));
	EXPECT_EQ(w.vertices[2], Point3(-1, 0.5, 0));
	EXPECT_EQ(w.vertices[3], Point3(1, 0, 0));
	EXPECT_EQ(w.vertices[4], Point3(1, 1, 0));
}

//Scenario: Parsing triangle faces
//  Given file ← a file containing:
//    """
//    v -1 1 0
//    v -1 0 0
//    v 1 0 0
//    v 1 1 0
//
//    f 1 2 3
//    f 1 3 4
//    """
//  When parser ← parse_obj_file(file)
//    And g ← parser.default_group
//    And t1 ← first child of g
//    And t2 ← second child of g
//  Then t1.p1 = parser.vertices[1]
//    And t1.p2 = parser.vertices[2]
//    And t1.p3 = parser.vertices[3]
//    And t2.p1 = parser.vertices[1]
//    And t2.p2 = parser.vertices[3]
//    And t2.p3 = parser.vertices[4]
TEST(Wavefront, Triangles) {
	Wavefront w(
		R"literal(
			v -1 1 0
			v -1 0 0
			v 1 0 0
			v 1 1 0
			f 1 2 3
			f 1 3 4
		)literal");

	Point3 p1(-1, 1, 0);
	Point3 p2(-1, 0, 0);
	Point3 p3(1, 0, 0);
	Point3 p4(1, 1, 0);

	const auto& g = w.default_group();
	const auto& shapes = g->shapes();

	EXPECT_EQ(*shapes[0], Triangle(p1, p2, p3));
	EXPECT_EQ(*shapes[1], Triangle(p1, p3, p4));
}

//Scenario: Triangulating polygons
//  Given file ← a file containing:
//    """
//    v -1 1 0
//    v -1 0 0
//    v 1 0 0
//    v 1 1 0
//    v 0 2 0
//
//    f 1 2 3 4 5
//    """
//  When parser ← parse_obj_file(file)
//    And g ← parser.default_group
//    And t1 ← first child of g
//    And t2 ← second child of g
//    And t3 ← third child of g
//  Then t1.p1 = parser.vertices[1]
//    And t1.p2 = parser.vertices[2]
//    And t1.p3 = parser.vertices[3]
//    And t2.p1 = parser.vertices[1]
//    And t2.p2 = parser.vertices[3]
//    And t2.p3 = parser.vertices[4]
//    And t3.p1 = parser.vertices[1]
//    And t3.p2 = parser.vertices[4]
//    And t3.p3 = parser.vertices[5]
TEST(Wavefront, Polygon) {
	Wavefront w(
		R"literal(
			v -1 1 0
			v -1 0 0
			v 1 0 0
			v 1 1 0
			v 0 2 0
			f 1 2 3 4 5
		)literal");

	Point3 p1(-1, 1, 0);
	Point3 p2(-1, 0, 0);
	Point3 p3(1, 0, 0);
	Point3 p4(1, 1, 0);
	Point3 p5(0, 2, 0);

	const auto& g = w.default_group();
	const auto& shapes = g->shapes();

	EXPECT_EQ(*shapes[0], Triangle(p1, p2, p3));
	EXPECT_EQ(*shapes[1], Triangle(p1, p3, p4));
	EXPECT_EQ(*shapes[2], Triangle(p1, p4, p5));
}

//Scenario: Triangles in groups
//  Given file ← the file "triangles.obj"
//  When parser ← parse_obj_file(file)
//    And g1 ← "FirstGroup" from parser
//    And g2 ← "SecondGroup" from parser
//    And t1 ← first child of g1
//    And t2 ← first child of g2
//  Then t1.p1 = parser.vertices[1]
//    And t1.p2 = parser.vertices[2]
//    And t1.p3 = parser.vertices[3]
//    And t2.p1 = parser.vertices[1]
//    And t2.p2 = parser.vertices[3]
//    And t2.p3 = parser.vertices[4]
//
//Scenario: Converting an OBJ file to a group
//  Given file ← the file "triangles.obj"
//    And parser ← parse_obj_file(file)
//  When g ← obj_to_group(parser)
//  Then g includes "FirstGroup" from parser
//    And g includes "SecondGroup" from parser
//
//Scenario: Vertex normal records
//  Given file ← a file containing:
//    """
//    vn 0 0 1
//    vn 0.707 0 -0.707
//    vn 1 2 3
//    """
//  When parser ← parse_obj_file(file)
//  Then parser.normals[1] = vector(0, 0, 1)
//    And parser.normals[2] = vector(0.707, 0, -0.707)
//    And parser.normals[3] = vector(1, 2, 3)
//
//Scenario: Faces with normals
//  Given file ← a file containing:
//    """
//    v 0 1 0
//    v -1 0 0
//    v 1 0 0
//
//    vn -1 0 0
//    vn 1 0 0
//    vn 0 1 0
//
//    f 1//3 2//1 3//2
//    f 1/0/3 2/102/1 3/14/2
//    """
//  When parser ← parse_obj_file(file)
//    And g ← parser.default_group
//    And t1 ← first child of g
//    And t2 ← second child of g
//  Then t1.p1 = parser.vertices[1]
//    And t1.p2 = parser.vertices[2]
//    And t1.p3 = parser.vertices[3]
//    And t1.n1 = parser.normals[3]
//    And t1.n2 = parser.normals[1]
//    And t1.n3 = parser.normals[2]
//    And t2 = t1
