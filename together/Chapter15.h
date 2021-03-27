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
class Chapter15
{

public:


	void run() {
		std::vector<std::unique_ptr<Shape>> vec;

		{
			// triangle
			auto t = std::make_unique<Triangle>(Point3(), Point3(1,1,0), Point3(2,0,0));
			vec.push_back(std::move(t));
		}

		PointLight light(Point3(-10, 10, -10), Color(1, 1, 1));

		World world(light, std::move(vec));

		Camera camera(4000, 2000, pi / 3);
		Point3 from(0, 8, -28);
		Point3 to(0, 1, 0);
		Vec3 up(0, 1, 0);
		camera.transform = Matrix4::view(from, to, up);

		Canvas canvas = camera.render(world);

		std::ofstream ostrm("chap15.ppm", std::ios::binary);
		canvas.write_ppm(ostrm);
	}
};

