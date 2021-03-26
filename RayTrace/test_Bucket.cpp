#include "pch.h"

#include "World.h"

using namespace ray;

// Default ctor gives us 2 shapes.
TEST(Bucket, Ctor) {
	World w;
	const auto& shapes = w.shapes();
	EXPECT_EQ(shapes.size(), 2);
}

// 10 identical shapes -> no bucket
TEST(Bucket, Duplicates) {

	std::vector<std::unique_ptr<Shape>> vec;
	for (int i = 0; i < 10; i++) {
		auto s = std::make_unique<Sphere>();
		//s->transform = Matrix4::scale(i, i, i);
		vec.push_back(std::move(s));
	}

	World w(PointLight(Point3(-10, 10, -10), Color::white()), std::move(vec));
	const auto& shapes = w.shapes();
	EXPECT_EQ(shapes.size(), 10);
}

// Simple split: half left, half right
TEST(Bucket, Simple) {

	std::vector<std::unique_ptr<Shape>> vec;
	for (int i = 0; i < 10; i++) {
		auto s = std::make_unique<Sphere>();
		s->transform = Matrix4::translate((i%2==0 ? -1 : 1) * 10, 0, 0);
		vec.push_back(std::move(s));
	}

	World w(PointLight(Point3(-10, 10, -10), Color::white()), std::move(vec));
	const auto& shapes = w.shapes();
	EXPECT_EQ(shapes.size(), 2);
}
