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
class Chapter11
{

public:
	void run() {
		std::vector<std::unique_ptr<Shape>> vec;

		{
			auto jitter = std::make_unique<Checker>(Color::blue(), Color::lightgrey());
			jitter->transform = Matrix4::scale(0.1, 0.1, 0.1);

			// background
			auto wall = std::make_unique<Plane>();
			wall->material.pattern = std::make_unique<PerlinPattern>(std::move(jitter), 5.0f);
			wall->material.pattern->transform = Matrix4::scale(0.1, 0.1, 0.1);
			wall->material.diffuse = 1;
			wall->transform = Matrix4::translate(0, 0, 10) * Matrix4::rotateX(pi / 2);
			vec.push_back(std::move(wall));
		}

		{
			// water
			auto glass = std::make_unique<Plane>();
			glass->material = Material::glass();
			glass->material.pattern = std::make_unique<Solid>(Color::aquamarine());
			glass->material.ambient = 0.3;
			glass->material.diffuse = 0.3;
			glass->material.reflective = 0.6;
			vec.push_back(std::move(glass));
			// auto dbg = std::make_unique<Plane>();
			// dbg->material.pattern = std::make_unique<Solid>(Color::red());
			// vec.push_back(std::move(dbg));
		}

		{
			// ground
			auto ground = std::make_unique<Plane>();
			ground->material.pattern = std::make_unique<Solid>(Color::beige());
			ground->transform = Matrix4::translate(0, -1.5, 0); // *Matrix4::rotateX(pi / 10);
			vec.push_back(std::move(ground));
		}

		{
			// lillies
			for (int x = 0; x < 5; x++) {
				auto lilly = std::make_unique<Sphere>();
				lilly->material.pattern = std::make_unique<Solid>(Color::green());
				lilly->material.reflective = 0.1;
				lilly->transform = Matrix4::translate(2*x-5, 0 ,4+(rand() % 10)/10.0) * Matrix4::scale(0.9, 0.1, 0.9);
				vec.push_back(std::move(lilly));
			}
		}

		{
			// rocks
			for (int x = 0; x < 10; x++) {
				auto rock = std::make_unique<Sphere>();
				rock->material.pattern = std::make_unique<Solid>(Color::chocolate());
				rock->transform = Matrix4::translate((rand() % 100) / 10.0 - 5,  -1.4, 4 + (rand() % 10) / 10.0) * Matrix4::scale(0.2, 0.1, 0.2);
				vec.push_back(std::move(rock));
			}
		}

		{
			// bush
			for (int x = 0; x < 7; x++) {
				auto t = Matrix4::translate(2*x + (rand() % 10)/10.0-7, (rand()%10)/30.0, 0);
				auto bush = std::make_unique<Sphere>();
				bush->material.pattern = std::make_unique<Solid>(Color::darksalmon());
				bush->transform = t * Matrix4::translate(0, 0, 10) * Matrix4::scale(0.5, 0.3, 0.3);
				vec.push_back(std::move(bush));
				bush = std::make_unique<Sphere>();
				bush->material.pattern = std::make_unique<Solid>(Color::darksalmon());
				bush->transform = t * Matrix4::translate(0, 0.4, 10) * Matrix4::scale(0.4, 0.2, 0.2);
				vec.push_back(std::move(bush));
			}
		}

		PointLight light(Point3(-10, 10, -10), Color(1, 1, 1));

		World world(light, std::move(vec));

		Camera camera(4000, 2000, pi / 3);
		Point3 from(0, 5, -5);
		Point3 to(0, 0, 10);
		Vec3 up(0, 1, 0);
		camera.transform = Matrix4::view(from, to, up);

		Canvas canvas = camera.render(world);

		std::ofstream ostrm("chap11.ppm", std::ios::binary);
		canvas.write_ppm(ostrm);
	}
};

