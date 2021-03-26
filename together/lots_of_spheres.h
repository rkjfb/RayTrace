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
class LotsSpheres
{

public:


	void run() {
		std::vector<std::unique_ptr<Shape>> vec;

		int range = 5;
		int step = 1;
		for (int x = -range; x < range; x++) {
			auto gx = std::make_unique<Group>();
			for (int y = -range; y< range; y++) {
				auto gy = std::make_unique<Group>();
				for (int z = -range; z < range; z++) {
					auto sphere = std::make_unique<Sphere>();
					sphere->transform = Matrix4::translate(x+0.5, y+0.5, z+0.5) * Matrix4::scale(0.25,0.25,0.25);
					sphere->material = Material::glass();
					sphere->material.ambient = 0.1;
					sphere->material.diffuse = 0.5;
					sphere->material.specular = 0.5;
					sphere->material.pattern = std::make_unique<Solid>(Color::cssrgb(150 + 50 * x, 150 + 50 * y, 150 + 50 * z));
					vec.push_back(std::move(sphere));
				}
			}
		}

		PointLight light(Point3(-10, 10, -10), Color(1, 1, 1));

		World world(light, std::move(vec));

		Camera camera(4000, 2000, pi / 3);
		Point3 from(0, 0, -22);
		Point3 to(0, 0, 0);
		Vec3 up(0, 1, 0);
		camera.transform = Matrix4::view(from, to, up);

		Canvas canvas = camera.render(world);

		std::ofstream ostrm("lots_of_spheres.ppm", std::ios::binary);
		canvas.write_ppm(ostrm);
	}
};

