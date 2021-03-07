#pragma once
#include <cmath>

#include "Color.h"
#include "Matrix.h"

namespace ray {
	class Shape;

	class Pattern
	{
	public:
		Matrix4 transform;

		virtual bool equals(const Pattern& rhs) const = 0;

		bool operator==(const Pattern& rhs) const {
			return equals(rhs);
		}

		bool operator!=(const Pattern& rhs) const {
			return !operator==(rhs);
		}

		// C++ is hard: https://www.fluentcpp.com/2017/09/12/how-to-return-a-smart-pointer-and-use-covariance/
		virtual std::unique_ptr<Pattern> clone() const = 0;

		virtual Color pattern_at(const Point3& p) const = 0;

		// virtual so solid color can override, skipping transforms
		virtual Color pattern_at_shape(const Shape& shape, const Point3& p) const;
	};

	// Solid color pattern, not very interesting.
	class Solid : public Pattern {
	public:
		Solid() : c(Color::white()) {}
		Solid(const Color& inc) :c(inc) {}
		Solid(double r, double g, double b) :c(r,g,b) {}
		Color c;

		friend std::ostream& operator<<(std::ostream& os, const Solid& p) {
			return os << "Solid(" << p.c << ")";
		}

		bool equals(const Pattern& rhs) const override {
			const Solid* rhs_solid = dynamic_cast<const Solid*>(&rhs);
			if (rhs_solid == nullptr) {
				return false;
			}
			return c == rhs_solid->c;
		}

		bool operator==(const Solid& rhs) const {
			return equals(rhs);
		}

		bool operator!=(const Solid& rhs) const {
			return !operator==(rhs);
		}

		std::unique_ptr<Pattern> clone() const override {
			return std::make_unique<Solid>(*this);
		}

		Color pattern_at(const Point3& p) const override {
			return c;
		}

		Color pattern_at_shape(const Shape& shape, const Point3& p) const override {
			// no need to transform anything, answer is always the same.
			return c;
		}
	};

	class Stripe : public Pattern {
	public:
		Stripe(const Color& ina, const Color& inb) :a(ina), b(inb) {}
		Color a;
		Color b;

		friend std::ostream& operator<<(std::ostream& os, const Stripe& p) {
			return os << "Stripe(" << p.a << ", " << p.b << ")";
		}

		bool equals(const Pattern& rhs) const override {
			const Stripe* rhs_stripe = dynamic_cast<const Stripe*>(&rhs);
			if (rhs_stripe == nullptr) {
				return false;
			}
			return a == rhs_stripe->a && b == rhs_stripe->b;
		}

		bool operator==(const Stripe& rhs) const {
			return equals(rhs);
		}

		bool operator!=(const Stripe& rhs) const {
			return !operator==(rhs);
		}

		std::unique_ptr<Pattern> clone() const override {
			return std::make_unique<Stripe>(*this);
		}

		Color pattern_at(const Point3& p) const override {
			if ((int)floor(p.x) % 2 == 0) {
				return a;
			}
			return b;
		}
	};

};