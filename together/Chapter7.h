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
class Chapter7
{

public:
	void run() {
		auto floor = std::make_unique<Sphere>();
		floor->transform = Matrix4::scale(10, 0.01f, 10);
		floor->material.pattern = std::make_unique<Solid>(Color::black());
		//floor->material.specular = 0;
		floor->material.reflective = 1;

		auto left_wall = std::make_unique<Sphere>();
		left_wall->transform =
			Matrix4::translate(0, 0, 5) *
			Matrix4::rotateY(-pi / 4) *
			Matrix4::rotateX(pi / 2) *
			Matrix4::scale(10, 0.01f, 10);
		left_wall->material = floor->material;

		auto right_wall = std::make_unique<Sphere>();
		right_wall->transform =
			Matrix4::translate(0, 0, 5) *
			Matrix4::rotateY(pi / 4) *
			Matrix4::rotateX(pi / 2) *
			Matrix4::scale(10, 0.01f, 10);
		right_wall->material = floor->material;

		auto middle = std::make_unique<Sphere>();
		middle->transform = Matrix4::translate(-0.5, 1, 0.5f);
		middle->material.pattern = std::make_unique<Ring>(Color::lime(), Color::red());
		middle->material.pattern->transform = Matrix4::scale(0.1, 0.1, 0.1);
		middle->material.diffuse = 0.7f;
		middle->material.specular = 0.3f;
		middle->material.reflective = 1;

		auto right = std::make_unique<Sphere>();
		right->transform = Matrix4::translate(1.5f, 0.5f, -0.5f);// *Matrix4::scale(0.5f, 0.5f, 0.5f);
		right->material.pattern = std::make_unique<Gradient>(Color::blue(), Color::white());
		right->material.pattern->transform = Matrix4::scale(0.1, 0.1, 0.1);
		right->material.diffuse = 0.7f;
		right->material.specular = 0.3f;
		right->material.reflective = 1;


		auto back_right = std::make_unique<Sphere>();
		back_right->transform = Matrix4::translate(1.5f, 1.5f, 1.5f);// *Matrix4::scale(0.5f, 0.5f, 0.5f);
		back_right->material.pattern = std::make_unique<Checker>(Color::blue(), Color::lime());
		back_right->material.pattern->transform = Matrix4::scale(0.1, 0.1, 0.1);
		back_right->material.diffuse = 0.7f;
		back_right->material.specular = 0.3f;

		auto left = std::make_unique<Sphere>();
		left->transform = Matrix4::translate(-1.5f, 0.333f, -0.75f); // *Matrix4::scale(0.333f, 0.333f, 0.333f);
		left->material.pattern = std::make_unique<RingGradient>(Color::lime(), Color::red());
		left->material.pattern->transform = Matrix4::scale(0.1, 0.1, 0.1);
		left->material.diffuse = 0.7f;
		left->material.specular = 0.3f;
		left->material.reflective = 1;

		std::vector<std::unique_ptr<Shape>> vec;
		vec.push_back(std::move(floor));
		vec.push_back(std::move(left_wall));
		vec.push_back(std::move(right_wall));
		vec.push_back(std::move(middle));
		vec.push_back(std::move(right));
		//vec.push_back(std::move(back_right));
		vec.push_back(std::move(left));

		PointLight light(Point3(-10, 10, -10), Color(1, 1, 1));

		World world(light, std::move(vec));

		Camera camera(2000, 1000, pi / 3);
		Point3 from(0, 1.5, -10);
		Point3 to(0, 1, 0);
		Vec3 up(0, 1, 0);
		camera.transform = Matrix4::view(from, to, up);

		Canvas canvas = camera.render(world);

		std::ofstream ostrm("chap7b.ppm", std::ios::binary);
		canvas.write_ppm(ostrm);
	}
};

