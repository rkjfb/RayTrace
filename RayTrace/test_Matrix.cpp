#include "pch.h"
#include "Matrix.h"

using namespace ray;

//Feature: Matrices
//
//Scenario: Constructing and inspecting a 4x4 matrix
//  Given the following 4x4 matrix M:
//    |  1   |  2   |  3   |  4   |
//    |  5.5 |  6.5 |  7.5 |  8.5 |
//    |  9   | 10   | 11   | 12   |
//    | 13.5 | 14.5 | 15.5 | 16.5 |
//  Then M[0,0] = 1
//    And M[0,3] = 4
//    And M[1,0] = 5.5
//    And M[1,2] = 7.5
//    And M[2,2] = 11
//    And M[3,0] = 13.5
//    And M[3,2] = 15.5
TEST(Matrix, Ctor4) {
	Matrix4 m1;
	std::array<double, 16> a1 = m1.to_array();
	EXPECT_EQ(a1[0], 1);

	Matrix4 m2(std::array<double,16>{ 
		1,2,3,4,
			5.5, 6.5, 7.5,  8.5,
			9, 10, 11, 12,
			13.5, 14.5, 15.5,16.5 
	});
	std::array<double, 16> a2 = m2.to_array();
	EXPECT_EQ(a2[0], 1);
	EXPECT_EQ(a2[3], 4);
	EXPECT_EQ(a2[4], 5.5);
	EXPECT_EQ(a2[6], 7.5);
	EXPECT_EQ(a2[10], 11);
	EXPECT_EQ(a2[12], 13.5);
	EXPECT_EQ(a2[14], 15.5);
}

//Scenario: A 2x2 matrix ought to be representable
//  Given the following 2x2 matrix M:
//    | -3 |  5 |
//    |  1 | -2 |
//  Then M[0,0] = -3
//    And M[0,1] = 5
//    And M[1,0] = 1
//    And M[1,1] = -2
TEST(Matrix, Ctor2) {
	Matrix2 m1;
	EXPECT_EQ(m1.m00, 1);
	Matrix2 m2(std::array<double, 4>{
		-3, 5, 1, -2
	});
	EXPECT_EQ(m2.m00, -3);
	EXPECT_EQ(m2.m01, 5);
	EXPECT_EQ(m2.m10, 1);
	EXPECT_EQ(m2.m11, -2);
}

//Scenario: A 3x3 matrix ought to be representable
//  Given the following 3x3 matrix M:
//    | -3 |  5 |  0 |
//    |  1 | -2 | -7 |
//    |  0 |  1 |  1 |
//  Then M[0,0] = -3
//    And M[1,1] = -2
//    And M[2,2] = 1
TEST(Matrix, Ctor3) {
	Matrix3 m1;
	EXPECT_EQ(m1.m00, 1);
	Matrix3 m2(std::array<double, 9>{
		-3, 5, 0,
		1, -2, -7,
		0,  1,  1
	});
	EXPECT_EQ(m2.m00, -3);
	EXPECT_EQ(m2.m11, -2);
	EXPECT_EQ(m2.m22, 1);
}

//Scenario: Matrix equality with identical matrices
//  Given the following matrix A:
//      | 1 | 2 | 3 | 4 |
//      | 5 | 6 | 7 | 8 |
//      | 9 | 8 | 7 | 6 |
//      | 5 | 4 | 3 | 2 |
//    And the following matrix B:
//      | 1 | 2 | 3 | 4 |
//      | 5 | 6 | 7 | 8 |
//      | 9 | 8 | 7 | 6 |
//      | 5 | 4 | 3 | 2 |
//  Then A = B
TEST(Matrix, Equal4) {
	Matrix4 a(std::array<double, 16>{
		1, 2, 3, 4,
			5, 6, 7, 8,
			9, 8, 7, 6,
			5, 4, 3, 2
	});
	Matrix4 b(std::array<double, 16>{
		1, 2, 3, 4,
			5, 6, 7, 8,
			9, 8, 7, 6,
			5, 4, 3, 2
	});
	EXPECT_EQ(a, b);
}

//Scenario: Matrix equality with different matrices
//  Given the following matrix A:
//      | 1 | 2 | 3 | 4 |
//      | 5 | 6 | 7 | 8 |
//      | 9 | 8 | 7 | 6 |
//      | 5 | 4 | 3 | 2 |
//    And the following matrix B:
//      | 2 | 3 | 4 | 5 |
//      | 6 | 7 | 8 | 9 |
//      | 8 | 7 | 6 | 5 |
//      | 4 | 3 | 2 | 1 |
//  Then A != B
TEST(Matrix, NotEqual4) {
	Matrix4 a(std::array<double, 16>{
		1, 2, 3, 4,
			5, 6, 7, 8,
			9, 8, 7, 6,
			5, 4, 3, 2
	});
	Matrix4 b(std::array<double, 16>{
	 2, 3, 4, 5, 
	 6, 7, 8, 9, 
	 8, 7, 6, 5, 
	 4, 3, 2, 1
	});
	EXPECT_NE(a, b);
}

//Scenario: Multiplying two matrices
//  Given the following matrix A:
//      | 1 | 2 | 3 | 4 |
//      | 5 | 6 | 7 | 8 |
//      | 9 | 8 | 7 | 6 |
//      | 5 | 4 | 3 | 2 |
//    And the following matrix B:
//      | -2 | 1 | 2 |  3 |
//      |  3 | 2 | 1 | -1 |
//      |  4 | 3 | 6 |  5 |
//      |  1 | 2 | 7 |  8 |
//  Then A * B is the following 4x4 matrix:
//      | 20|  22 |  50 |  48 |
//      | 44|  54 | 114 | 108 |
//      | 40|  58 | 110 | 102 |
//      | 16|  26 |  46 |  42 |
TEST(Matrix, MulMatrix4) {
	Matrix4 a(std::array<double, 16>{
		1, 2, 3, 4,
			5, 6, 7, 8,
			9, 8, 7, 6,
			5, 4, 3, 2
	});
	Matrix4 b(std::array<double, 16>{
		-2, 1, 2, 3,
		3, 2, 1, -1,
		4, 3, 6, 5,
		1, 2, 7, 8
	});
	Matrix4 result = a * b;

	Matrix4 expect(std::array<double, 16>{
		20, 22, 50, 48,
		44, 54, 114,108,
		40, 58, 110,102,
		16, 26, 46, 42 
	});
	EXPECT_EQ(result, expect);
}

//Scenario: A matrix multiplied by a tuple
//  Given the following matrix A:
//      | 1 | 2 | 3 | 4 |
//      | 2 | 4 | 4 | 2 |
//      | 8 | 6 | 4 | 1 |
//      | 0 | 0 | 0 | 1 |
//    And b ← tuple(1, 2, 3, 1)
//  Then A * b = tuple(18, 24, 33, 1)
TEST(Matrix, MulVec3) {
	Matrix4 a(std::array<double, 16>{
		1, 2, 3, 4,
			2, 4, 4, 2,
			8, 6, 4, 1,
			0, 0, 0, 1
	});
	Vec3 b(1, 2, 3, 1);
	Vec3 result = a * b;
	EXPECT_EQ(result, Vec3(18, 24, 33, 1));
}

//Scenario: Multiplying a matrix by the identity matrix
//  Given the following matrix A:
//    | 0 | 1 |  2 |  4 |
//    | 1 | 2 |  4 |  8 |
//    | 2 | 4 |  8 | 16 |
//    | 4 | 8 | 16 | 32 |
//  Then A * identity_matrix = A
TEST(Matrix, MulIdent) {
	Matrix4 a(std::array<double, 16>{
		0, 1, 2, 4,
		1,2,4,8,
		2,4,8,16,
		4,8,16,32
	});
	Matrix4 result = a * Matrix4::identity();

	EXPECT_EQ(result, a);
}

//Scenario: Multiplying the identity matrix by a tuple
//  Given a ← tuple(1, 2, 3, 4)
//  Then identity_matrix * a = a
TEST(Matrix, MulVec3Ident) {
	Vec3 v(1, 2, 3, 4);
	Vec3 result = Matrix4::identity() * v;

	EXPECT_EQ(result, v);
}

//Scenario: Transposing a matrix
//  Given the following matrix A:
//    | 0 | 9 | 3 | 0 |
//    | 9 | 8 | 0 | 8 |
//    | 1 | 8 | 5 | 3 |
//    | 0 | 0 | 5 | 8 |
//  Then transpose(A) is the following matrix:
//    | 0 | 9 | 1 | 0 |
//    | 9 | 8 | 8 | 0 |
//    | 3 | 0 | 5 | 5 |
//    | 0 | 8 | 3 | 8 |
TEST(Matrix, Transpose) {
	Matrix4 a(std::array<double, 16>{
		0,9,3,0,
		9,8,0,8,
		1,8,5,3,
		0,0,5,8
	});
	Matrix4 result = a.transpose();

	Matrix4 expect(std::array<double, 16>{
		0,9,1,0,
		9,8,8,0,
		3,0,5,5,
		0,8,3,8
	});
	EXPECT_EQ(result, expect);
}

//Scenario: Transposing the identity matrix
//  Given A ← transpose(identity_matrix)
//  Then A = identity_matrix
TEST(Matrix, TransposeIdent) {
	Matrix4 result = Matrix4::identity().transpose();
	EXPECT_EQ(result, Matrix4::identity());
}

//Scenario: Calculating the determinant of a 2x2 matrix
//  Given the following 2x2 matrix A:
//    |  1 | 5 |
//    | -3 | 2 |
//  Then determinant(A) = 17
TEST(Matrix, det2) {
	Matrix2 a(std::array<double, 4>{ 1, 5, -3, 2 });
	EXPECT_EQ(a.det(), 17);
}

//Scenario: A submatrix of a 3x3 matrix is a 2x2 matrix
//  Given the following 3x3 matrix A:
//    |  1 | 5 |  0 |
//    | -3 | 2 |  7 |
//    |  0 | 6 | -3 |
//  Then submatrix(A, 0, 2) is the following 2x2 matrix:
//    | -3 | 2 |
//    |  0 | 6 |
TEST(Matrix, Submatrix3) {
	Matrix3 a(std::array<double, 9>{
		1,5,0,
		-3,2,7,
		0,6,-3
	});
	Matrix2 result = a.submatrix(0, 2);

	Matrix2 expect(std::array<double, 4>{
		-3,2,
		0,6
	});
	EXPECT_EQ(result, expect);
}

//Scenario: A submatrix of a 4x4 matrix is a 3x3 matrix
//  Given the following 4x4 matrix A:
//    | -6 |  1 |  1 |  6 |
//    | -8 |  5 |  8 |  6 |
//    | -1 |  0 |  8 |  2 |
//    | -7 |  1 | -1 |  1 |
//  Then submatrix(A, 2, 1) is the following 3x3 matrix:
//    | -6 |  1 | 6 |
//    | -8 |  8 | 6 |
//    | -7 | -1 | 1 |
TEST(Matrix, Submatrix4) {
	Matrix4 a(std::array<double, 16>{
	-6,1,1,6,
	-8,5,8,6,
	-1,0,8,2,
	-7,1,-1,1
	});
	Matrix3 result = a.submatrix(2, 1);

	Matrix3 expect(std::array<double, 9>{
		-6, 1, 6,
		-8, 8, 6,
		-7, -1, 1
	});
	EXPECT_EQ(result, expect);
}

//Scenario: Calculating a minor of a 3x3 matrix
//  Given the following 3x3 matrix A:
//      |  3 |  5 |  0 |
//      |  2 | -1 | -7 |
//      |  6 | -1 |  5 |
//    And B ← submatrix(A, 1, 0)
//  Then determinant(B) = 25
//    And minor(A, 1, 0) = 25
TEST(Matrix, Minor3) {
	Matrix3 a(std::array<double, 9>{
		3,5,0,
		2,-1,-7,
		6,-1,5
	});
	Matrix2 b = a.submatrix(1, 0);
	EXPECT_EQ(b.det(), 25);
	EXPECT_EQ(a.minor(1, 0), 25);
}

//Scenario: Calculating a cofactor of a 3x3 matrix
//  Given the following 3x3 matrix A:
//      |  3 |  5 |  0 |
//      |  2 | -1 | -7 |
//      |  6 | -1 |  5 |
//  Then minor(A, 0, 0) = -12
//    And cofactor(A, 0, 0) = -12
//    And minor(A, 1, 0) = 25
//    And cofactor(A, 1, 0) = -25
TEST(Matrix, Cofactor3) {
	Matrix3 a(std::array<double, 9>{
		3, 5, 0,
			2, -1, -7,
			6, -1, 5
	});
	EXPECT_EQ(a.minor(0, 0), -12);
	EXPECT_EQ(a.cofactor(0, 0), -12);
	EXPECT_EQ(a.minor(1, 0), 25);
	EXPECT_EQ(a.cofactor(1, 0), -25);
}

//Scenario: Calculating the determinant of a 3x3 matrix
//  Given the following 3x3 matrix A:
//    |  1 |  2 |  6 |
//    | -5 |  8 | -4 |
//    |  2 |  6 |  4 |
//  Then cofactor(A, 0, 0) = 56
//    And cofactor(A, 0, 1) = 12
//    And cofactor(A, 0, 2) = -46
//    And determinant(A) = -196
TEST(Matrix, Determinant3) {
	Matrix3 a(std::array<double, 9>{
		1,2,6,
		-5,8,-4,
		2,6,4
	});
	EXPECT_EQ(a.cofactor(0, 0), 56);
	EXPECT_EQ(a.cofactor(0, 1), 12);
	EXPECT_EQ(a.cofactor(0, 2), -46);
	EXPECT_EQ(a.det(), -196);
}

//Scenario: Calculating the determinant of a 4x4 matrix
//  Given the following 4x4 matrix A:
//    | -2 | -8 |  3 |  5 |
//    | -3 |  1 |  7 |  3 |
//    |  1 |  2 | -9 |  6 |
//    | -6 |  7 |  7 | -9 |
//  Then cofactor(A, 0, 0) = 690
//    And cofactor(A, 0, 1) = 447
//    And cofactor(A, 0, 2) = 210
//    And cofactor(A, 0, 3) = 51
//    And determinant(A) = -4071
TEST(Matrix, Determinant4) {
	Matrix4 a(std::array<double, 16>{
		-2,-8,3,5,
		-3,1,7,3,
		1,2,-9,6,
		-6,7,7,-9
	});
	EXPECT_EQ(a.cofactor(0, 0), 690);
	EXPECT_EQ(a.cofactor(0, 1), 447);
	EXPECT_EQ(a.cofactor(0, 2), 210);
	EXPECT_EQ(a.cofactor(0, 3), 51);
	EXPECT_EQ(a.det(), -4071);
}

//Scenario: Testing an invertible matrix for invertibility
//  Given the following 4x4 matrix A:
//    |  6 |  4 |  4 |  4 |
//    |  5 |  5 |  7 |  6 |
//    |  4 | -9 |  3 | -7 |
//    |  9 |  1 |  7 | -6 |
//  Then determinant(A) = -2120
//    And A is invertible
TEST(Matrix, Invertible4) {
	Matrix4 a(std::array<double, 16>{
		6,4,4,4,
		5,5,7,6,
		4,-9,3,-7,
		9,1,7,-6
	});
	EXPECT_EQ(a.det(), -2120);
	EXPECT_TRUE(a.invertible());
}

//Scenario: Testing a noninvertible matrix for invertibility
//  Given the following 4x4 matrix A:
//    | -4 |  2 | -2 | -3 |
//    |  9 |  6 |  2 |  6 |
//    |  0 | -5 |  1 | -5 |
//    |  0 |  0 |  0 |  0 |
//  Then determinant(A) = 0
//    And A is not invertible
TEST(Matrix, NotInvertible4) {
	Matrix4 a(std::array<double, 16>{
		-4,2,-2,-3,
		9,6,2,6,
		0,-5,1,-5,
		0,0,0,0
	});
	EXPECT_EQ(a.det(), 0);
	EXPECT_FALSE(a.invertible());
}

//Scenario: Calculating the inverse of a matrix
//  Given the following 4x4 matrix A:
//      | -5 |  2 |  6 | -8 |
//      |  1 | -5 |  1 |  8 |
//      |  7 |  7 | -6 | -7 |
//      |  1 | -3 |  7 |  4 |
//    And B ← inverse(A)
//  Then determinant(A) = 532
//    And cofactor(A, 2, 3) = -160
//    And B[3,2] = -160/532
//    And cofactor(A, 3, 2) = 105
//    And B[2,3] = 105/532
//    And B is the following 4x4 matrix:
//      |  0.21805 |  0.45113 |  0.24060 | -0.04511 |
//      | -0.80827 | -1.45677 | -0.44361 |  0.52068 |
//      | -0.07895 | -0.22368 | -0.05263 |  0.19737 |
//      | -0.52256 | -0.81391 | -0.30075 |  0.30639 |
TEST(Matrix, Inverse4) {
	Matrix4 a(std::array<double, 16>{
		-5,2,6,-8,
		1,-5,1,8,
		7,7,-6,-7,
		1,-3,7,4
	});
	Matrix4 b = a.inverse();
	EXPECT_EQ(a.det(), 532);
	EXPECT_EQ(a.cofactor(2, 3), -160);
	std::array<double, 16> array_b = b.to_array();
	EXPECT_NEAR(array_b[14], -160.0 / 532.0, RAY_EPSILON);
	EXPECT_EQ(a.cofactor(3, 2), 105);
	EXPECT_NEAR(array_b[11], 105.0 / 532.0, RAY_EPSILON);
	Matrix4 expect(std::array<double, 16>{
		0.21805f, 0.45113f, 0.24060f, -0.04511f,
		-0.80827f , -1.45677f , -0.44361f ,  0.52068f,
		-0.07895f , -0.22368f , -0.05263f ,  0.19737f,
		-0.52256f , -0.81391f , -0.30075f ,  0.30639f 
	});

	EXPECT_EQ(b, expect);
}

//Scenario: Calculating the inverse of another matrix
//  Given the following 4x4 matrix A:
//    |  8 | -5 |  9 |  2 |
//    |  7 |  5 |  6 |  1 |
//    | -6 |  0 |  9 |  6 |
//    | -3 |  0 | -9 | -4 |
//  Then inverse(A) is the following 4x4 matrix:
//    | -0.15385 | -0.15385 | -0.28205 | -0.53846 |
//    | -0.07692 |  0.12308 |  0.02564 |  0.03077 |
//    |  0.35897 |  0.35897 |  0.43590 |  0.92308 |
//    | -0.69231 | -0.69231 | -0.76923 | -1.92308 |
TEST(Matrix, Inverse4b) {
	Matrix4 a(std::array<double, 16>{
		8, -5, 9, 2,
		7,5,6,1,
		-6,0,9,6,
		-3,0,-9,-4
	});
	Matrix4 b = a.inverse();
	Matrix4 expect(std::array<double, 16>{
		-0.15385f, -0.15385f, -0.28205f, -0.53846f,
			-0.07692f, 0.12308f, 0.02564f, 0.03077f,
			0.35897f, 0.35897f, 0.43590f, 0.92308f,
			-0.69231f, -0.69231f, -0.76923f, -1.92308f
	});

	EXPECT_EQ(b, expect);
}

//Scenario: Calculating the inverse of a third matrix
//  Given the following 4x4 matrix A:
//    |  9 |  3 |  0 |  9 |
//    | -5 | -2 | -6 | -3 |
//    | -4 |  9 |  6 |  4 |
//    | -7 |  6 |  6 |  2 |
//  Then inverse(A) is the following 4x4 matrix:
//    | -0.04074 | -0.07778 |  0.14444 | -0.22222 |
//    | -0.07778 |  0.03333 |  0.36667 | -0.33333 |
//    | -0.02901 | -0.14630 | -0.10926 |  0.12963 |
//    |  0.17778 |  0.06667 | -0.26667 |  0.33333 |
TEST(Matrix, Inverse4c) {
	Matrix4 a(std::array<double, 16>{
		9, 3, 0, 9,
			-5, -2, -6, -3,
			-4, 9, 6, 4,
			-7, 6, 6, 2
	});
	Matrix4 b = a.inverse();
	Matrix4 expect(std::array<double, 16>{
		-0.04074f, -0.07778f, 0.14444f, -0.22222f,
			-0.07778f, 0.03333f, 0.36667f, -0.33333f,
			-0.02901f, -0.14630f, -0.10926f, 0.12963f,
			0.17778f, 0.06667f, -0.26667f, 0.33333f
	});

	EXPECT_EQ(b, expect);
}

//Scenario: Multiplying a product by its inverse
//  Given the following 4x4 matrix A:
//      |  3 | -9 |  7 |  3 |
//      |  3 | -8 |  2 | -9 |
//      | -4 |  4 |  4 |  1 |
//      | -6 |  5 | -1 |  1 |
//    And the following 4x4 matrix B:
//      |  8 |  2 |  2 |  2 |
//      |  3 | -1 |  7 |  0 |
//      |  7 |  0 |  5 |  4 |
//      |  6 | -2 |  0 |  5 |
//    And C ← A * B
//  Then C * inverse(B) = A
TEST(Matrix, InverseRoundTrip) {
	Matrix4 a(std::array<double, 16>{
		3, -9, 7, 3,
			3, -8, 2, -9,
			-4, 4, 4, 1,
			-6, 5, -1, 1
	});
	Matrix4 b(std::array<double, 16>{
		8, 2, 2, 2,
			3, -1, 7, 0,
			7, 0, 5, 4,
			6, -2, 0, 5
	});
	Matrix4 c = a * b;
	Matrix4 result = c * b.inverse();
	EXPECT_EQ(result, a);
}
