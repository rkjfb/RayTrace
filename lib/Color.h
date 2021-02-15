#pragma once

namespace ray {

	bool IsEqual(float left, float right);

	class Color
	{
	public:
		float r = 0.0f;
		float g = 0.0f;
		float b = 0.0f;

		Color(float inr, float ing, float inb) {
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
		Color operator*(float s) const {
			return Color(s * r, s * g, s * b);
		}
		static Color white() {
			return Color(1, 1, 1);
		}
		static Color red() {
			return Color(1, 0,0);
		}
	};

} // namespace ray