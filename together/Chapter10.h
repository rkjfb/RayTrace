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
class Chapter10
{

public:
	void run() {
		std::vector<std::unique_ptr<Shape>> vec;

		std::unique_ptr<Plane> floor = std::make_unique<Plane>();
		{
			auto combine = std::make_unique<Checker>(Color::red(), Color::blue());
			auto left = std::make_unique<Checker>(Color::darkred(), Color::darksalmon());
			left->transform = Matrix4::scale(0.25, 0.25, 0.25) * Matrix4::rotateY(pi / 5);
			auto right = std::make_unique<Checker>(Color::slategrey(), Color::lightgrey());
			right->transform = Matrix4::scale(0.25, 0.25, 0.25) * Matrix4::rotateY(-pi / 5);

			floor->material.pattern = std::make_unique<JoinPattern>(std::move(combine), std::move(left), std::move(right));

			vec.push_back(std::move(floor));
		}

		std::unique_ptr<Plane> ceiling = std::make_unique<Plane>();
		{
			auto combine = std::make_unique<Solid>(Color(0.5,0,0.5));
			auto left = std::make_unique<Stripe>(Color::green(), Color::white());
			auto right = left->clone();
			right->transform = Matrix4::rotateY(pi / 2);

			ceiling->material.pattern = std::make_unique<JoinPattern>(std::move(combine), std::move(left), std::move(right));
			ceiling->transform = Matrix4::translate(0, 4, 0) * Matrix4::scale(2,2,2);

			vec.push_back(std::move(ceiling));
		}

		PointLight light(Point3(0, 2, 10), Color(1, 1, 1));

		World world(light, std::move(vec));

		Camera camera(4000, 2000, pi / 3);
		Point3 from(0, 2, -1);
		Point3 to(0, 2, 0);
		Vec3 up(0, 1, 0);
		camera.transform = Matrix4::view(from, to, up);

		Canvas canvas = camera.render(world);

		std::ofstream ostrm("chap10.ppm", std::ios::binary);
		canvas.write_ppm(ostrm);
	}
};

