#include "pch.h"
#include "Material.h"
#include "Shape.h"
#include "Intersect.h"

using namespace ray;


//Feature: Cylinders
//
//Scenario Outline: A ray misses a cylinder
//  Given cyl ← cylinder()
//    And direction ← normalize(<direction>)
//    And r ← ray(<origin>, direction)
//  When xs ← local_intersect(cyl, r)
//  Then xs.count = 0
//
//  Examples:
//    | origin          | direction       |
//    | point(1, 0, 0)  | vector(0, 1, 0) |
//    | point(0, 0, 0)  | vector(0, 1, 0) |
//    | point(0, 0, -5) | vector(1, 1, 1) |
TEST(Cylinder, Miss) {
	Cylinder c;

	struct {
		Point3 p;
		Vec3 v;
	} expect[] = {
		{ Point3(1, 0, 0)  , Vec3(0,1,0) },
		{ Point3(0, 0, 0)  , Vec3(0,1,0) },
		{ Point3(0, 0, -5)  , Vec3(1,1,1) }
	};

	for (auto test : expect) {
		Ray r(test.p, test.v);
		IntersectionList xs;
		c.local_intersect(r, xs);
		EXPECT_EQ(xs.size(), 0);
	}
}

//Scenario Outline: A ray strikes a cylinder
//  Given cyl ← cylinder()
//    And direction ← normalize(<direction>)
//    And r ← ray(<origin>, direction)
//  When xs ← local_intersect(cyl, r)
//  Then xs.count = 2
//    And xs[0].t = <t0>
//    And xs[1].t = <t1>
//
//  Examples:
//    | origin            | direction         | t0      | t1      |
//    | point(1, 0, -5)   | vector(0, 0, 1)   | 5       | 5       |
//    | point(0, 0, -5)   | vector(0, 0, 1)   | 4       | 6       |
//    | point(0.5, 0, -5) | vector(0.1, 1, 1) | 6.80798 | 7.08872 |
TEST(Cylinder, LocalIntersect) {
	Cylinder c;

	struct {
		Point3 p;
		Vec3 v;
		double t1;
		double t2;
	} expect[] = {
		{ Point3(1, 0, -5), Vec3(0, 0,1), 5, 5 },
		{ Point3(0, 0, -5), Vec3(0, 0,1), 4, 6 },
		{ Point3(0.5, 0, -5), Vec3(0.1, 1,1), 6.80798 , 7.08872 }
	};

	for (auto test : expect) {
		Ray r(test.p, test.v.norm());
		IntersectionList xs;
		c.local_intersect(r, xs);
		EXPECT_NEAR(test.t1, xs.at(0).t, RAY_EPSILON);
		EXPECT_NEAR(test.t2, xs.at(1).t, RAY_EPSILON);
	}
}

//Scenario Outline: Normal vector on a cylinder
//  Given cyl ← cylinder()
//  When n ← local_normal_at(cyl, <point>)
//  Then n = <normal>
//
//  Examples:
//    | point           | normal           |
//    | point(1, 0, 0)  | vector(1, 0, 0)  |
//    | point(0, 5, -1) | vector(0, 0, -1) |
//    | point(0, -2, 1) | vector(0, 0, 1)  |
//    | point(-1, 1, 0) | vector(-1, 0, 0) |
TEST(Cylinder, Normal) {
	Cylinder c;

	struct {
		Point3 p;
		Vec3 n;
	} expect[] = {
		{ Point3(1, 0, 0)  , Vec3(1, 0, 0)  },
		{ Point3(0, 5, -1)  , Vec3(0, 0, -1)  },
		{ Point3(0, -2,1)  , Vec3(0,0,1)  },
		{ Point3(-1,1, 0)  , Vec3(-1, 0, 0)  },
	};

	for (auto test : expect) {
		Vec3 actual = c.local_normal_at(test.p, Intersection(1,&c));
		EXPECT_EQ(test.n, actual);
	}
}

//Scenario: The default minimum and maximum for a cylinder
//  Given cyl ← cylinder()
//  Then cyl.minimum = -infinity
//    And cyl.maximum = infinity
TEST(Cylinder, Ctor) {
	Cylinder c;
	EXPECT_EQ(c.minimum, -std::numeric_limits<double>::max());
	EXPECT_EQ(c.maximum, std::numeric_limits<double>::max());
}

//Scenario Outline: Intersecting a constrained cylinder
//  Given cyl ← cylinder()
//    And cyl.minimum ← 1
//    And cyl.maximum ← 2
//    And direction ← normalize(<direction>)
//    And r ← ray(<point>, direction)
//  When xs ← local_intersect(cyl, r)
//  Then xs.count = <count>
//
//  Examples:
//    |   | point             | direction         | count |
//    | 1 | point(0, 1.5, 0)  | vector(0.1, 1, 0) | 0     |
//    | 2 | point(0, 3, -5)   | vector(0, 0, 1)   | 0     |
//    | 3 | point(0, 0, -5)   | vector(0, 0, 1)   | 0     |
//    | 4 | point(0, 2, -5)   | vector(0, 0, 1)   | 0     |
//    | 5 | point(0, 1, -5)   | vector(0, 0, 1)   | 0     |
//    | 6 | point(0, 1.5, -2) | vector(0, 0, 1)   | 2     |
TEST(Cylinder, HitClosed) {
	Cylinder c;
	c.minimum = 1;
	c.maximum = 2;

	struct {
		Point3 p;
		Vec3 v;
	} expect[] = {
		{ Point3(0, 1.5, 0)  , Vec3(0.1, 1, 0)      },
		{ Point3(0, 3, -5)   , Vec3(0, 0, 1)        },
		{ Point3(0, 0, -5)   , Vec3(0, 0, 1)        },
		{ Point3(0, 2, -5)   , Vec3(0, 0, 1)        },
		{ Point3(0, 1, -5)   , Vec3(0, 0, 1)        }
	};

	for (auto test : expect) {
		Ray r(test.p, test.v.norm());
		IntersectionList xs;
		c.local_intersect(r, xs);
		EXPECT_EQ(xs.size(), 0);
	}

	{
		Ray r2(Point3(0, 1.5, -2), Vec3(0, 0, 1));
		IntersectionList xs2;
		c.local_intersect(r2, xs2);
		EXPECT_EQ(xs2.size(), 2);
	}
}

//Scenario: The default closed value for a cylinder
//  Given cyl ← cylinder()
//  Then cyl.closed = false
TEST(Cylinder, CtorClosed) {
	Cylinder c;
	EXPECT_FALSE(c.closed);
}

//Scenario Outline: Intersecting the caps of a closed cylinder
//  Given cyl ← cylinder()
//    And cyl.minimum ← 1
//    And cyl.maximum ← 2
//    And cyl.closed ← true
//    And direction ← normalize(<direction>)
//    And r ← ray(<point>, direction)
//  When xs ← local_intersect(cyl, r)
//  Then xs.count = <count>
//
//  Examples:
//    |   | point            | direction        | count |
//    | 1 | point(0, 3, 0)   | vector(0, -1, 0) | 2     |
//    | 2 | point(0, 3, -2)  | vector(0, -1, 2) | 2     |
//    | 3 | point(0, 4, -2)  | vector(0, -1, 1) | 2     | # corner case
//    | 4 | point(0, 0, -2)  | vector(0, 1, 2)  | 2     |
//    | 5 | point(0, -1, -2) | vector(0, 1, 1)  | 2     | # corner case
TEST(Cylinder, HitCaps) {
	Cylinder c;
	c.minimum = 1;
	c.maximum = 2;
	c.closed = true;

	struct {
		Point3 p;
		Vec3 n;
		int count;
	} expect[] = {
		{ Point3(0, 3, 0)   , Vec3(0, -1, 0) , 2     },
		{ Point3(0, 3, -2)  , Vec3(0, -1, 2) , 2     },
		{ Point3(0, 4, -2)  , Vec3(0, -1, 1) , 2     },
		{ Point3(0, 0, -2)  , Vec3(0, 1, 2)  , 2     },
		{ Point3(0, -1, -2) , Vec3(0, 1, 1)  , 2     }
	};

	for (auto test : expect) {
		Vec3 n = test.n.norm();
		Ray r(test.p, n);
		IntersectionList xs;
		c.local_intersect(r, xs);
		EXPECT_EQ(xs.size(), test.count);
	}
}

//Scenario Outline: The normal vector on a cylinder's end caps
//  Given cyl ← cylinder()
//    And cyl.minimum ← 1
//    And cyl.maximum ← 2
//    And cyl.closed ← true
//  When n ← local_normal_at(cyl, <point>)
//  Then n = <normal>
//
//  Examples:
//    | point            | normal           |
//    | point(0, 1, 0)   | vector(0, -1, 0) |
//    | point(0.5, 1, 0) | vector(0, -1, 0) |
//    | point(0, 1, 0.5) | vector(0, -1, 0) |
//    | point(0, 2, 0)   | vector(0, 1, 0)  |
//    | point(0.5, 2, 0) | vector(0, 1, 0)  |
//    | point(0, 2, 0.5) | vector(0, 1, 0)  |
TEST(Cylinder, CapNormal) {
	Cylinder c;
	c.minimum = 1;
	c.maximum = 2;
	c.closed = true;

	struct {
		Point3 p;
		Vec3 n;
	} expect[] = {
		{ Point3(0, 1, 0)   , Vec3(0, -1, 0) },
		{ Point3(0.5, 1, 0) , Vec3(0, -1, 0) },
		{ Point3(0, 1, 0.5) , Vec3(0, -1, 0) },
		{ Point3(0, 2, 0)   , Vec3(0, 1, 0)  },
		{ Point3(0.5, 2, 0) , Vec3(0, 1, 0)  },
		{ Point3(0, 2, 0.5) , Vec3(0, 1, 0)  }
	};

	for (auto test : expect) {
		Vec3 actual = c.local_normal_at(test.p, Intersection(1, &c));
		EXPECT_EQ(test.n, actual);
	}
}
