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
		auto join_pattern = std::make_unique<Checker>(Color::red(), Color::blue());
		auto left = std::make_unique<Checker>(Color::darkred(), Color::darksalmon());
		left->transform = Matrix4::scale(0.25, 0.25, 0.25) * Matrix4::rotateY(pi/5);
		auto right = std::make_unique<Checker>(Color::slategrey(), Color::lightgrey());
		right->transform = Matrix4::scale(0.25, 0.25, 0.25) * Matrix4::rotateY(-pi / 5);

		std::unique_ptr<JoinPattern> pattern = std::make_unique<JoinPattern>(std::move(join_pattern), std::move(left), std::move(right));

		std::unique_ptr<Plane> floor = std::make_unique<Plane>();
		floor->material.pattern = std::move(pattern);
		floor->material.specular = 0;

/*
		std::unique_ptr<Sphere> middle = std::make_unique<Sphere>();
		middle->transform = Matrix4::translate(-0.5, .5, 0.5f);
		middle->material.pattern = std::make_unique<Gradient>(Color::red(), Color::lime());
		middle->material.diffuse = 0.7f;
		middle->material.specular = 0.3f;
		*/

		std::vector<std::unique_ptr<Shape>> vec;
		vec.push_back(std::move(floor));

		PointLight light(Point3(-10, 10, -10), Color(1, 1, 1));

		World world(light, std::move(vec));

		Camera camera(4000, 2000, pi / 3);
		Point3 from(0, 2, -1);
		Point3 to(0, 1.9, 0);
		Vec3 up(0, 1, 0);
		camera.transform = Matrix4::view(from, to, up);

		Canvas canvas = camera.render(world);

		std::ofstream ostrm("chap10.ppm", std::ios::binary);
		canvas.write_ppm(ostrm);
	}
};

