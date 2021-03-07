#pragma once
#include "Color.h"
#include <cmath>

namespace ray {
	class Pattern
	{
	public:
		// BUGBUG: refactor - maybe Pattern, SolidPattern, StripPattern, etc..
		// For now, 123 is a sentinel to let lighting know to ignore unset patterns.
		Pattern() : a(Color(0.123,0.123,0.123)), b(Color::black()) {}

		Pattern(const Color& ina, const Color& inb) :a(ina), b(inb) {}
		Color a;
		Color b;

		friend std::ostream& operator<<(std::ostream& os, const Pattern& p) {
			return os << "Pattern(" << p.a << ", " << p.b << ")";
		}

		bool operator==(const Pattern& rhs) const {
			return a == rhs.a && b == rhs.b;
		}

		bool operator!=(const Pattern& rhs) const {
			return !operator==(rhs);
		}

		static Pattern Stripe(const Color& ina, const Color& inb) {
			return Pattern(ina, inb);
		}

		Color stripe_at(const Point3& p) const {
			if ((int)floor(p.x) % 2 == 0) {
				return a;
			}
			return b;
		}

		/*
		TODO
		Color stripe_at(const Shape& shape, const Point3& p) const {
			if ((int)floor(p.x) % 2 == 0) {
				return a;
			}
			return b;
		}
		*/
	};

};