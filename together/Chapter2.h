#pragma once
#include <ostream>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "Tuple.h"
#include "Canvas.h"
class Chapter2
{
public:
	void run() {
		position = ray::Point3(0, 1, 0);
		velocity = ray::Vec3(1, 1.8f, 0).norm() * 11.25f;
		gravity = ray::Vec3(0, -0.1f, 0);
		wind = ray::Vec3(-0.01f, 0, 0);

		ray::Canvas canvas(900, 550);
		auto& pixels = canvas.pixels();

		while (position.y > 0.0f) {
			size_t x = std::lround(position.x);
			size_t zero = 0;
			x = std::clamp(x, zero, canvas.width());
			size_t y = canvas.height() - std::lround(position.y);
			y = std::clamp(y, zero, canvas.height());
			pixels[x][y] = ray::Color::white();
			std::cout << position << std::endl;
			tick();
		}

		std::ofstream ostrm("chap2.ppm", std::ios::binary);
		canvas.write_ppm(ostrm);

	}
	void tick() {
		position = position + velocity;
		velocity = velocity + gravity + wind;
	}

	ray::Point3 position;
	ray::Vec3 velocity;
	ray::Vec3 gravity;
	ray::Vec3 wind;
};

