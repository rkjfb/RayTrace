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
TEST(Wavefront, NamedGroups) {
	std::ifstream file("triangles.obj");
	Wavefront w(file);

	Point3 p1(-1, 1, 0);
	Point3 p2(-1, 0, 0);
	Point3 p3(1, 0, 0);
	Point3 p4(1, 1, 0);

	const auto& g1 = w.named_group("FirstGroup");
	const auto& s1 = g1->shapes();
	EXPECT_EQ(*s1[0], Triangle(p1, p2, p3));

	const auto& g2 = w.named_group("SecondGroup");
	const auto& s2 = g2->shapes();
	EXPECT_EQ(*s2[0], Triangle(p1, p3, p4));
}

//Scenario: Converting an OBJ file to a group
//  Given file ← the file "triangles.obj"
//    And parser ← parse_obj_file(file)
//  When g ← obj_to_group(parser)
//  Then g includes "FirstGroup" from parser
//    And g includes "SecondGroup" from parser
TEST(Wavefront, MegaGroup) {
	std::ifstream file("triangles.obj");
	Wavefront w(file);

	const auto& g1 = w.named_group("FirstGroup");
	const Group* weak_g1 = g1.get();
	const auto& g2 = w.named_group("SecondGroup");
	const Group* weak_g2 = g2.get();

	auto mega = w.get_mega_group();
	EXPECT_EQ(mega->size(), 2);
	/*
	* todo: groups are re-allocated as part of spatialize'ing during mega group building.
	* 
	const auto& s = mega->shapes();
	// group order is not garanteed, so check both.
	bool found_first = s[0].get() == weak_g1 || s[1].get() == weak_g1;
	EXPECT_TRUE(found_first);
	bool found_second = s[0].get() == weak_g2 || s[1].get() == weak_g2;
	EXPECT_TRUE(found_second);
	*/
}

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
TEST(Wavefront, VertexNormal) {
	Wavefront w(
		R"literal(
		vn 0 0 1
		vn 0.707 0 -0.707
		vn 1 2 3
		)literal");

	EXPECT_EQ(w.normals[1], Vec3(0, 0, 1));
	EXPECT_EQ(w.normals[2], Vec3(0.707, 0, -0.707));
	EXPECT_EQ(w.normals[3], Vec3(1, 2, 3));
}

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
TEST(Wavefront, FaceNormal) {
	Wavefront w(
		R"literal(
		v 0 1 0
		v -1 0 0
		v 1 0 0

		vn -1 0 0
		vn 1 0 0
		vn 0 1 0

		f 1//3 2//1 3//2
		f 1/0/3 2/102/1 3/14/2
		f -2/0/0 -1/102/-1 0/14/-1
		)literal");

	auto& group = w.default_group();
	auto& shapes = group->shapes();

	Point3 p1(0, 1, 0);
	Point3 p2(-1, 0, 0);
	Point3 p3(1, 0, 0);
	Vec3 n1(0, 1, 0);
	Vec3 n2(-1, 0, 0);
	Vec3 n3(1, 0, 0);

	SmoothTriangle tri(p1, p2, p3, n1, n2, n3);

	auto& t1 = shapes[0];
	auto& t2 = shapes[1];
	auto& t3 = shapes[2];

	EXPECT_EQ(*t1, tri);
	EXPECT_EQ(*t1, *t2);
	EXPECT_EQ(*t1, *t3);
}
