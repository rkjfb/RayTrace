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
		//green	#008000	0,128,0
		static Color green() { return cssrgb(0,128,0); }
		//yellow	#FFFF00	255,255,0
		static Color yellow() { return cssrgb(255, 255, 0); }
		//darkorange	#FF8C00	255, 140, 0
		static Color darkorange() { return cssrgb(255, 140, 0); }
		//orchid	#DA70D6	218,112,214
		static Color orchid() { return cssrgb(218,112,214); }
		//powderblue	#B0E0E6	176,224,230
		static Color powderblue() { return cssrgb(176,224,230); }

		// beige	#F5F5DC	245, 245, 220
		static Color beige() { return cssrgb(245, 245, 220); }
		// blanchedalmond	#FFEBCD	255, 235, 205
		static Color blanchedalmond() { return cssrgb(255, 235, 205); }
		// brown	#A52A2A	165, 42, 42
		static Color brown() { return cssrgb(165, 42, 42); }
		// burlywood	#DEB887	222, 184, 135
		static Color burlywood() { return cssrgb(222, 184, 135); }
		// chocolate	#D2691E	210, 105, 30
		static Color chocolate() { return cssrgb(210, 105, 30); }

		// aquamarine	#7FFFD4	127, 255, 212
		static Color aquamarine() { return cssrgb(127, 255, 212); }
		//saddlebrown	#8B4513	139,69,19
		static Color saddlebrown() { return cssrgb(139, 69, 19); }


	};

} // namespace ray
