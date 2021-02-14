#pragma once
#include <array>
#include "Tuple.h"

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

		static Matrix4 identity() {
			// currently initialize to identity, so no-op.
			return Matrix4();
		}

	};
} // namespace ray

