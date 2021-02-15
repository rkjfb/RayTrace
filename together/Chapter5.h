#pragma once
#include <ostream>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "Tuple.h"
#include "Canvas.h"
#include "Matrix.h"
#include "Shape.h"
#include "Intersect.h"

class Chapter5
{
public:
	void run() {
		int width = 900;
		int height = 550;
		ray::Canvas canvas(width, height);
		auto& pixels = canvas.pixels();

		ray::Sphere sphere;
		std::vector<ray::Intersection> intersections;
		for (int x = 0; x < width; x++) {
			float flx = 2.0f * x / width - 1.0f;
			for (int y = 0; y < height; y++) {
				float fly = 2.0f * y / height - 1.0f;

				ray::Ray r(ray::Point3(flx, fly, 1), ray::Vec3(0, 0, -1));
				ray::Intersection::intersect(sphere, r, intersections);
				if (intersections.size() > 0) {
					pixels[x][y] = ray::Color::red();
				}
			}
		}

		std::ofstream ostrm("chap5.ppm", std::ios::binary);
		canvas.write_ppm(ostrm);

	}
};

