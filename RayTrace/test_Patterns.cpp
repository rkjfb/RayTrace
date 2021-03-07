#include "pch.h"
#include "Pattern.h"
#include "Shape.h"

using namespace ray;

//Feature: Patterns
//
//Background:
//  Given black ← color(0, 0, 0)
//    And white ← color(1, 1, 1)
//
//Scenario: Creating a stripe pattern
//  Given pattern ← stripe_pattern(white, black)
//  Then pattern.a = white
//    And pattern.b = black
TEST(Pattern, StripeCtor) {
	Stripe pattern(Color::white(), Color::black());
	EXPECT_EQ(pattern.a, Color::white());
	EXPECT_EQ(pattern.b, Color::black());
}

//Scenario: A stripe pattern is constant in y
//  Given pattern ← stripe_pattern(white, black)
//  Then stripe_at(pattern, point(0, 0, 0)) = white
//    And stripe_at(pattern, point(0, 1, 0)) = white
//    And stripe_at(pattern, point(0, 2, 0)) = white
TEST(Pattern, StripeConstantY) {
	Stripe pattern(Color::white(), Color::black());
	EXPECT_EQ(pattern.pattern_at(Point3(0, 0, 0)), Color::white());
	EXPECT_EQ(pattern.pattern_at(Point3(0, 1, 0)), Color::white());
	EXPECT_EQ(pattern.pattern_at(Point3(0, 2, 0)), Color::white());
}

//Scenario: A stripe pattern is constant in z
//  Given pattern ← stripe_pattern(white, black)
//  Then stripe_at(pattern, point(0, 0, 0)) = white
//    And stripe_at(pattern, point(0, 0, 1)) = white
//    And stripe_at(pattern, point(0, 0, 2)) = white
TEST(Pattern, StripeConstantZ) {
	Stripe pattern(Color::white(), Color::black());
	EXPECT_EQ(pattern.pattern_at(Point3(0, 0, 0)), Color::white());
	EXPECT_EQ(pattern.pattern_at(Point3(0, 0, 1)), Color::white());
	EXPECT_EQ(pattern.pattern_at(Point3(0, 0, 2)), Color::white());
}

//Scenario: A stripe pattern alternates in x
//  Given pattern ← stripe_pattern(white, black)
//  Then stripe_at(pattern, point(0, 0, 0)) = white
//    And stripe_at(pattern, point(0.9, 0, 0)) = white
//    And stripe_at(pattern, point(1, 0, 0)) = black
//    And stripe_at(pattern, point(-0.1, 0, 0)) = black
//    And stripe_at(pattern, point(-1, 0, 0)) = black
//    And stripe_at(pattern, point(-1.1, 0, 0)) = white
TEST(Pattern, StripeAlternateX) {
	Stripe pattern(Color::white(), Color::black());
	EXPECT_EQ(pattern.pattern_at(Point3(0, 0, 0)), Color::white());
	EXPECT_EQ(pattern.pattern_at(Point3(0.9, 0, 0)), Color::white());
	EXPECT_EQ(pattern.pattern_at(Point3(1, 0, 0)), Color::black());
	EXPECT_EQ(pattern.pattern_at(Point3(-0.1, 0, 0)), Color::black());
	EXPECT_EQ(pattern.pattern_at(Point3(-1, 0, 0)), Color::black());
	EXPECT_EQ(pattern.pattern_at(Point3(-1.1, 0, 0)), Color::white());
}

//Scenario: Stripes with an object transformation
//  Given object ← sphere()
//    And set_transform(object, scaling(2, 2, 2))
//    And pattern ← stripe_pattern(white, black)
//  When c ← stripe_at_object(pattern, object, point(1.5, 0, 0))
//  Then c = white
TEST(Pattern, StripeObjectTransform) {
	Sphere sphere;
	sphere.transform = Matrix4::scale(2, 2, 2);
	Stripe pattern(Color::white(), Color::black());
	Color result = pattern.pattern_at_shape(sphere, Point3(1.5, 0, 0));

	EXPECT_EQ(result, Color::white());
}

//Scenario: Stripes with a pattern transformation
//  Given object ← sphere()
//    And pattern ← stripe_pattern(white, black)
//    And set_pattern_transform(pattern, scaling(2, 2, 2))
//  When c ← stripe_at_object(pattern, object, point(1.5, 0, 0))
//  Then c = white
TEST(Pattern, StripePatternTransform) {
	Sphere sphere;
	Stripe pattern(Color::white(), Color::black());
	pattern.transform = Matrix4::scale(2, 2, 2);

	Color result = pattern.pattern_at_shape(sphere, Point3(1.5, 0, 0));

	EXPECT_EQ(result, Color::white());
}

//Scenario: Stripes with both an object and a pattern transformation
//  Given object ← sphere()
//    And set_transform(object, scaling(2, 2, 2))
//    And pattern ← stripe_pattern(white, black)
//    And set_pattern_transform(pattern, translation(0.5, 0, 0))
//  When c ← stripe_at_object(pattern, object, point(2.5, 0, 0))
//  Then c = white
TEST(Pattern, StripeDoubleTransform) {
	Sphere sphere;
	sphere.transform = Matrix4::scale(2, 2, 2);
	Stripe pattern(Color::white(), Color::black());
	pattern.transform = Matrix4::translate(0.5, 0, 0);

	Color result = pattern.pattern_at_shape(sphere, Point3(2.5, 0, 0));

	EXPECT_EQ(result, Color::white());
}

//Scenario: The default pattern transformation
//  Given pattern ← test_pattern()
//  Then pattern.transform = identity_matrix
TEST(Pattern, PatternCtorTransform) {
	std::unique_ptr<Pattern> pattern = std::make_unique<Solid>();
	EXPECT_EQ(pattern->transform, Matrix4::identity());
}

//Scenario: Assigning a transformation
//  Given pattern ← test_pattern()
//  When set_pattern_transform(pattern, translation(1, 2, 3))
//  Then pattern.transform = translation(1, 2, 3)
TEST(Pattern, PatternTransformAssign) {
	std::unique_ptr<Pattern> pattern = std::make_unique<Solid>();
	pattern->transform = Matrix4::translate(1, 2, 3);
	EXPECT_EQ(pattern->transform, Matrix4::translate(1,2,3));
}

//Scenario: A pattern with an object transformation
//  Given shape ← sphere()
//    And set_transform(shape, scaling(2, 2, 2))
//    And pattern ← test_pattern()
//  When c ← pattern_at_shape(pattern, shape, point(2, 3, 4))
//  Then c = color(1, 1.5, 2)
TEST(Pattern, PatternObjectTransform) {
	Sphere shape;
	shape.transform = Matrix4::scale(2, 2, 2);
	std::unique_ptr<Pattern> pattern = std::make_unique<TestPattern>();
	Color result = pattern->pattern_at_shape(shape, Point3(2, 3, 4));
	EXPECT_EQ(result, Color(1,1.5,2));
}

//Scenario: A pattern with a pattern transformation
//  Given shape ← sphere()
//    And pattern ← test_pattern()
//    And set_pattern_transform(pattern, scaling(2, 2, 2))
//  When c ← pattern_at_shape(pattern, shape, point(2, 3, 4))
//  Then c = color(1, 1.5, 2)
TEST(Pattern, PatternPatternTransform) {
	Sphere shape;
	std::unique_ptr<Pattern> pattern = std::make_unique<TestPattern>();
	pattern->transform = Matrix4::scale(2, 2, 2);
	Color result = pattern->pattern_at_shape(shape, Point3(2, 3, 4));
	EXPECT_EQ(result, Color(1, 1.5, 2));
}

//Scenario: A pattern with both an object and a pattern transformation
//  Given shape ← sphere()
//    And set_transform(shape, scaling(2, 2, 2))
//    And pattern ← test_pattern()
//    And set_pattern_transform(pattern, translation(0.5, 1, 1.5))
//  When c ← pattern_at_shape(pattern, shape, point(2.5, 3, 3.5))
//  Then c = color(0.75, 0.5, 0.25)
TEST(Pattern, PatternDoubleTransform) {
	Sphere shape;
	shape.transform = Matrix4::scale(2, 2, 2);
	std::unique_ptr<Pattern> pattern = std::make_unique<TestPattern>();
	pattern->transform = Matrix4::translate(0.5, 1, 1.5);
	Color result = pattern->pattern_at_shape(shape, Point3(2.5, 3, 3.5));
	EXPECT_EQ(result, Color(0.75, 0.5, 0.25));
}

//Scenario: A gradient linearly interpolates between colors
//  Given pattern ← gradient_pattern(white, black)
//  Then pattern_at(pattern, point(0, 0, 0)) = white
//    And pattern_at(pattern, point(0.25, 0, 0)) = color(0.75, 0.75, 0.75)
//    And pattern_at(pattern, point(0.5, 0, 0)) = color(0.5, 0.5, 0.5)
//    And pattern_at(pattern, point(0.75, 0, 0)) = color(0.25, 0.25, 0.25)
TEST(Pattern, GradientPatternAt) {
	Gradient pattern(Color::white(), Color::black());
	EXPECT_EQ(pattern.pattern_at(Point3()), Color::white());
	Color result = pattern.pattern_at(Point3(0.25, 0, 0));
	EXPECT_EQ(result, Color(0.75, 0.75, 0.75));
	EXPECT_EQ(pattern.pattern_at(Point3(0.5, 0, 0)), Color(0.5, 0.5, 0.5));
	EXPECT_EQ(pattern.pattern_at(Point3(0.75, 0, 0)), Color(0.25, 0.25, 0.25));
}

TEST(Pattern, RingGradient) {
	RingGradient pattern(Color::white(), Color::black());
	EXPECT_EQ(pattern.pattern_at(Point3()), Color::white());
	double ss = sqrt(0.25 * 0.25 / 2);
	EXPECT_EQ(pattern.pattern_at(Point3(ss, 0, ss)), Color(0.75, 0.75, 0.75));
	ss = sqrt(0.5 * 0.5 / 2);
	EXPECT_EQ(pattern.pattern_at(Point3(ss, 0, ss)), Color(0.5, 0.5, 0.5));
	ss = sqrt(0.75 * 0.75 / 2);
	EXPECT_EQ(pattern.pattern_at(Point3(ss, 0, ss)), Color(0.25, 0.25, 0.25));
}

//Scenario: A ring should extend in both x and z
//  Given pattern ← ring_pattern(white, black)
//  Then pattern_at(pattern, point(0, 0, 0)) = white
//    And pattern_at(pattern, point(1, 0, 0)) = black
//    And pattern_at(pattern, point(0, 0, 1)) = black
//    # 0.708 = just slightly more than √2/2
//    And pattern_at(pattern, point(0.708, 0, 0.708)) = black
TEST(Pattern, RingPatternAt) {
	Ring pattern(Color::white(), Color::black());
	EXPECT_EQ(pattern.pattern_at(Point3()), Color::white());
	EXPECT_EQ(pattern.pattern_at(Point3(1,0,0)), Color::black());
	EXPECT_EQ(pattern.pattern_at(Point3(0, 0, 1)), Color::black());
	EXPECT_EQ(pattern.pattern_at(Point3(0.708,0,0.708)), Color::black());
}

//Scenario: Checkers should repeat in x
//  Given pattern ← checkers_pattern(white, black)
//  Then pattern_at(pattern, point(0, 0, 0)) = white
//    And pattern_at(pattern, point(0.99, 0, 0)) = white
//    And pattern_at(pattern, point(1.01, 0, 0)) = black
TEST(Pattern, CheckerX) {
	Checker pattern(Color::white(), Color::black());
	EXPECT_EQ(pattern.pattern_at(Point3()), Color::white());
	EXPECT_EQ(pattern.pattern_at(Point3(0.99, 0, 0)), Color::white());
	EXPECT_EQ(pattern.pattern_at(Point3(1.01, 0, 0)), Color::black());
}

//Scenario: Checkers should repeat in y
//  Given pattern ← checkers_pattern(white, black)
//  Then pattern_at(pattern, point(0, 0, 0)) = white
//    And pattern_at(pattern, point(0, 0.99, 0)) = white
//    And pattern_at(pattern, point(0, 1.01, 0)) = black
TEST(Pattern, CheckerY) {
	Checker pattern(Color::white(), Color::black());
	EXPECT_EQ(pattern.pattern_at(Point3()), Color::white());
	EXPECT_EQ(pattern.pattern_at(Point3(0, 0.99, 0)), Color::white());
	EXPECT_EQ(pattern.pattern_at(Point3(0, 1.01, 0)), Color::black());
}

//Scenario: Checkers should repeat in z
//  Given pattern ← checkers_pattern(white, black)
//  Then pattern_at(pattern, point(0, 0, 0)) = white
//    And pattern_at(pattern, point(0, 0, 0.99)) = white
//    And pattern_at(pattern, point(0, 0, 1.01)) = black
TEST(Pattern, CheckerZ) {
	Checker pattern(Color::white(), Color::black());
	EXPECT_EQ(pattern.pattern_at(Point3()), Color::white());
	EXPECT_EQ(pattern.pattern_at(Point3(0, 0, 0.99)), Color::white());
	EXPECT_EQ(pattern.pattern_at(Point3(0, 0, 1.01)), Color::black());
}
