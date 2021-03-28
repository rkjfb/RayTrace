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
#include "Wavefront.h"

using namespace ray;

#pragma once
class Teapot
{

public:

	void run() {
		std::vector<std::unique_ptr<Shape>> vec;
		{
			std::ifstream file("cow.obj");
			Wavefront w(file);

			auto g = w.get_mega_group();
			g->transform = Matrix4::translate(0,-g->bounds().min.y, 0) * Matrix4::rotateY(pi / 2);
			vec.push_back(std::move(g));
		}

		{
			auto s = std::make_unique<Cube>();
			double scale = 20;
			s->transform = Matrix4::translate(0,scale,0) * Matrix4::scale(scale, scale, scale);
			s->material.pattern = std::make_unique<Checker>(Color::green(), Color::blanchedalmond());
			s->material.pattern->transform = Matrix4::scale(1 / scale, 1 / scale, 1 / scale);
			vec.push_back(std::move(s));
		}

		PointLight light(Point3(-10, 10, -10), Color(1, 1, 1));

		World world(light, std::move(vec));

		Camera camera(4000, 2000, pi / 3);
		Point3 from(-8, 10, -15 );
		Point3 to(0, 3, 0);
		Vec3 up(0, 1, 0);
		camera.transform = Matrix4::view(from, to, up);

		Canvas canvas = camera.render(world);

		std::ofstream ostrm("teapot.ppm", std::ios::binary);
		canvas.write_ppm(ostrm);
	}
};

