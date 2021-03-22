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
class Chapter13
{

public:
	void run() {
		std::vector<std::unique_ptr<Shape>> vec;

		{
			for (int i = 0; i < 2; i++) {
				// cone: use 2 cones to give a bit of a lip.
				auto cone = std::make_unique<Cone>();
				cone->maximum = 1;
				cone->minimum = 0;

				auto jitter = std::make_unique<Checker>(Color::beige(), Color::chocolate());
				//jitter->transform = Matrix4::scale(0.1, 0.1, 0.1);
				cone->material.pattern = std::make_unique<PerlinPattern>(std::move(jitter), 1.0f);
				// cone->material.pattern = std::make_unique<Checker>(Color::beige(), Color::chocolate());
				// cone->material.pattern = std::make_unique<Solid>(Color::chocolate());
				cone->material.pattern->transform = Matrix4::scale(0.1, 0.1, 0.1);
				double s = i == 0 ? 0 : 0.01;
				cone->transform = Matrix4::scale(0.5+s, 1.5, 0.5+s);
				vec.push_back(std::move(cone));
			}
		}

		{
			// ice cream
			for (int i = 0; i < 3; i++) {
				auto sphere = std::make_unique<Sphere>();
				Matrix4 center = Matrix4::translate(0.25, 1.53, 0);
				Matrix4 size = Matrix4::scale(0.25, 0.25, 0.25);
				sphere->transform = Matrix4::rotateY(i*2*pi/3) * center * size;
				sphere->material.transparency = 0.2;
				Color c = Color::white();
				switch (i) {
				case 0: c = Color::yellow(); break;
				case 1: c = Color::lime(); break;
				case 2: c = Color::darksalmon(); break;
				default: assert(false); break;
				}
				sphere->material.pattern = std::make_unique<Solid>(c);
				vec.push_back(std::move(sphere));

				// sprinkles
				for (int j = 0; j < 10; j++) {
					auto sprinkle = std::make_unique<Cylinder>();
					sprinkle->closed = true;
					sprinkle->maximum = 1;
					sprinkle->minimum = 0;
					sprinkle->material.pattern = std::make_unique<Solid>(Color::cssrgb(rand() % 255, rand() % 255, rand() % 255));
					//sprinkle->material.pattern = std::make_unique<Solid>(Color::blue());
					Vec3 offset = size * Vec3(1, 1, 1);
					Matrix4 ball_offset = Matrix4::translate(0, offset.y, 0);
					Matrix4 rot = Matrix4::rotateX(pi / 2 * (rand() % 20) / 20.0 - pi/4) * Matrix4::rotateY(pi/4+pi / 2 * (rand() % 20) / 20.0 - pi / 4) * Matrix4::rotateZ(pi / 2 * (rand() % 20) / 20.0 - pi / 4);
					sprinkle->transform = Matrix4::rotateY(i * 2 * pi / 3) * center * rot * ball_offset * Matrix4::rotateX(pi/2) * Matrix4::scale(0.005,0.05,0.005);
					vec.push_back(std::move(sprinkle));
				}
			}
		}

		{
			// choco stick thingies
			for (int i = 0; i < 2; i++) {
				auto stick = std::make_unique<Cylinder>();
				stick->closed = true;
				stick->maximum = 1;
				stick->minimum = 0;
				stick->transform = Matrix4::rotateY(i * pi / 8) * Matrix4::translate(-0.25, 1.2, 0) * Matrix4::rotateX(pi / 8) * Matrix4::rotateZ(pi / 8) * Matrix4::scale(0.05, 1, 0.05);
				stick->material.pattern = std::make_unique<Solid>(Color::saddlebrown());
				vec.push_back(std::move(stick));
			}
		}


		PointLight light(Point3(9, 14, -9), Color(1, 1, 1));

		World world(light, std::move(vec));

		Camera camera(4000, 2000, pi / 3);
		Point3 from(0, 3, -3);
		Point3 to(0, 1, 0);
		Vec3 up(0, 1, 0);
		camera.transform = Matrix4::view(from, to, up);

		Canvas canvas = camera.render(world);

		std::ofstream ostrm("chap13.ppm", std::ios::binary);
		canvas.write_ppm(ostrm);
	}
};

