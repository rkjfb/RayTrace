#include "pch.h"
#include "Matrix.h"

#define _USE_MATH_DEFINES
#include <cmath>

using namespace ray;

//Feature: Matrix Transformations
//
//Scenario: Multiplying by a translation matrix
//  Given transform ← translation(5, -3, 2)
//    And p ← point(-3, 4, 5)
//   Then transform * p = point(2, 1, 7)
TEST(Transform, Translate) {
	Matrix4 t = Matrix4::translate(5, -3, 2);
	Point3 p(-3, 4, 5);
	Point3 result = t * p;
	EXPECT_EQ(result, Point3(2,1,7));
}

//Scenario: Multiplying by the inverse of a translation matrix
//  Given transform ← translation(5, -3, 2)
//    And inv ← inverse(transform)
//    And p ← point(-3, 4, 5)
//   Then inv * p = point(-8, 7, 3)
TEST(Transform, InverseTranslate) {
	Matrix4 t = Matrix4::translate(5, -3, 2);
	Matrix4 inv = t.inverse();
	Point3 p(-3, 4, 5);
	Point3 result = inv * p;
	EXPECT_EQ(result, Point3(-8,7,3));
}

//Scenario: Translation does not affect vectors
//  Given transform ← translation(5, -3, 2)
//    And v ← vector(-3, 4, 5)
//   Then transform * v = v
TEST(Transform, TranslateVector) {
	Matrix4 t = Matrix4::translate(5, -3, 2);
	Vec3 v(-3, 4, 5);
	Vec3 result = t * v;
	EXPECT_EQ(result, v);
}

//Scenario: A scaling matrix applied to a point
//  Given transform ← scaling(2, 3, 4)
//    And p ← point(-4, 6, 8)
//   Then transform * p = point(-8, 18, 32)
TEST(Transform, ScalePoint) {
	Matrix4 t = Matrix4::scale(2,3,4);
	Point3 p(-4, 6, 8);
	Point3 result = t * p;
	EXPECT_EQ(result, Point3(-8,18,32));
}

//Scenario: A scaling matrix applied to a vector
//  Given transform ← scaling(2, 3, 4)
//    And v ← vector(-4, 6, 8)
//   Then transform * v = vector(-8, 18, 32)
TEST(Transform, ScaleVector) {
	Matrix4 t = Matrix4::scale(2, 3, 4);
	Vec3 v(-4, 6, 8);
	Vec3 result = t * v;
	EXPECT_EQ(result, Vec3(-8, 18, 32));
}

//Scenario: Multiplying by the inverse of a scaling matrix
//  Given transform ← scaling(2, 3, 4)
//    And inv ← inverse(transform)
//    And v ← vector(-4, 6, 8)
//   Then inv * v = vector(-2, 2, 2)
TEST(Transform, InverseScaleVector) {
	Matrix4 t = Matrix4::scale(2, 3, 4);
	Matrix4 inv = t.inverse();
	Vec3 v(-4, 6, 8);
	Vec3 result = inv * v;
	EXPECT_EQ(result, Vec3(-2,2,2));
}

//Scenario: Reflection is scaling by a negative value
//  Given transform ← scaling(-1, 1, 1)
//    And p ← point(2, 3, 4)
//   Then transform * p = point(-2, 3, 4)
TEST(Transform, ScaleReflect) {
	Matrix4 t = Matrix4::scale(-1,1,1);
	Point3 p(2, 3, 4);
	Point3 result = t * p;
	EXPECT_EQ(result, Point3(-2, 3, 4));
}

//Scenario: Rotating a point around the x axis
//  Given p ← point(0, 1, 0)
//    And half_quarter ← rotation_x(π / 4)
//    And full_quarter ← rotation_x(π / 2)
//  Then half_quarter * p = point(0, √2/2, √2/2)
//    And full_quarter * p = point(0, 0, 1)
TEST(Transform, RotateX) {
	Point3 p(0, 1, 0);

	Matrix4 half = Matrix4::rotateX(pi / 4);
	Point3 halfp = half * p;
	EXPECT_EQ(halfp, Point3(0, sqrtf(2) / 2, sqrtf(2) / 2));

	Matrix4 full = Matrix4::rotateX(pi / 2); 
	Point3 fullp = full * p;
	EXPECT_EQ(fullp, Point3(0, 0, 1));
}

//Scenario: The inverse of an x-rotation rotates in the opposite direction
//  Given p ← point(0, 1, 0)
//    And half_quarter ← rotation_x(π / 4)
//    And inv ← inverse(half_quarter)
//  Then inv * p = point(0, √2/2, -√2/2)
TEST(Transform, RotateX2) {
	Point3 p(0, 1, 0);

	Matrix4 half = Matrix4::rotateX(pi / 4);
	Matrix4 inv = half.inverse();
	Point3 invp = inv * p;
	EXPECT_EQ(invp, Point3(0, sqrtf(2) / 2, -sqrtf(2) / 2));
}

//Scenario: Rotating a point around the y axis
//  Given p ← point(0, 0, 1)
//    And half_quarter ← rotation_y(π / 4)
//    And full_quarter ← rotation_y(π / 2)
//  Then half_quarter * p = point(√2/2, 0, √2/2)
//    And full_quarter * p = point(1, 0, 0)
TEST(Transform, RotateY) {
	Point3 p(0, 0, 1);

	Matrix4 half = Matrix4::rotateY(pi / 4);
	Point3 halfp = half * p;
	EXPECT_EQ(halfp, Point3(sqrtf(2) / 2, 0, sqrtf(2) / 2));

	Matrix4 full = Matrix4::rotateY(pi / 2);
	Point3 fullp = full * p;
	EXPECT_EQ(fullp, Point3(1, 0, 0));
}

//Scenario: Rotating a point around the z axis
//  Given p ← point(0, 1, 0)
//    And half_quarter ← rotation_z(π / 4)
//    And full_quarter ← rotation_z(π / 2)
//  Then half_quarter * p = point(-√2/2, √2/2, 0)
//    And full_quarter * p = point(-1, 0, 0)
TEST(Transform, RotateZ) {
	Point3 p(0, 1, 0);

	Matrix4 half = Matrix4::rotateZ(pi / 4);
	Point3 halfp = half * p;
	EXPECT_EQ(halfp, Point3(-sqrtf(2) / 2, sqrtf(2) / 2, 0));

	Matrix4 full = Matrix4::rotateZ(pi / 2);
	Point3 fullp = full * p;
	EXPECT_EQ(fullp, Point3(-1, 0, 0));
}

//Scenario: A shearing transformation moves x in proportion to y
//  Given transform ← shearing(1, 0, 0, 0, 0, 0)
//    And p ← point(2, 3, 4)
//  Then transform * p = point(5, 3, 4)
TEST(Transform, ShearXY) {
	Matrix4 t = Matrix4::shear(1, 0, 0, 0, 0, 0);
	Point3 p(2,3,4);

	Point3 result = t * p;
	EXPECT_EQ(result, Point3(5,3,4));
}

//Scenario: A shearing transformation moves x in proportion to z
//  Given transform ← shearing(0, 1, 0, 0, 0, 0)
//    And p ← point(2, 3, 4)
//  Then transform * p = point(6, 3, 4)
TEST(Transform, ShearXZ) {
	Matrix4 t = Matrix4::shear(0, 1, 0, 0, 0, 0);
	Point3 p(2, 3, 4);

	Point3 result = t * p;
	EXPECT_EQ(result, Point3(6, 3, 4));
}

//Scenario: A shearing transformation moves y in proportion to x
//  Given transform ← shearing(0, 0, 1, 0, 0, 0)
//    And p ← point(2, 3, 4)
//  Then transform * p = point(2, 5, 4)
TEST(Transform, ShearYX) {
	Matrix4 t = Matrix4::shear(0, 0, 1, 0, 0, 0);
	Point3 p(2, 3, 4);

	Point3 result = t * p;
	EXPECT_EQ(result, Point3(2,5, 4));
}

//Scenario: A shearing transformation moves y in proportion to z
//  Given transform ← shearing(0, 0, 0, 1, 0, 0)
//    And p ← point(2, 3, 4)
//  Then transform * p = point(2, 7, 4)
TEST(Transform, ShearYZ) {
	Matrix4 t = Matrix4::shear(0, 0, 0, 1, 0, 0);
	Point3 p(2, 3, 4);

	Point3 result = t * p;
	EXPECT_EQ(result, Point3(2,7, 4));
}

//Scenario: A shearing transformation moves z in proportion to x
//  Given transform ← shearing(0, 0, 0, 0, 1, 0)
//    And p ← point(2, 3, 4)
//  Then transform * p = point(2, 3, 6)
TEST(Transform, ShearZX) {
	Matrix4 t = Matrix4::shear(0, 0, 0, 0, 1, 0);
	Point3 p(2, 3, 4);

	Point3 result = t * p;
	EXPECT_EQ(result, Point3(2, 3, 6));
}

//Scenario: A shearing transformation moves z in proportion to y
//  Given transform ← shearing(0, 0, 0, 0, 0, 1)
//    And p ← point(2, 3, 4)
//  Then transform * p = point(2, 3, 7)
TEST(Transform, ShearZY) {
	Matrix4 t = Matrix4::shear(0, 0, 0, 0, 0, 1);
	Point3 p(2, 3, 4);

	Point3 result = t * p;
	EXPECT_EQ(result, Point3(2, 3, 7));
}

//Scenario: Individual transformations are applied in sequence
//  Given p ← point(1, 0, 1)
//    And A ← rotation_x(π / 2)
//    And B ← scaling(5, 5, 5)
//    And C ← translation(10, 5, 7)
//  # apply rotation first
//  When p2 ← A * p
//  Then p2 = point(1, -1, 0)
//  # then apply scaling
//  When p3 ← B * p2
//  Then p3 = point(5, -5, 0)
//  # then apply translation
//  When p4 ← C * p3
//  Then p4 = point(15, 0, 7)
TEST(Transform, Sequence) {
	Point3 p(1,0,1);
	Matrix4 a = Matrix4::rotateX(pi / 2);
	Matrix4 b = Matrix4::scale(5, 5, 5);
	Matrix4 c = Matrix4::translate(10, 5, 7);

	Point3 p2 = a * p;
	EXPECT_EQ(p2, Point3(1,-1,0));

	Point3 p3 = b * p2;
	EXPECT_EQ(p3, Point3(5, -5, 0));

	Point3 p4 = c * p3;
	EXPECT_EQ(p4, Point3(15, 0, 7));
}

//Scenario: Chained transformations must be applied in reverse order
//  Given p ← point(1, 0, 1)
//    And A ← rotation_x(π / 2)
//    And B ← scaling(5, 5, 5)
//    And C ← translation(10, 5, 7)
//  When T ← C * B * A
//  Then T * p = point(15, 0, 7)
TEST(Transform, Sequence2) {
	Point3 p(1, 0, 1);
	Matrix4 a = Matrix4::rotateX(pi / 2);
	Matrix4 b = Matrix4::scale(5, 5, 5);
	Matrix4 c = Matrix4::translate(10, 5, 7);

	Matrix4 t = c * b * a;

	Point3 p2 = t * p;
	EXPECT_EQ(p2, Point3(15, 0, 7));
}

//Scenario: The transformation matrix for the default orientation
//  Given from ← point(0, 0, 0)
//    And to ← point(0, 0, -1)
//    And up ← vector(0, 1, 0)
//  When t ← view_transform(from, to, up)
//  Then t = identity_matrix
TEST(Transform, ViewDefault) {
	Point3 from(0, 0, 0);
	Point3 to(0, 0, -1);
	Vec3 up(0, 1, 0);
	Matrix4 view = Matrix4::view(from, to, up);
	EXPECT_EQ(view, Matrix4::identity());
}

//Scenario: A view transformation matrix looking in positive z direction
//  Given from ← point(0, 0, 0)
//    And to ← point(0, 0, 1)
//    And up ← vector(0, 1, 0)
//  When t ← view_transform(from, to, up)
//  Then t = scaling(-1, 1, -1)
TEST(Transform, ViewPosZ) {
	Point3 from(0, 0, 0);
	Point3 to(0, 0, 1);
	Vec3 up(0, 1, 0);
	Matrix4 view = Matrix4::view(from, to, up);
	EXPECT_EQ(view, Matrix4::scale(-1,1,-1));
}

//Scenario: The view transformation moves the world
//  Given from ← point(0, 0, 8)
//    And to ← point(0, 0, 0)
//    And up ← vector(0, 1, 0)
//  When t ← view_transform(from, to, up)
//  Then t = translation(0, 0, -8)
TEST(Transform, ViewTranslate) {
	Point3 from(0, 0, 8);
	Point3 to(0, 0, 0);
	Vec3 up(0, 1, 0);
	Matrix4 view = Matrix4::view(from, to, up);
	EXPECT_EQ(view, Matrix4::translate(0,0,-8));
}

//Scenario: An arbitrary view transformation
//  Given from ← point(1, 3, 2)
//    And to ← point(4, -2, 8)
//    And up ← vector(1, 1, 0)
//  When t ← view_transform(from, to, up)
//  Then t is the following 4x4 matrix:
//      | -0.50709 | 0.50709 |  0.67612 | -2.36643 |
//      |  0.76772 | 0.60609 |  0.12122 | -2.82843 |
//      | -0.35857 | 0.59761 | -0.71714 |  0.00000 |
//      |  0.00000 | 0.00000 |  0.00000 |  1.00000 |
TEST(Transform, ViewComplex) {
	Point3 from(1, 3, 2);
	Point3 to(4, -2, 8);
	Vec3 up(1, 1, 0);
	Matrix4 view = Matrix4::view(from, to, up);
	std::array<double, 16> expectArray = {
		- 0.50709f, 0.50709f, 0.67612f, -2.36643f,
		0.76772f, 0.60609f, 0.12122f, -2.82843f,
		-0.35857f, 0.59761f, -0.71714f, 0.00000f,
		0.00000f, 0.00000f, 0.00000f, 1.00000
	};
	Matrix4 expect(expectArray);

	EXPECT_EQ(view, expect);
}
