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
class Chapter14
{

public:

	std::unique_ptr<Shape> hexagon_corner() {
		auto s = std::make_unique<Sphere>();
		s->transform = Matrix4::translate(0, 0, -1) * Matrix4::scale(0.25, 0.25, 0.25);
		//s->material = Material::mirror();
		return s;
	}

	std::unique_ptr<Shape> hexagon_edge() {
		auto c = std::make_unique<Cylinder>();
		c->minimum = 0;
		c->maximum = 1;
		c->transform = Matrix4::translate(0, 0, -1) * 
			Matrix4::rotateY(-pi/6)*
		Matrix4::rotateZ(-pi/2)*
			Matrix4::scale(0.25, 1, 0.25);
		//c->material = Material::mirror();
		return c;
	}

	std::unique_ptr<Shape> hexagon_side() {
		auto g = std::make_unique<Group>();
		g->add(hexagon_corner());
		g->add(hexagon_edge());
		return g;
	}

	std::unique_ptr<Shape> hexagon() {
		auto h = std::make_unique<Group>();
		for (int n = 0; n < 6; n++) {
			auto s = hexagon_side();
			s->transform = Matrix4::rotateY(n * pi / 3);
			h->add(std::move(s));
		}
		return h;
	}

	void run() {
		std::vector<std::unique_ptr<Shape>> vec;

		{
			// walls
			auto wall = std::make_unique<Cube>();
			wall->material.pattern = std::make_unique<Checker>(Color::darkorange(), Color::yellow());
			double patscale = 8;
			double scale = 30;
			wall->material.pattern->transform = Matrix4::scale(patscale/scale, patscale/scale, patscale/scale);
			wall->transform = Matrix4::scale(scale, scale, scale);
			vec.push_back(std::move(wall));
		}


		for (int n = 0; n < 24; n++) {
			auto h = hexagon();
			h->transform = Matrix4::rotateY(n * pi / 12) * Matrix4::translate(0,0,12) * Matrix4::scale(2.7,1,1) * Matrix4::rotateX(n*pi/2);
			vec.push_back(std::move(h));
		}

		{
			auto sphere = std::make_unique<Sphere>();
			sphere->material = Material::glass();
			sphere->transform = Matrix4::translate(0,0,0) * Matrix4::scale(3, 3, 3);
			vec.push_back(std::move(sphere));
		}

		PointLight light(Point3(-10, 10, -10), Color(1, 1, 1));

		World world(light, std::move(vec));

		Camera camera(4000, 2000, pi / 3);
		Point3 from(0, 8, -28);
		Point3 to(0, 1, 0);
		Vec3 up(0, 1, 0);
		camera.transform = Matrix4::view(from, to, up);

		Canvas canvas = camera.render(world);

		std::ofstream ostrm("chap14.ppm", std::ios::binary);
		canvas.write_ppm(ostrm);
	}
};

