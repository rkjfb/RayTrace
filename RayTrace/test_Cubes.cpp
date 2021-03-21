#include "pch.h"

#include "Shape.h"
#include "Intersect.h"

using namespace ray;


//Feature: Cubes
//
//Scenario Outline: A ray intersects a cube
//  Given c ← cube()
//    And r ← ray(<origin>, <direction>)
//  When xs ← local_intersect(c, r)
//  Then xs.count = 2
//    And xs[0].t = <t1>
//    And xs[1].t = <t2>
//
//  Examples:
//    |        | origin            | direction        | t1 | t2 |
//    | +x     | point(5, 0.5, 0)  | vector(-1, 0, 0) |  4 |  6 |
//    | -x     | point(-5, 0.5, 0) | vector(1, 0, 0)  |  4 |  6 |
//    | +y     | point(0.5, 5, 0)  | vector(0, -1, 0) |  4 |  6 |
//    | -y     | point(0.5, -5, 0) | vector(0, 1, 0)  |  4 |  6 |
//    | +z     | point(0.5, 0, 5)  | vector(0, 0, -1) |  4 |  6 |
//    | -z     | point(0.5, 0, -5) | vector(0, 0, 1)  |  4 |  6 |
//    | inside | point(0, 0.5, 0)  | vector(0, 0, 1)  | -1 |  1 |
TEST(Cube, LocalIntersect) {
	Cube c;

	struct {
		Point3 p;
		Vec3 v;
		double t1;
		double t2;
	} expect[] = {
		{ Point3(5, 0.5, 0), Vec3(-1, 0, 0), 4, 6 },
		{ Point3(-5, 0.5, 0), Vec3(1, 0, 0)  ,  4 ,  6 },
		{ Point3(0.5, 5, 0), Vec3(0, -1, 0) ,  4 ,  6 },
		{ Point3(0.5, -5, 0), Vec3(0, 1, 0)  ,  4 ,  6 },
		{ Point3(0.5, 0, 5), Vec3(0, 0, -1) ,  4 ,  6 },
		{ Point3(0.5, 0, -5), Vec3(0, 0, 1)  ,  4 ,  6 },
		{ Point3(0, 0.5, 0), Vec3(0, 0, 1)  , -1 ,  1 }
	};

	for (auto test : expect) {
		Ray r(test.p, test.v);
		IntersectionList xs;
		c.local_intersect(r, xs);
		EXPECT_EQ(test.t1, xs.at(0).t);
		EXPECT_EQ(test.t2, xs.at(1).t);
	}
}

//Scenario Outline: A ray misses a cube
//  Given c ← cube()
//    And r ← ray(<origin>, <direction>)
//  When xs ← local_intersect(c, r)
//  Then xs.count = 0
//
//  Examples:
//    | origin           | direction                      |
//    | point(-2, 0, 0)  | vector(0.2673, 0.5345, 0.8018) |
//    | point(0, -2, 0)  | vector(0.8018, 0.2673, 0.5345) |
//    | point(0, 0, -2)  | vector(0.5345, 0.8018, 0.2673) |
//    | point(2, 0, 2)   | vector(0, 0, -1)               |
//    | point(0, 2, 2)   | vector(0, -1, 0)               |
//    | point(2, 2, 0)   | vector(-1, 0, 0)               |
TEST(Cube, MissIntersect) {
	Cube c;

	struct {
		Point3 p;
		Vec3 v;
	} expect[] = {
		{ Point3(-2, 0, 0)  , Vec3(0.2673, 0.5345, 0.8018) },
		{ Point3(0, -2, 0)  , Vec3(0.8018, 0.2673, 0.5345) },
		{ Point3(0, 0, -2)  , Vec3(0.5345, 0.8018, 0.2673) },
		{ Point3(2, 0, 2)   , Vec3(0, 0, -1)               },
		{ Point3(0, 2, 2)   , Vec3(0, -1, 0)               },
		{ Point3(2, 2, 0)   , Vec3(-1, 0, 0)               }
	};

	for (auto test : expect) {
		Ray r(test.p, test.v);
		IntersectionList xs;
		c.local_intersect(r, xs);
		EXPECT_EQ(xs.size(), 0);
	}
}

//
//Scenario Outline: The normal on the surface of a cube
//  Given c ← cube()
//    And p ← <point>
//  When normal ← local_normal_at(c, p)
//  Then normal = <normal>
//
//  Examples:
//    | point                | normal           |
//    | point(1, 0.5, -0.8)  | vector(1, 0, 0)  |
//    | point(-1, -0.2, 0.9) | vector(-1, 0, 0) |
//    | point(-0.4, 1, -0.1) | vector(0, 1, 0)  |
//    | point(0.3, -1, -0.7) | vector(0, -1, 0) |
//    | point(-0.6, 0.3, 1)  | vector(0, 0, 1)  |
//    | point(0.4, 0.4, -1)  | vector(0, 0, -1) |
//    | point(1, 1, 1)       | vector(1, 0, 0)  |
//    | point(-1, -1, -1)    | vector(-1, 0, 0) |
TEST(Cube, Normal) {
	Cube c;

	struct {
		Point3 p;
		Vec3 n;
	} expect[] = {
		{ Point3(1, 0.5, -0.8)  , Vec3(1, 0, 0)  },
		{ Point3(-1, -0.2, 0.9) , Vec3(-1, 0, 0) },
		{ Point3(-0.4, 1, -0.1) , Vec3(0, 1, 0)  },
		{ Point3(0.3, -1, -0.7) , Vec3(0, -1, 0) },
		{ Point3(-0.6, 0.3, 1)  , Vec3(0, 0, 1)  },
		{ Point3(0.4, 0.4, -1)  , Vec3(0, 0, -1) },
		{ Point3(1, 1, 1)       , Vec3(1, 0, 0)  },
		{ Point3(-1, -1, -1)    , Vec3(-1, 0, 0) }
	};

	for (auto test : expect) {
		Vec3 actual = c.local_normal_at(test.p);
		EXPECT_EQ(test.n, actual);
	}
}
