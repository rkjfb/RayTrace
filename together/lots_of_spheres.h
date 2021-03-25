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
			auto g = std::make_unique<Group>();
			for (int y = -range; y< range; y++) {
				for (int z = -range; z < range; z++) {
					auto sphere = std::make_unique<Sphere>();
					sphere->transform = Matrix4::translate(x, y, z);
					g->add(std::move(sphere));
				}
			}
			vec.push_back(std::move(g));
		}

		PointLight light(Point3(-10, 10, -10), Color(1, 1, 1));

		World world(light, std::move(vec));

		Camera camera(1000, 1000, pi / 3);
		Point3 from(0, 0, -10);
		Point3 to(0, 0, 0);
		Vec3 up(0, 1, 0);
		camera.transform = Matrix4::view(from, to, up);

		Canvas canvas = camera.render(world);

		std::ofstream ostrm("lots_of_spheres.ppm", std::ios::binary);
		canvas.write_ppm(ostrm);
	}
};

