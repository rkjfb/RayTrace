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
#include "World.h"
#include "Camera.h"

using namespace ray;

#pragma once
class Chapter12
{

public:
	void run() {
		std::vector<std::unique_ptr<Shape>> vec;

		{
			// walls
			auto wall = std::make_unique<Cube>();
			// wall->material.pattern = std::make_unique<Checker>(Color::black(), Color::white());
			wall->material.pattern = std::make_unique<Solid>(Color::lightgrey());
			wall->material.pattern->transform = Matrix4::scale(0.2, 0.1, 0.2);
			wall->material.ambient = 0.2;
			wall->transform = Matrix4::scale(10, 20, 10);
			vec.push_back(std::move(wall));
		}

		{
			// ceiling
			auto ceiling = std::make_unique<Cube>();
			// ceiling->material.pattern = std::make_unique<RingGradient>(Color::red(), Color::darksalmon());
			ceiling->material.pattern = std::make_unique<Solid>(Color::red());
			ceiling->transform = Matrix4::scale(20, 10, 20);
			vec.push_back(std::move(ceiling));
		}


		{
			// little spheres
			for (int y = 0; y < 5; y++) {
				for (int x = 0; x < 5; x++) {
					auto sphere = std::make_unique<Sphere>();

					Matrix4 t = Matrix4::translate(2 * x - 5, 2 * y+  (rand() % 10) / 10.0 - 6, (rand() %100) / 10 + 3);
					Matrix4 r = Matrix4::rotateX(pi / (3 + rand() % 4));
					r = r * Matrix4::rotateY(pi / (3 + rand() % 4));
					r = r * Matrix4::rotateZ(pi / (3 + rand() % 4));
					sphere->transform = t * Matrix4::scale(0.5, 0.5, 0.5) * r;

					sphere->material.pattern = std::make_unique<Solid>(Color::cssrgb(rand() % 255, rand() % 255, rand() % 255));
					// sphere->material.pattern = std::make_unique<Solid>(Color::white());

					//if (rand() % 4 == 0) {
					//	sphere->material.ambient = 0.5;
					//}
					vec.push_back(std::move(sphere));
				}
			}
		}

		{
			//nest cubes
			for (int s = 1; s < 5; s++) {
				// cube
				auto cube = std::make_unique<Cube>();
				if (s%2 == 0) {
					cube->material = Material::glass();
				}
				else
				{
					cube->material = Material::air();
				}
				cube->transform = Matrix4::scale(0.4 * s, 0.4 * s, 0.4 * s);
				vec.push_back(std::move(cube));
			}
		}

		PointLight light(Point3(-9, 9, -9), Color(1, 1, 1));

		World world(light, std::move(vec));

		Camera camera(2000, 1000, pi / 3);
		Point3 from(2, 2, -8);
		// side view of cube: Point3 from(-5, 2, 0);
		Point3 to(0, 0, 0);
		Vec3 up(0, 1, 0);
		camera.transform = Matrix4::view(from, to, up);

		Canvas canvas = camera.render(world);

		std::ofstream ostrm("chap12.ppm", std::ios::binary);
		canvas.write_ppm(ostrm);
	}
};

