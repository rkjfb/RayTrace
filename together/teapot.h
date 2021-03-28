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
		std::ifstream file("teapot.obj");
		Wavefront w(file);

		std::vector<std::unique_ptr<Shape>> vec;
		vec.push_back(w.get_mega_group());

		{
			auto s = std::make_unique<Cube>();
			double scale = 20;
			s->transform = Matrix4::scale(scale, scale, scale);
			s->material.pattern = std::make_unique<Checker>(Color::darkorange(), Color::yellow());
			s->material.pattern->transform = Matrix4::scale(1 / scale, 1 / scale, 1 / scale);
			vec.push_back(std::move(s));
		}

		PointLight light(Point3(-10, 10, -10), Color(1, 1, 1));

		World world(light, std::move(vec));

		Camera camera(4000, 2000, pi / 3);
		Point3 from(0, 10, -10);
		Point3 to(0, 0, 0);
		Vec3 up(0, 1, 0);
		camera.transform = Matrix4::view(from, to, up);

		Canvas canvas = camera.render(world);

		std::ofstream ostrm("teapot.ppm", std::ios::binary);
		canvas.write_ppm(ostrm);
	}
};

