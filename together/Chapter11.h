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
			auto wall = std::make_unique<Plane>();
			wall->material.pattern = std::make_unique<Checker>(Color::black(), Color::white());
			wall->material.diffuse = 1;
			wall->transform = Matrix4::translate(0, 0, 10) * Matrix4::rotateX(pi / 2);
			vec.push_back(std::move(wall));
		}

		{
			auto glass = Sphere::glass();
			vec.push_back(std::move(glass));
		}

		{
			auto air = Sphere::air();
			air->material.pattern = std::make_unique<Solid>(Color::black());
			air->transform = Matrix4::scale(0.5, 0.5, 0.5);
			vec.push_back(std::move(air));
		}

		PointLight light(Point3(-10, 10, -10), Color(1, 1, 1));

		World world(light, std::move(vec));

		Camera camera(2000, 1000, pi / 3);
		Point3 from(0, 0, -4);
		Point3 to(0, 0, 0);
		Vec3 up(0, 1, 0);
		camera.transform = Matrix4::view(from, to, up);

		Canvas canvas = camera.render(world);

		std::ofstream ostrm("chap11.ppm", std::ios::binary);
		canvas.write_ppm(ostrm);
	}
};

