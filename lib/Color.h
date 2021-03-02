#pragma once

#include "Tuple.h"

namespace ray {

	class Color
	{
	public:
		double r = 0.0f;
		double g = 0.0f;
		double b = 0.0f;

		Color(double inr, double ing, double inb) {
			r = inr;
			g = ing;
			b = inb;
		}

		friend std::ostream& operator<<(std::ostream& os, const Color& c) {
			return os << "Color(" << c.r << ", " << c.g << ", " << c.b << ")";
		}

		bool operator==(const Color& rhs) const {
			return IsEqual(r, rhs.r) && IsEqual(g, rhs.g) && IsEqual(b, rhs.b);
		}
		bool operator!=(const Color& rhs) const {
			return !operator==(rhs);
		}

		Color operator+(const Color& rhs) const {
			return Color(r + rhs.r, g + rhs.g, b + rhs.b);
		}
		Color operator-(const Color& rhs) const {
			return Color(r - rhs.r, g - rhs.g, b - rhs.b);
		}
		Color operator*(const Color& rhs) const {
			return Color(r * rhs.r, g * rhs.g, b * rhs.b);
		}
		Color operator*(double s) const {
			return Color(s * r, s * g, s * b);
		}
		static Color white() {
			return Color(1, 1, 1);
		}
		static Color black() {
			return Color(0,0,0);
		}
		static Color red() {
			return Color(1, 0,0);
		}
	};

} // namespace ray
