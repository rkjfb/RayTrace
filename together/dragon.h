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
#include "PlyLoader.h"

using namespace ray;

#pragma once
class Dragon
{

public:

	void run() {
		std::vector<std::unique_ptr<Shape>> vec;

		{
			PlyLoader ply("..\\models\\bunny\\bun_zipper_res2.ply");

			auto g = ply.get_mega_group();
			Bounds bounds = g->bounds();
			std::cout << bounds << std::endl;
			//g->transform = Matrix4::translate(0, 0, 0) * Matrix4::scale(1, 1, 1) * Matrix4::rotateY(pi);
			Material m;
			m.pattern = std::make_unique<Solid>(Color::red());
			m.ambient = 0.5;
			g->update_material(m);
			//g->update_material(Material::mirror());
			vec.push_back(std::move(g));
		}

		{
			// background/floor.
			auto s = std::make_unique<Cube>();
			double scale = 20;
			s->transform = Matrix4::translate(0,0,0) * Matrix4::scale(scale, scale, scale);
			s->material.pattern = std::make_unique<Checker>(Color::green(), Color::blanchedalmond());
			s->material.pattern->transform = Matrix4::scale(1 / scale, 1 / scale, 1 / scale);
			//vec.push_back(std::move(s));
		}

		PointLight light(Point3(-10, 10, -10), Color(1, 1, 1));

		World world(light, std::move(vec));

		Camera camera(4000, 2000, pi / 3);
		Point3 from(0, 1, -1);
		Point3 to(0, 0, 0);
		Vec3 up(0, 1, 0);
		camera.transform = Matrix4::view(from, to, up);

		Canvas canvas = camera.render(world);

		std::ofstream ostrm("dragon.ppm", std::ios::binary);
		canvas.write_ppm(ostrm);
	}
};

