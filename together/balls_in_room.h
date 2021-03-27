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
class BallsInRoom
{

public:


	void run() {
		std::vector<std::unique_ptr<Shape>> vec;

        auto floor = std::make_unique<Plane>();
        {
            floor->material.pattern = std::make_unique<Checker>(Color(1, 0.9, 0.9), Color::black());
            floor->material.ambient = 0.1;
            floor->material.diffuse = 0.9;
            floor->material.specular = 0.9;
            floor->material.shininess = 200.0;
        }

        {
            auto left = std::make_unique<Plane>();
            left->transform = Matrix4::translate(0, 0, 5) * Matrix4::rotateY(-pi / 4) * Matrix4::rotateX(pi / 2);
            left->material = floor->material;
            vec.push_back(std::move(left));
        }

        {
            auto right = std::make_unique<Plane>();
            right->transform = Matrix4::translate(0, 0, 5) * Matrix4::rotateY(pi / 4) * Matrix4::rotateX(pi / 2);
            right->material = floor->material;
            vec.push_back(std::move(right));
        }

        {
            auto back = std::make_unique<Plane>();
            back->transform = Matrix4::translate(0, 0, -15) * Matrix4::rotateX(-pi / 2);
            back->material = floor->material;
            vec.push_back(std::move(back));
        }

        {
            auto large = std::make_unique<Sphere>();
            large->transform = Matrix4::translate(-0.5, 1, 0.5);
            large->material = Material::mirror();
            /*
            large->material.pattern = std::make_unique<Solid>(Color(0.1, 0.1, 0.1));
            large->material.ambient = 0.1;
            large->material.diffuse = 0.7;
            large->material.reflective = 0.9;
            large->material.specular = 0.3;
            large->material.shininess = 200.0;
            */
            vec.push_back(std::move(large));
        }

        {
            auto medium = std::make_unique<Sphere>();
            medium->transform = Matrix4::translate(1.5, 0.5, -0.5) * Matrix4::scale(0.5, 0.5, 0.5);
            medium->material.pattern = std::make_unique<Solid>(Color(0.5, 1, 0.1));
            medium->material.ambient = 0.1;
            medium->material.diffuse = 0.7;
            medium->material.specular = 0.3;
            medium->material.shininess = 200.0;
            vec.push_back(std::move(medium));
        }

        {
            auto small = std::make_unique<Sphere>();
            small->transform = Matrix4::translate(-1.5, 0.33, -0.75) * Matrix4::scale(0.5, 0.5, 0.5);
            small->material.pattern = std::make_unique<Solid>(Color(0.1, 0.1, 0.1));
            small->material.ambient = 0.1;
            small->material.diffuse = 0.7;
            small->material.reflective = 1;
            small->material.specular = 0.3;
            small->material.shininess = 200.0;
            small->material.transparency = 1;
            small->material.refractive_index = 2.47;
            vec.push_back(std::move(small));
        }

        {
            auto bubble = std::make_unique<Sphere>();
            bubble->transform = Matrix4::translate(-1.5, 0.33, -0.75) * Matrix4::scale(0.25, 0.25, 0.25);
            bubble->material.pattern = std::make_unique<Solid>(Color::black());
            bubble->material.ambient = 1;
            bubble->material.diffuse = 0;
            bubble->material.reflective = 1;
            bubble->material.specular = 0.95;
            bubble->material.shininess = 300.0;
            bubble->material.transparency = 1;
            bubble->material.refractive_index = 1.0029;
            vec.push_back(std::move(bubble));
        }

        vec.push_back(std::move(floor));

		PointLight light(Point3(-10, 10, -10), Color(1, 1, 1));

		World world(light, std::move(vec));

		Camera camera(4000, 2000, pi / 3);
		Point3 from(0, 1.5, -5);
		Point3 to(0, 1, 0);
		Vec3 up(0, 1, 0);
		camera.transform = Matrix4::view(from, to, up);

		Canvas canvas = camera.render(world);

		std::ofstream ostrm("balls_in_room.ppm", std::ios::binary);
		canvas.write_ppm(ostrm);
	}
};

