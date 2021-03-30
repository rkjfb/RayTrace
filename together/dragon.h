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
			PlyLoader ply("..\\models\\spider.ply");

			auto g = ply.get_mega_group();
			Bounds bounds = g->bounds();
			std::cout << bounds << std::endl;
			Point3 c = bounds.center();
			Matrix4 centering = Matrix4::translate(-c.x, -c.y, -c.z);
			g->transform = Matrix4::translate(2, 0, 0) * Matrix4::scale(1, 1, 1) * Matrix4::rotateY(-pi/3)* Matrix4::rotateX(-pi/2) * centering;
			Material m;
			auto jitter = std::make_unique<RingGradient>(Color::yellow(), Color::red());
			m.pattern = std::make_unique<PerlinPattern>(std::move(jitter), 1.0f);
			//m.pattern->transform = Matrix4::scale(0.1, 0.1, 0.1);

			//m.pattern = std::make_unique<Solid>(Color::red());
			g->update_material(m);
			vec.push_back(std::move(g));
		}

		{
			// origin
			auto s = std::make_unique<Sphere>();
			s->material = Material::glass();
			s->transform = Matrix4::translate(-3, 1, 0); // *Matrix4::scale(5, 3, 0.1);
			//vec.push_back(std::move(s));
		}
		{
			// mirror
			auto s = std::make_unique<Cube>();
			s->material = Material::mirror();
			s->transform = Matrix4::translate(0, 0, 5) * Matrix4::scale(5, 3, 0.1);
			vec.push_back(std::move(s));

			s = std::make_unique<Cube>();
			s->material.pattern = std::make_unique<Solid>(Color::chocolate());
			s->transform = Matrix4::translate(0, 0, 5) * Matrix4::scale(5.1, 3.1, 0.01);
			vec.push_back(std::move(s));
		}


		{
			// background/floor.
			auto s = std::make_unique<Cube>();
			double scale = 20;
			s->transform = Matrix4::translate(0,scale,0) * Matrix4::scale(scale, scale, scale);
			s->material.pattern = std::make_unique<Stripe>(Color::darkolivegreen(), Color::midnightblue());
			s->material.pattern->transform = Matrix4::scale(1 / scale, 1 / scale, 1 / scale);
			vec.push_back(std::move(s));
		}

		PointLight light(Point3(-10, 10, -10), Color(1, 1, 1));

		World world(light, std::move(vec));

		Camera camera(4000, 2000, pi / 3);
		Point3 from(0, 5, -10);
		Point3 to(0, 0, 0);
		Vec3 up(0, 1, 0);
		camera.transform = Matrix4::view(from, to, up);

		Canvas canvas = camera.render(world);

		std::ofstream ostrm("dragon.ppm", std::ios::binary);
		canvas.write_ppm(ostrm);
	}
};

