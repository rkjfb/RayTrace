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
			std::ifstream file("dragon.obj");
			Wavefront w(file);

			auto g = w.get_mega_group();
			Bounds bounds = g->bounds();
			//g->transform = Matrix4::translate(0, 0, 0) * Matrix4::scale(3, 3, 3) * Matrix4::rotateY(pi);
			Material m;
			m.pattern = std::make_unique<Solid>(Color::red());
			//g->update_material(Material::mirror());
			vec.push_back(std::move(g));
		}

		{
			std::ifstream file("teapot_normals.obj");
			Wavefront w(file);

			auto g = w.get_mega_group();
			Bounds bounds = g->bounds();
			g->transform = Matrix4::translate(-4, 0, 0) * Matrix4::scale(0.2, 0.2, 0.2) * Matrix4::rotateX(-pi / 2);
			Material m;
			m.pattern = std::make_unique<Solid>(Color::red());
			g->update_material(Material::mirror());
			//vec.push_back(std::move(g));
		}

		{
			std::ifstream file("teapot.obj");
			Wavefront w(file);

			auto g = w.get_mega_group();
			Bounds bounds = g->bounds();
			g->transform = Matrix4::translate(4, 0, 0) * Matrix4::rotateY(pi);
			Material m;
			m.pattern = std::make_unique<Solid>(Color::red());
			g->update_material(m);

			//vec.push_back(std::move(g));
		}

		for (int x = -10; x < 11; x += 5)
		{
			std::ifstream file("cow.obj");
			Wavefront w(file);

			auto g = w.get_mega_group();
			g->transform = Matrix4::translate(x, -g->bounds().min.y, 10) * Matrix4::rotateY(pi / 2);
			Material m;
			m.pattern = std::make_unique<Solid>(Color::chocolate());

			if (x == 0) {
				auto jitter = std::make_unique<Checker>(Color::beige(), Color::chocolate());
				m.pattern = std::make_unique<PerlinPattern>(std::move(jitter), 1.0f);
				m.pattern->transform = Matrix4::scale(0.1, 0.1, 0.1);
			}

			g->update_material(m);
			//vec.push_back(std::move(g));
		}

		{
			auto s = std::make_unique<Cube>();
			double scale = 25;
			s->transform = Matrix4::translate(0,scale,0) * Matrix4::scale(scale, scale, scale);
			s->material.pattern = std::make_unique<Checker>(Color::green(), Color::blanchedalmond());
			s->material.pattern->transform = Matrix4::scale(1 / scale, 1 / scale, 1 / scale);
			vec.push_back(std::move(s));
		}

		PointLight light(Point3(-5, 15, -20), Color(1, 1, 1));

		World world(light, std::move(vec));

		Camera camera(4000, 2000, pi / 3);
		Point3 from(0, 10, -15);
		Point3 to(0, 3, 0);
		Vec3 up(0, 1, 0);
		camera.transform = Matrix4::view(from, to, up);

		Canvas canvas = camera.render(world);

		std::ofstream ostrm("teapot.ppm", std::ios::binary);
		canvas.write_ppm(ostrm);
	}
};

