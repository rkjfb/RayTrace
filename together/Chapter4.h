#pragma once
#include <ostream>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "Tuple.h"
#include "Canvas.h"
#include "Matrix.h"
class Chapter4
{
public:
	void run() {

		float armlength = 200.0f;
		float rot = 2 * ray::pi / 12;
		int width = 900;
		int height = 550;
		ray::Canvas canvas(width, height);
		auto& pixels = canvas.pixels();

		for (int i = 0; i < 12; i++) {

			ray::Matrix4 armtx = ray::Matrix4::translate(0, armlength, 0);
			ray::Matrix4 clockrot = ray::Matrix4::rotateZ(rot * i);
			ray::Matrix4 centertx = ray::Matrix4::translate(static_cast<float>(width / 2), static_cast<float>(height / 2), 0);

			ray::Point3 p;
			ray::Point3 position = centertx * clockrot * armtx * p;

			size_t x = std::lround(position.x);
			size_t zero = 0;
			x = std::clamp(x, zero, canvas.width());
			size_t y = canvas.height() - std::lround(position.y);
			y = std::clamp(y, zero, canvas.height());
			pixels[x][y] = ray::Color::white();
			std::cout << position << std::endl;
		}

		std::ofstream ostrm("chap4.ppm", std::ios::binary);
		canvas.write_ppm(ostrm);

	}
};

