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

class Chapter6
{
public:
	void run() {
		int width = 900;
		int height = 550;
		ray::Canvas canvas(width, height);
		auto& pixels = canvas.pixels();

		ray::Sphere sphere;
		sphere.material.pattern = std::make_unique<ray::Solid>(1, 0.2, 1);

		ray::PointLight light(ray::Point3(-10, 10, -10), ray::Color(1, 1, 1));

		std::vector<ray::Intersection> intersections;
		for (int x = 0; x < width; x++) {
			float flx = 2.0f * x / width - 1.0f;
			for (int y = 0; y < height; y++) {
				float fly = 2.0f * y / height - 1.0f;

				ray::Ray r(ray::Point3(flx, fly, 1), ray::Vec3(0, 0, -1));
				sphere.intersect(r, intersections);
				if (intersections.size() > 0) {
					const ray::Intersection* i = ray::Intersection::hit(intersections);
					ray::Point3 point = r.position(i->t);
					ray::Vec3 normal = sphere.normal_at(point);
					ray::Color c = sphere.material.lighting(light, sphere, point, -r.direction, normal, false);
					pixels[x][y] = c;
				}
			}
		}

		std::ofstream ostrm("chap6.ppm", std::ios::binary);
		canvas.write_ppm(ostrm);

	}
};

