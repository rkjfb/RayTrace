#pragma once
#include <array>
#include "Tuple.h"
#include "Ray.h"

namespace ray {

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
		friend std::ostream& operator<<(std::ostream& os, const Matrix2& m) {
			return os << "Matrix2("
				<< m.m00 << ", "
				<< m.m01 << ", "
				<< m.m10 << ", "
				<< m.m11 << ")";
		}

		bool operator==(const Matrix2& rhs) const {
			return IsEqual(m00, rhs.m00) &&
				IsEqual(m01, rhs.m01) &&
				IsEqual(m10, rhs.m10) &&
				IsEqual(m11, rhs.m11);
		}
		bool operator!=(const Matrix2& rhs) const {
			return !operator==(rhs);
		}

		float det() const {
			return m00 * m11 - m01 * m10;
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

		friend std::ostream& operator<<(std::ostream& os, const Matrix3& m) {
			return os << "Matrix3("
				<< m.m00 << ", "
				<< m.m01 << ", "
				<< m.m02 << ", "
				<< m.m10 << ", "
				<< m.m11 << ", "
				<< m.m12 << ", "
				<< m.m20 << ", "
				<< m.m21 << ", "
				<< m.m22 << ")";
		}

		bool operator==(const Matrix3& rhs) const {
			return IsEqual(m00, rhs.m00) &&
				IsEqual(m01, rhs.m01) &&
				IsEqual(m02, rhs.m02) &&
				IsEqual(m10, rhs.m10) &&
				IsEqual(m11, rhs.m11) &&
				IsEqual(m12, rhs.m12) &&
				IsEqual(m20, rhs.m20) &&
				IsEqual(m21, rhs.m21) &&
				IsEqual(m22, rhs.m22);
		}
		bool operator!=(const Matrix3& rhs) const {
			return !operator==(rhs);
		}

		// returns supplied row as array
		std::array<float, 3> row(int i) const {
			switch (i) {
			case 0:
				return { m00, m01, m02 };
			case 1:
				return { m10, m11, m12 };
			case 2:
				break;
			default:
				assert(false);
			}
			return { m20, m21, m22 };
		}

		// returns supplied column as array
		std::array<float, 3> col(int i) const {
			switch (i) {
			case 0:
				return { m00, m10, m20 };
			case 1:
				return { m01, m11, m21};
			case 2:
				break;
			default:
				assert(false);
			}
			return { m02, m12, m22 };
		}

		Matrix2 submatrix(int droprow, int dropcol) const {
			std::array<float, 4> m2;
			int index = 0;
			for (int i = 0; i < 3; i++) {
				if (i == droprow) continue;
				std::array<float, 3> r = row(i);
				for (int j = 0; j < 3; j++) {
					if (j == dropcol) continue;
					m2[index] = r[j];
					index++;
				}
			}
			assert(index == 4);
			return Matrix2(m2);
		}

		float minor(int row, int col) const {
			Matrix2 m = submatrix(row, col);
			return m.det();
		}

		float cofactor(int row, int col) const {
			float f = minor(row, col);
			if ((row + col) % 2 == 1) {
				f = -f;
			}
			return f;
		}

		float det() const {
			float det = 0;
			std::array<float, 3> r = row(0);
			for (int i = 0; i < r.size(); i++) {
				det += r[i] * cofactor(0, i);
			}
			return det;
		}
	};

	class Matrix4
	{
	public:
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
		friend std::ostream& operator<<(std::ostream& os, const Matrix4& m) {
			return os << "Matrix4("
				<< m.m00 << ", "
				<< m.m01 << ", "
				<< m.m02 << ", "
				<< m.m03 << ", "
				<< m.m10 << ", "
				<< m.m11 << ", "
				<< m.m12 << ", "
				<< m.m13 << ", "
				<< m.m20 << ", "
				<< m.m21 << ", "
				<< m.m22 << ", "
				<< m.m23 << ", "
				<< m.m30 << ", "
				<< m.m31 << ", "
				<< m.m32 << ", "
				<< m.m33 << ")";
		}

		bool operator==(const Matrix4& rhs) const {
			return IsEqual(m00, rhs.m00) &&
				IsEqual(m01, rhs.m01) &&
				IsEqual(m02, rhs.m02) &&
				IsEqual(m03, rhs.m03) &&
				IsEqual(m10, rhs.m10) &&
				IsEqual(m11, rhs.m11) &&
				IsEqual(m12, rhs.m12) &&
				IsEqual(m13, rhs.m13) &&
				IsEqual(m20, rhs.m20) &&
				IsEqual(m21, rhs.m21) &&
				IsEqual(m22, rhs.m22) &&
				IsEqual(m23, rhs.m23) &&
				IsEqual(m30, rhs.m30) &&
				IsEqual(m31, rhs.m31) &&
				IsEqual(m32, rhs.m32) &&
				IsEqual(m33, rhs.m33);
		}
		bool operator!=(const Matrix4& rhs) const {
			return !operator==(rhs);
		}

		// returns supplied row as array
		std::array<float, 4> row(int i) const {
			switch (i) {
			case 0:
				return { m00, m01, m02, m03 };
			case 1:
				return { m10, m11, m12, m13 };
			case 2:
				return { m20, m21, m22, m23 };
			case 3:
				break;
			default:
				assert(false);
			}
			return { m30, m31, m32, m33 };
		}

		// returns supplied column as array
		std::array<float, 4> col(int i) const {
			switch (i) {
			case 0:
				return { m00, m10, m20, m30 };
			case 1:
				return { m01, m11, m21, m31 };
			case 2:
				return { m02, m12, m22, m32 };
			case 3:
				break;
			default:
				assert(false);
			}
			return { m03, m13, m23, m33 };
		}

		Vec3 operator*(const Vec3& rhs) const {
			std::array<float, 4> ret;
			for (int i = 0; i < 4; i++) {
				std::array<float, 4> r = row(i);
				ret[i] = rhs.x * r[0] + rhs.y * r[1] + rhs.z * r[2] + rhs.w * r[3];
			}
			return Vec3(ret);
		}

		Point3 operator*(const Point3& rhs) const {
			std::array<float, 3> ret;
			for (int i = 0; i < ret.size(); i++) {
				std::array<float, 4> r = row(i);
				ret[i] = rhs.x * r[0] + rhs.y * r[1] + rhs.z * r[2] + r[3];
			}
			return Point3(ret);
		}

		Ray operator*(const Ray& rhs) const {
			return Ray(operator*(rhs.origin), operator*(rhs.direction));
		}

		Matrix4 operator*(const Matrix4& rhs) const {
			std::array<float, 16> ret;
			for (int i = 0; i < 4; i++) {
				std::array<float, 4> r = row(i);
				for (int j = 0; j < 4; j++) {
					std::array<float, 4> c = rhs.col(j);
					ret[i*4 + j] = c[0] * r[0] + c[1] * r[1] + c[2] * r[2] + c[3] * r[3];
				}
			}
			return Matrix4(ret);
		}

		Matrix4 transpose() const {
			auto a = std::array<float, 16>{
				m00, m10, m20, m30,
				m01, m11, m21, m31,
				m02, m12, m22, m32,
				m03, m13, m23, m33
				};
			return Matrix4(a);
		}

		Matrix3 submatrix(int droprow, int dropcol) const {
			std::array<float, 9> m3;
			int index = 0;
			for (int i = 0; i < 4; i++) {
				if (i == droprow) continue;
				std::array<float, 4> r = row(i);
				for (int j = 0; j < 4; j++) {
					if (j == dropcol) continue;
					m3[index] = r[j];
					index++;
				}
			}
			assert(index == 9);
			return Matrix3(m3);
		}

		float minor(int row, int col) const {
			Matrix3 m = submatrix(row, col);
			return m.det();
		}

		float cofactor(int row, int col) const {
			float f = minor(row, col);
			if ((row + col) % 2 == 1) {
				f = -f;
			}
			return f;
		}

		float det() const {
			float det = 0;
			std::array<float, 4> r = row(0);
			for (int i = 0; i < r.size(); i++) {
				det += r[i] * cofactor(0, i);
			}
			return det;
		}

		bool invertible() const {
			return !IsEqual(det(), 0);
		}

		// Inverse from the book: very slow.
		Matrix4 inverse_book() const {
			// Pseudocode:
			// 1. Matrix4 of cofactors
			// 2. transpose
			// 3. divide by determinant
			//
			// We're trying to skip the transpose and intermediate storage steps here.
			assert(invertible());

			std::array<float, 16> m4;
			float mul = 1 / det();
			for (int row = 0; row < 4; row++) {
				for (int col = 0; col < 4; col++) {
					float c = cofactor(row, col);
					// (column, row) to transpose
					m4[col * 4 + row] = mul * c;
				}
			}

			return Matrix4(m4);
		}

		std::array<float, 16> to_array() const {
			return {
				m00, m01,m02,m03,
				m10, m11,m12,m13,
				m20, m21,m22,m23,
				m30, m31,m32,m33 };
		}

		// Much faster invert than the book version.
		// Copied from https://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
		// Which copied from https://glm.g-truc.net/0.9.9/index.html
		//bool InverseMat44(Matrix4& out)
		Matrix4 inverse() const
		{
			std::array<float, 16> inv;
			double det;
			int i;

			inv[0] = m11 * m22 * m33 - m11 * m23 * m32 - m21 * m12 * m33 + m21 * m13 * m32 + m31 * m12 * m23 - m31 * m13 * m22;
			inv[4] = -m10 * m22 * m33 + m10 * m23 * m32 + m20 * m12 * m33 - m20 * m13 * m32 - m30 * m12 * m23 + m30 * m13 * m22;
			inv[8] = m10 * m21 * m33 - m10 * m23 * m31 - m20 * m11 * m33 + m20 * m13 * m31 + m30 * m11 * m23 - m30 * m13 * m21;
			inv[12] = -m10 * m21 * m32 + m10 * m22 * m31 + m20 * m11 * m32 - m20 * m12 * m31 - m30 * m11 * m22 + m30 * m12 * m21;
			inv[1] = -m01 * m22 * m33 + m01 * m23 * m32 + m21 * m02 * m33 - m21 * m03 * m32 - m31 * m02 * m23 + m31 * m03 * m22;
			inv[5] = m00 * m22 * m33 - m00 * m23 * m32 - m20 * m02 * m33 + m20 * m03 * m32 + m30 * m02 * m23 - m30 * m03 * m22;
			inv[9] = -m00 * m21 * m33 + m00 * m23 * m31 + m20 * m01 * m33 - m20 * m03 * m31 - m30 * m01 * m23 + m30 * m03 * m21;
			inv[13] = m00 * m21 * m32 - m00 * m22 * m31 - m20 * m01 * m32 + m20 * m02 * m31 + m30 * m01 * m22 - m30 * m02 * m21;
			inv[2] = m01 * m12 * m33 - m01 * m13 * m32 - m11 * m02 * m33 + m11 * m03 * m32 + m31 * m02 * m13 - m31 * m03 * m12;
			inv[6] = -m00 * m12 * m33 + m00 * m13 * m32 + m10 * m02 * m33 - m10 * m03 * m32 - m30 * m02 * m13 + m30 * m03 * m12;
			inv[10] = m00 * m11 * m33 - m00 * m13 * m31 - m10 * m01 * m33 + m10 * m03 * m31 + m30 * m01 * m13 - m30 * m03 * m11;
			inv[14] = -m00 * m11 * m32 + m00 * m12 * m31 + m10 * m01 * m32 - m10 * m02 * m31 - m30 * m01 * m12 + m30 * m02 * m11;
			inv[3] = -m01 * m12 * m23 + m01 * m13 * m22 + m11 * m02 * m23 - m11 * m03 * m22 - m21 * m02 * m13 + m21 * m03 * m12;
			inv[7] = m00 * m12 * m23 - m00 * m13 * m22 - m10 * m02 * m23 + m10 * m03 * m22 + m20 * m02 * m13 - m20 * m03 * m12;
			inv[11] = -m00 * m11 * m23 + m00 * m13 * m21 + m10 * m01 * m23 - m10 * m03 * m21 - m20 * m01 * m13 + m20 * m03 * m11;
			inv[15] = m00 * m11 * m22 - m00 * m12 * m21 - m10 * m01 * m22 + m10 * m02 * m21 + m20 * m01 * m12 - m20 * m02 * m11;

			det = m00 * inv[0] + m01 * inv[4] + m02 * inv[8] + m03 * inv[12]; 
			assert(det != 0);

			det = 1.0 / det;

			for (i = 0; i < 16; i++)
				inv[i] = static_cast<float>(inv[i] * det);

			return Matrix4(inv);
		}

		static Matrix4 identity() {
			// currently initialize to identity, so no-op.
			return Matrix4();
		}

		static Matrix4 translate(float x, float y, float z) {
			Matrix4 m;
			m.m03 = x;
			m.m13 = y;
			m.m23 = z;
			return m;
		}

		static Matrix4 scale(float x, float y, float z) {
			Matrix4 m;
			m.m00 = x;
			m.m11 = y;
			m.m22 = z;
			return m;
		}

		static Matrix4 rotateX(float r) {
			Matrix4 m;
			m.m11 = m.m22 = cos(r);
			m.m21 = sin(r);
			m.m12 = -m.m21;
			return m;
		}

		static Matrix4 rotateY(float r) {
			Matrix4 m;
			m.m00 = m.m22 = cos(r);
			m.m02 = sin(r);
			m.m20 = -m.m02;
			return m;
		}

		static Matrix4 rotateZ(float r) {
			Matrix4 m;
			m.m00 = m.m11 = cos(r);
			m.m10 = sin(r);
			m.m01 = -m.m10;
			return m;
		}

		static Matrix4 shear(float xy, float xz, float yx, float yz, float zx, float zy) {
			Matrix4 m;
			m.m01 = xy;
			m.m02 = xz;
			m.m10 = yx;
			m.m12 = yz;
			m.m20 = zx;
			m.m21 = zy;
			return m;
		}

		static Matrix4 view(const Point3& from, const Point3& to, const Vec3& up) {
			Vec3 forward = (to - from).norm();
			Vec3 upn = up.norm();
			Vec3 left = forward.cross(upn);
			Vec3 true_up = left.cross(forward);
			std::array<float, 16> a = {
				left.x, left.y, left.z, 0,
					true_up.x, true_up.y, true_up.z, 0,
					-forward.x, -forward.y, -forward.z, 0,
					0,0,0,1
			};
			Matrix4 rotate(a);
			return rotate * translate(-from.x, -from.y, -from.z);
		}
	private:
		float m00 = 0, m01 = 0, m02 = 0, m03 = 0;
		float m10 = 0, m11 = 0, m12 = 0, m13 = 0;
		float m20 = 0, m21 = 0, m22 = 0, m23 = 0;
		float m30 = 0, m31 = 0, m32 = 0, m33 = 0;
	};
} // namespace ray


