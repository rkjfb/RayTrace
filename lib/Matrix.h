#pragma once
#include <array>
#include "Tuple.h"
#include "Ray.h"

namespace ray {

	class Matrix2
	{
	public:
		double m00 = 0, m01 = 0;
		double m10 = 0, m11 = 0;
		Matrix2() {
			m00 = m11 = 1;
		}
		Matrix2(std::array<double, 4>& i) {
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

		double det() const {
			return m00 * m11 - m01 * m10;
		}

	};

	class Matrix3
	{
	public:
		double m00 = 0, m01 = 0, m02 = 0;
		double m10 = 0, m11 = 0, m12 = 0;
		double m20 = 0, m21 = 0, m22 = 0;
		Matrix3() {
			m00 = m11 = m22 = 1;
		}
		Matrix3(std::array<double, 9>& i) {
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
		std::array<double, 3> row(int i) const {
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
		std::array<double, 3> col(int i) const {
			switch (i) {
			case 0:
				return { m00, m10, m20 };
			case 1:
				return { m01, m11, m21 };
			case 2:
				break;
			default:
				assert(false);
			}
			return { m02, m12, m22 };
		}

		Matrix2 submatrix(int droprow, int dropcol) const {
			std::array<double, 4> m2;
			int index = 0;
			for (int i = 0; i < 3; i++) {
				if (i == droprow) continue;
				std::array<double, 3> r = row(i);
				for (int j = 0; j < 3; j++) {
					if (j == dropcol) continue;
					m2[index] = r[j];
					index++;
				}
			}
			assert(index == 4);
			return Matrix2(m2);
		}

		double minor(int row, int col) const {
			Matrix2 m = submatrix(row, col);
			return m.det();
		}

		double cofactor(int row, int col) const {
			double f = minor(row, col);
			if ((row + col) % 2 == 1) {
				f = -f;
			}
			return f;
		}

		double det() const {
			double det = 0;
			std::array<double, 3> r = row(0);
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
			m[0] = m[5] = m[10] = m[15] = 1;
			update_inverse_cache();
		}
		Matrix4(std::array<double, 16>& a) : m(a) {
			update_inverse_cache();
		}

		Matrix4(const std::array<double, 16>& a, const std::array<double, 16>& inv) : m(a), inverse_cache(inv) {
		}


		friend std::ostream& operator<<(std::ostream& os, const Matrix4& m) {
			return os << "Matrix4("
				<< m.m[0] << ", "
				<< m.m[1] << ", "
				<< m.m[2] << ", "
				<< m.m[3] << ", "
				<< m.m[4] << ", "
				<< m.m[5] << ", "
				<< m.m[6] << ", "
				<< m.m[7] << ", "
				<< m.m[8] << ", "
				<< m.m[9] << ", "
				<< m.m[10] << ", "
				<< m.m[11] << ", "
				<< m.m[12] << ", "
				<< m.m[13] << ", "
				<< m.m[14] << ", "
				<< m.m[15] << ")";
		}

		bool operator==(const Matrix4& rhs) const {
			return IsEqual(m[0], rhs.m[0]) &&
				IsEqual(m[1], rhs.m[1]) &&
				IsEqual(m[2], rhs.m[2]) &&
				IsEqual(m[3], rhs.m[3]) &&
				IsEqual(m[4], rhs.m[4]) &&
				IsEqual(m[5], rhs.m[5]) &&
				IsEqual(m[6], rhs.m[6]) &&
				IsEqual(m[7], rhs.m[7]) &&
				IsEqual(m[8], rhs.m[8]) &&
				IsEqual(m[9], rhs.m[9]) &&
				IsEqual(m[10], rhs.m[10]) &&
				IsEqual(m[11], rhs.m[11]) &&
				IsEqual(m[12], rhs.m[12]) &&
				IsEqual(m[13], rhs.m[13]) &&
				IsEqual(m[14], rhs.m[14]) &&
				IsEqual(m[15], rhs.m[15]);
		}
		bool operator!=(const Matrix4& rhs) const {
			return !operator==(rhs);
		}

		// returns supplied row as array
		std::array<double, 4> row(int i) const {
			switch (i) {
			case 0:
				return { m[0], m[1], m[2], m[3] };
			case 1:
				return { m[4], m[5], m[6], m[7] };
			case 2:
				return { m[8], m[9], m[10], m[11] };
			case 3:
				break;
			default:
				assert(false);
			}
			return { m[12], m[13], m[14], m[15] };
		}

		// returns supplied column as array
		std::array<double, 4> col(int i) const {
			switch (i) {
			case 0:
				return { m[0], m[4], m[8], m[12] };
			case 1:
				return { m[1], m[5], m[9], m[13] };
			case 2:
				return { m[2], m[6], m[10], m[14] };
			case 3:
				break;
			default:
				assert(false);
			}
			return { m[3], m[7], m[11], m[15] };
		}

	private:
		static Point3 multiply(std::array<double, 16> m, const Point3& p) {
			return Point3(
				p.x * m[0] + p.y * m[1] + p.z * m[2] + m[3],
				p.x * m[4] + p.y * m[5] + p.z * m[6] + m[7],
				p.x * m[8] + p.y * m[9] + p.z * m[10] + m[11]
			);
		}
		static Vec3 multiply(std::array<double, 16> m, const Vec3& v) {
			return Vec3(
				v.x * m[0] + v.y * m[1] + v.z * m[2] + v.w * m[3],
				v.x * m[4] + v.y * m[5] + v.z * m[6] + v.w * m[7],
				v.x * m[8] + v.y * m[9] + v.z * m[10] + v.w * m[11],
				v.x * m[12] + v.y * m[13] + v.z * m[14] + v.w * m[15]
			);
		}

	public:
		Vec3 operator*(const Vec3& rhs) const {
			return Vec3(
				rhs.x * m[0] + rhs.y * m[1] + rhs.z * m[2] + rhs.w * m[3],
				rhs.x * m[4] + rhs.y * m[5] + rhs.z * m[6] + rhs.w * m[7],
				rhs.x * m[8] + rhs.y * m[9] + rhs.z * m[10] + rhs.w * m[11],
				rhs.x * m[12] + rhs.y * m[13] + rhs.z * m[14] + rhs.w * m[15]
			);
		}

		// Perf: Returns the this.inverse() * v
		Vec3 inverse_multiply(const Vec3& v) const {
			return multiply(inverse_cache, v);
		}

		Point3 operator*(const Point3& p) const {
			return multiply(m, p);
		}

		// Perf: Returns the this.inverse() * p
		Point3 inverse_multiply(const Point3& p) const {
			return multiply(inverse_cache, p);
		}

		Ray operator*(const Ray& rhs) const {
			return Ray(operator*(rhs.origin), operator*(rhs.direction()));
		}

		// Perf: Returns the this.inverse() * r
		Ray inverse_multiply(const Ray& r) const {
			return Ray(inverse_multiply(r.origin), inverse_multiply(r.direction()));
		}

		Matrix4 operator*(const Matrix4& rhs) const {
			std::array<double, 16> ret;
			for (int i = 0; i < 4; i++) {
				std::array<double, 4> r = row(i);
				for (int j = 0; j < 4; j++) {
					std::array<double, 4> c = rhs.col(j);
					ret[i * 4 + j] = c[0] * r[0] + c[1] * r[1] + c[2] * r[2] + c[3] * r[3];
				}
			}
			return Matrix4(ret);
		}

		// returns the transpose of a.
		static std::array<double, 16> transpose_array(const std::array<double, 16>& a) {
			return std::array<double, 16>{
				a[0], a[4], a[8], a[12],
					a[1], a[5], a[9], a[13],
					a[2], a[6], a[10], a[14],
					a[3], a[7], a[11], a[15]
			};
		}

		Matrix4 transpose() const {
			auto at = transpose_array(to_array());
			auto invt = transpose_array(inverse_cache);
			return Matrix4(at, invt);
		}

		Matrix3 submatrix(int droprow, int dropcol) const {
			std::array<double, 9> m3;
			int index = 0;
			for (int i = 0; i < 4; i++) {
				if (i == droprow) continue;
				std::array<double, 4> r = row(i);
				for (int j = 0; j < 4; j++) {
					if (j == dropcol) continue;
					m3[index] = r[j];
					index++;
				}
			}
			assert(index == 9);
			return Matrix3(m3);
		}

		double minor(int row, int col) const {
			Matrix3 m = submatrix(row, col);
			return m.det();
		}

		double cofactor(int row, int col) const {
			double f = minor(row, col);
			if ((row + col) % 2 == 1) {
				f = -f;
			}
			return f;
		}

		double det() const {
			double det = 0;
			std::array<double, 4> r = row(0);
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

			std::array<double, 16> m4;
			double mul = 1 / det();
			for (int row = 0; row < 4; row++) {
				for (int col = 0; col < 4; col++) {
					double c = cofactor(row, col);
					// (column, row) to transpose
					m4[col * 4 + row] = mul * c;
				}
			}

			return Matrix4(m4);
		}

		std::array<double, 16> to_array() const {
			return {
				m[0], m[1],m[2],m[3],
				m[4], m[5],m[6],m[7],
				m[8], m[9],m[10],m[11],
				m[12], m[13],m[14],m[15] };
		}

		Matrix4 inverse() const
		{
			return Matrix4(inverse_cache, to_array());
		}

		static Matrix4 identity() {
			// currently initialize to identity, so no-op.
			return Matrix4();
		}

		static Matrix4 translate(double x, double y, double z) {
			Matrix4 m;
			m.m[3] = x;
			m.m[7] = y;
			m.m[11] = z;
			m.update_inverse_cache();
			return m;
		}

		static Matrix4 scale(double x, double y, double z) {
			Matrix4 m;
			m.m[0] = x;
			m.m[5] = y;
			m.m[10] = z;
			m.update_inverse_cache();
			return m;
		}

		static Matrix4 rotateX(double r) {
			Matrix4 m;
			m.m[5] = m.m[10] = cos(r);
			m.m[9] = sin(r);
			m.m[6] = -m.m[9];
			m.update_inverse_cache();
			return m;
		}

		static Matrix4 rotateY(double r) {
			Matrix4 m;
			m.m[0] = m.m[10] = cos(r);
			m.m[2] = sin(r);
			m.m[8] = -m.m[2];
			m.update_inverse_cache();
			return m;
		}

		static Matrix4 rotateZ(double r) {
			Matrix4 m;
			m.m[0] = m.m[5] = cos(r);
			m.m[4] = sin(r);
			m.m[1] = -m.m[4];
			m.update_inverse_cache();
			return m;
		}

		static Matrix4 shear(double xy, double xz, double yx, double yz, double zx, double zy) {
			Matrix4 m;
			m.m[1] = xy;
			m.m[2] = xz;
			m.m[4] = yx;
			m.m[6] = yz;
			m.m[8] = zx;
			m.m[9] = zy;
			m.update_inverse_cache();
			return m;
		}

		static Matrix4 view(const Point3& from, const Point3& to, const Vec3& up) {
			Vec3 forward = (to - from).norm();
			Vec3 upn = up.norm();
			Vec3 left = forward.cross(upn);
			Vec3 true_up = left.cross(forward);
			std::array<double, 16> a = {
				left.x, left.y, left.z, 0,
					true_up.x, true_up.y, true_up.z, 0,
					-forward.x, -forward.y, -forward.z, 0,
					0,0,0,1
			};
			Matrix4 rotate(a);
			return rotate * translate(-from.x, -from.y, -from.z);
		}

	private:

		// main matrix
		std::array<double, 16> m = {};

		// cache of inverse of this matrix, needed because inverting is high frequency.
		std::array<double, 16> inverse_cache;
		// Much faster invert than the book version.
		// Copied from https://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
		// Which copied from https://glm.g-truc.net/0.9.9/index.html

		void update_inverse_cache() {
			std::array<double, 16>& inv = inverse_cache;
			double det;
			int i;

			inv[0] = m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];
			inv[4] = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];
			inv[8] = m[4] * m[9] * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];
			inv[12] = -m[4] * m[9] * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];
			inv[1] = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];
			inv[5] = m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];
			inv[9] = -m[0] * m[9] * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];
			inv[13] = m[0] * m[9] * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];
			inv[2] = m[1] * m[6] * m[15] - m[1] * m[7] * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] + m[13] * m[2] * m[7] - m[13] * m[3] * m[6];
			inv[6] = -m[0] * m[6] * m[15] + m[0] * m[7] * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] - m[12] * m[2] * m[7] + m[12] * m[3] * m[6];
			inv[10] = m[0] * m[5] * m[15] - m[0] * m[7] * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] + m[12] * m[1] * m[7] - m[12] * m[3] * m[5];
			inv[14] = -m[0] * m[5] * m[14] + m[0] * m[6] * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] - m[12] * m[1] * m[6] + m[12] * m[2] * m[5];
			inv[3] = -m[1] * m[6] * m[11] + m[1] * m[7] * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] - m[9] * m[2] * m[7] + m[9] * m[3] * m[6];
			inv[7] = m[0] * m[6] * m[11] - m[0] * m[7] * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] + m[8] * m[2] * m[7] - m[8] * m[3] * m[6];
			inv[11] = -m[0] * m[5] * m[11] + m[0] * m[7] * m[9] + m[4] * m[1] * m[11] - m[4] * m[3] * m[9] - m[8] * m[1] * m[7] + m[8] * m[3] * m[5];
			inv[15] = m[0] * m[5] * m[10] - m[0] * m[6] * m[9] - m[4] * m[1] * m[10] + m[4] * m[2] * m[9] + m[8] * m[1] * m[6] - m[8] * m[2] * m[5];

			det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];
			if (det != 0) {
				det = 1.0 / det;

				for (i = 0; i < 16; i++) {
					inv[i] = static_cast<double>(inv[i] * det);
				}
			}
			else
			{
				inv = Matrix4::identity().to_array();
			}
		}
	};
} // namespace ray


