#pragma once

#include "Tuple.h"

namespace ray {

	class Color
	{
	public:
		double r = 0.0f;
		double g = 0.0f;
		double b = 0.0f;

		Color(double inr, double ing, double inb) : r(inr), g(ing), b(inb) {}

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
		static Color cssrgb(int r, int g, int b) {
			return Color((double)r / 255, (double)g / 255, (double)b / 255);
		}
		// CSS 3 color names: https://www.w3.org/TR/2018/REC-css-color-3-20180619/
		static Color white() { return Color(1, 1, 1); }
		static Color black() { return Color(0,0,0);	}
		static Color red() { return Color(1, 0,0);}
		static Color blue() { return Color(0, 0, 1); }
		static Color lime() { return Color(0, 1, 0); }

		// darkred	#8B0000	139, 0, 0
		static Color darkred() { return cssrgb(139, 0, 0);  }

		// darksalmon	#E9967A	233, 150, 122
		static Color darksalmon() { return cssrgb(233, 150, 122); }
		//slategrey	#708090	112,128,144
		static Color slategrey() { return cssrgb(112, 128, 144); }
		//lightgrey	#D3D3D3	211,211,211
		static Color lightgrey() { return cssrgb(211, 211, 211); }
	};

} // namespace ray
