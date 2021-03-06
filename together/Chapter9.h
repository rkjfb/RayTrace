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
class Chapter9
{

public:
	void addhex(std::vector<std::unique_ptr<Shape>>& vec) {
		for (int i = 0; i < 6; i++) {
			auto wall = std::make_unique<Plane>();
			wall->transform = Matrix4::rotateY(i * 2 * pi / 6) * Matrix4::translate(0, 0, 12) * Matrix4::rotateX(pi / 2);
			wall->material.color = Color(.5, 0.9f, 0.9f);
			wall->material.specular = 0;
			vec.push_back(std::move(wall));
		}
	}

	void run() {
		std::unique_ptr<Plane> floor = std::make_unique<Plane>();
		floor->material.color = Color(1, 0.9f, 0.9f);
		floor->material.specular = 0;

		std::unique_ptr<Sphere> middle = std::make_unique<Sphere>();
		middle->transform = Matrix4::translate(-0.5, .5, 0.5f);
		middle->material.color = Color(0.1f, 1, 0.5f);
		middle->material.diffuse = 0.7f;
		middle->material.specular = 0.3f;

		std::unique_ptr<Sphere> right = std::make_unique<Sphere>();
		right->transform = Matrix4::translate(1.5f, 0.5f, -0.5f) * Matrix4::scale(0.5f, 0.5f, 0.5f);
		right->material.color = Color(0.5f, 1, 0.1f);
		right->material.diffuse = 0.7f;
		right->material.specular = 0.3f;

		std::unique_ptr<Sphere> left = std::make_unique<Sphere>();
		left->transform = Matrix4::translate(-1.5f, 0.333f, -0.75f) * Matrix4::scale(0.333f, 0.333f, 0.333f);
		left->material.color = Color(1, 0.8f, 0.1f);
		left->material.diffuse = 0.7f;
		left->material.specular = 0.3f;

		std::vector<std::unique_ptr<Shape>> vec;
		vec.push_back(std::move(floor));
		vec.push_back(std::move(middle));
		vec.push_back(std::move(right));
		vec.push_back(std::move(left));

		addhex(vec);

		PointLight light(Point3(-7, 7, -7), Color(1, 1, 1));

		World world(light, std::move(vec));

		Camera camera(3840, 2160, pi / 3);
		Point3 from(0, 8, -1);
		Point3 to(0, 1, 0);
		Vec3 up(0, 1, 0);
		camera.transform = Matrix4::view(from, to, up);

		Canvas canvas = camera.render(world);

		std::ofstream ostrm("chap9b.ppm", std::ios::binary);
		canvas.write_ppm(ostrm);
	}
};

