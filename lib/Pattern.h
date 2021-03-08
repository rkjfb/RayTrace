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

	class TestPattern : public Pattern {
		bool equals(const Pattern& rhs) const override {
			const TestPattern* rhs_test = dynamic_cast<const TestPattern*>(&rhs);
			return rhs_test != nullptr;
		}

		std::unique_ptr<Pattern> clone() const override {
			return std::make_unique<TestPattern>(*this);
		}

		Color pattern_at(const Point3& p) const override {
			return Color(p.x, p.y, p.z);
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

	class Gradient : public Pattern {
	public:
		Gradient(const Color& ina, const Color& inb) :a(ina), b(inb) {}
		Color a;
		Color b;

		friend std::ostream& operator<<(std::ostream& os, const Gradient& p) {
			return os << "Gradient(" << p.a << ", " << p.b << ")";
		}

		bool equals(const Pattern& rhs) const override {
			const Gradient* rhs_gradient = dynamic_cast<const Gradient*>(&rhs);
			if (rhs_gradient == nullptr) {
				return false;
			}
			return a == rhs_gradient->a && b == rhs_gradient->b;
		}

		bool operator==(const Gradient& rhs) const {
			return equals(rhs);
		}

		bool operator!=(const Gradient& rhs) const {
			return !operator==(rhs);
		}

		std::unique_ptr<Pattern> clone() const override {
			return std::make_unique<Gradient>(*this);
		}

		Color pattern_at(const Point3& p) const override {
			double lerp = p.x - floor(p.x);
			return a + (b - a) * lerp;
		}
	};

	class Ring : public Pattern {
	public:
		Ring(const Color& ina, const Color& inb) :a(ina), b(inb) {}
		Color a;
		Color b;

		friend std::ostream& operator<<(std::ostream& os, const Ring& p) {
			return os << "Ring(" << p.a << ", " << p.b << ")";
		}

		bool equals(const Pattern& rhs) const override {
			const Ring* rhs_gradient = dynamic_cast<const Ring*>(&rhs);
			if (rhs_gradient == nullptr) {
				return false;
			}
			return a == rhs_gradient->a && b == rhs_gradient->b;
		}

		bool operator==(const Ring& rhs) const {
			return equals(rhs);
		}

		bool operator!=(const Ring& rhs) const {
			return !operator==(rhs);
		}

		std::unique_ptr<Pattern> clone() const override {
			return std::make_unique<Ring>(*this);
		}

		Color pattern_at(const Point3& p) const override {
			double radius = sqrt(p.x * p.x + p.z * p.z);
			if ((int)floor(radius) % 2 == 0) {
				return a;
			}
			return b;
		}
	};

	class Checker : public Pattern {
	public:
		Checker(const Color& ina, const Color& inb) :a(ina), b(inb) {}
		Color a;
		Color b;

		friend std::ostream& operator<<(std::ostream& os, const Checker& p) {
			return os << "Checker(" << p.a << ", " << p.b << ")";
		}

		bool equals(const Pattern& rhs) const override {
			const Checker* rhs_gradient = dynamic_cast<const Checker*>(&rhs);
			if (rhs_gradient == nullptr) {
				return false;
			}
			return a == rhs_gradient->a && b == rhs_gradient->b;
		}

		bool operator==(const Checker& rhs) const {
			return equals(rhs);
		}

		bool operator!=(const Checker& rhs) const {
			return !operator==(rhs);
		}

		std::unique_ptr<Pattern> clone() const override {
			return std::make_unique<Checker>(*this);
		}

		Color pattern_at(const Point3& p) const override {
			double floorsum = floor(p.x) + floor(p.y) + floor(p.z);
			if ((int)floorsum % 2 == 0) {
				return a;
			}
			return b;
		}
	};

	// Radial gradient
	class RingGradient : public Pattern {
	public:
		RingGradient(const Color& ina, const Color& inb) :a(ina), b(inb) {}
		Color a;
		Color b;

		friend std::ostream& operator<<(std::ostream& os, const RingGradient& p) {
			return os << "RingGradient(" << p.a << ", " << p.b << ")";
		}

		bool equals(const Pattern& rhs) const override {
			const RingGradient* rhs_gradient = dynamic_cast<const RingGradient*>(&rhs);
			if (rhs_gradient == nullptr) {
				return false;
			}
			return a == rhs_gradient->a && b == rhs_gradient->b;
		}

		bool operator==(const RingGradient& rhs) const {
			return equals(rhs);
		}

		bool operator!=(const RingGradient& rhs) const {
			return !operator==(rhs);
		}

		std::unique_ptr<Pattern> clone() const override {
			return std::make_unique<RingGradient>(*this);
		}

		Color pattern_at(const Point3& p) const override {
			double radius = sqrt(p.x * p.x + p.z * p.z);
			double lerp = radius - floor(radius);
			return a + (b - a) * lerp;
		}
	};

	// Joins together 2 patterns, using a third pattern's r=right and b=left channels for choosing weight of join.
	class JoinPattern : public Pattern {
	public:
		JoinPattern(std::unique_ptr<Pattern> injoin, std::unique_ptr<Pattern> inleft, std::unique_ptr<Pattern> inright) 
			: join(std::move(injoin)), left(std::move(inleft)), right(std::move(inright)) {}
		std::unique_ptr<Pattern> join;
		std::unique_ptr<Pattern> left;
		std::unique_ptr<Pattern> right;

		friend std::ostream& operator<<(std::ostream& os, const JoinPattern& p) {
			return os << "ChainJoinPattern()";
		}

		bool equals(const Pattern& rhs) const override {
			const JoinPattern* rhs_join = dynamic_cast<const JoinPattern*>(&rhs);
			if (rhs_join == nullptr) {
				return false;
			}

			return *join == *rhs_join->join && *left == *rhs_join->left && *right == *rhs_join->right;
		}

		bool operator==(const JoinPattern& rhs) const {
			return equals(rhs);
		}

		bool operator!=(const JoinPattern& rhs) const {
			return !operator==(rhs);
		}

		std::unique_ptr<Pattern> clone() const override {
			return std::make_unique<JoinPattern>(join->clone(), left->clone(), right->clone());
		}

		Color pattern_at(const Point3& p) const override {
			Color join_color = join->pattern_at(p);
			Color left_color = left->pattern_at(left->transform.inverse_multiply(p));
			Color right_color = right->pattern_at(right->transform.inverse_multiply(p));
			return left_color * join_color.b + right_color * join_color.r;
		}
	};
};