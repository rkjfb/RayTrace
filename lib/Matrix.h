#pragma once
#include <array>

class Matrix2
{
public:
	float m00 = 0, m01 = 0;
	float m10 = 0, m11 = 0;
	Matrix2() {
		m00 = m11 = 1;
	}
	Matrix2(std::array<float, 4>& i) {
		m00 = i[0];
		m01 = i[1];

		m10 = i[2];
		m11 = i[3];
	}
};

class Matrix3
{
public:
	float m00 = 0, m01 = 0, m02 = 0;
	float m10 = 0, m11 = 0, m12 = 0;
	float m20 = 0, m21 = 0, m22 = 0;
	Matrix3() {
		m00 = m11 = m22 = 1;
	}
	Matrix3(std::array<float, 9>& i) {
		m00 = i[0];
		m01 = i[1];
		m02 = i[2];

		m10 = i[3];
		m11 = i[4];
		m12 = i[5];

		m20 = i[6];
		m21 = i[7];
		m22 = i[8];
	}
};

class Matrix4
{
public:
	float m00 = 0, m01 = 0, m02 = 0, m03 = 0;
	float m10 = 0, m11 = 0, m12 = 0, m13 = 0;
	float m20 = 0, m21 = 0, m22 = 0, m23 = 0;
	float m30 = 0, m31 = 0, m32 = 0, m33 = 0;
	Matrix4() {
		m00 = m11 = m22 = m33 = 1;
	}
	Matrix4(std::array<float, 16>& i) {
		m00 = i[0];
		m01 = i[1];
		m02 = i[2];
		m03 = i[3];

		m10 = i[4];
		m11 = i[5];
		m12 = i[6];
		m13 = i[7];

		m20 = i[8];
		m21 = i[9];
		m22 = i[10];
		m23 = i[11];

		m30 = i[12];
		m31 = i[13];
		m32 = i[14];
		m33 = i[15];
	}
};



