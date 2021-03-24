#include "pch.h"
#include "Intersect.h"

using namespace ray;


//Feature: Groups
//
//Scenario: Creating a new group
//  Given g ← group()
//  Then g.transform = identity_matrix
//    And g is empty
TEST(Group, Ctor) {
	Group g;
	EXPECT_EQ(g.transform, Matrix4::identity());
	EXPECT_EQ(g.size(), 0);
}

//Scenario: Adding a child to a group
//  Given g ← group()
//    And s ← test_shape()
//  When add_child(g, s)
//  Then g is not empty
//    And g includes s
//    And s.parent = g
TEST(Group, AddChild) {
	Group g;
	auto s = std::make_unique<TestShape>();
	Shape* weak_s = s.get();
	g.add(std::move(s));
	EXPECT_NE(g.size(), 0);
	EXPECT_EQ(g.shapes()[0].get(), weak_s);
	EXPECT_EQ(weak_s->parent, &g);
}

//Scenario: Intersecting a ray with an empty group
//  Given g ← group()
//    And r ← ray(point(0, 0, 0), vector(0, 0, 1))
//  When xs ← local_intersect(g, r)
//  Then xs is empty
TEST(Group, IntersectEmpty) {
	Group g;
	Ray r(Point3(0, 0, 0), Vec3(0, 0, 1));
	IntersectionList xs;
	g.local_intersect(r, xs);
	EXPECT_EQ(xs.size(), 0);
}

//Scenario: Intersecting a ray with a nonempty group
//  Given g ← group()
//    And s1 ← sphere()
//    And s2 ← sphere()
//    And set_transform(s2, translation(0, 0, -3))
//    And s3 ← sphere()
//    And set_transform(s3, translation(5, 0, 0))
//    And add_child(g, s1)
//    And add_child(g, s2)
//    And add_child(g, s3)
//  When r ← ray(point(0, 0, -5), vector(0, 0, 1))
//    And xs ← local_intersect(g, r)
//  Then xs.count = 4
//    And xs[0].object = s2
//    And xs[1].object = s2
//    And xs[2].object = s1
//    And xs[3].object = s1
TEST(Group, Intersect) {
	Group g;

	auto s1 = std::make_unique<Sphere>();
	Shape* weak_s1 = s1.get();

	auto s2 = std::make_unique<Sphere>();
	Shape* weak_s2 = s2.get();
	s2->transform = Matrix4::translate(0, 0, -3);

	auto s3 = std::make_unique<Sphere>();
	s3->transform = Matrix4::translate(5, 0, 0);

	g.add(std::move(s1));
	g.add(std::move(s2));
	g.add(std::move(s3));

	Ray r(Point3(0, 0, -5), Vec3(0, 0, 1));
	IntersectionList xs;
	g.local_intersect(r, xs);
	EXPECT_EQ(xs.size(), 4);
	EXPECT_EQ(xs.at(0).object, weak_s1);
	EXPECT_EQ(xs.at(1).object, weak_s1);
	EXPECT_EQ(xs.at(2).object, weak_s2);
	EXPECT_EQ(xs.at(3).object, weak_s2);
}

//Scenario: Intersecting a transformed group
//  Given g ← group()
//    And set_transform(g, scaling(2, 2, 2))
//    And s ← sphere()
//    And set_transform(s, translation(5, 0, 0))
//    And add_child(g, s)
//  When r ← ray(point(10, 0, -10), vector(0, 0, 1))
//    And xs ← intersect(g, r)
//  Then xs.count = 2
TEST(Group, IntersectTransformedGroup) {
	Group g;
	g.transform = Matrix4::scale(2, 2, 2);

	auto s = std::make_unique<Sphere>();
	s->transform = Matrix4::translate(5, 0, 0);
	g.add(std::move(s));

	Ray r(Point3(10, 0, -10), Vec3(0, 0, 1));
	IntersectionList xs;
	g.intersect(r, xs);
	EXPECT_EQ(xs.size(), 2);
}
