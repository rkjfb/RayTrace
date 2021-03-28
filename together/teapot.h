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

		PointLight light(Point3(-10, 10, -10), Color(1, 1, 1));

		World world(light, std::move(vec));

		Camera camera(200, 100, pi / 3);
		Point3 from(5, 2, -5);
		Point3 to(0, 0, 0);
		Vec3 up(0, 1, 0);
		camera.transform = Matrix4::view(from, to, up);

		Canvas canvas = camera.render(world);

		std::ofstream ostrm("teapot.ppm", std::ios::binary);
		canvas.write_ppm(ostrm);
	}
};

