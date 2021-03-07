#include "pch.h"
#include "World.h"

using namespace ray;

// only light: get 2 stock shapes
World::World(const PointLight& light) :
	_light(light)
{
	Material m;
	m.pattern = std::make_unique<Solid>(0.8, 1, 0.6);
	m.diffuse = 0.7f;
	m.specular = 0.2f;

	auto s1 = std::make_unique<Sphere>();
	s1->material = m;
	_shapes.emplace_back(std::move(s1));

	auto s2 = std::make_unique<Sphere>(Matrix4::scale(0.5f, 0.5f, 0.5f));
	_shapes.emplace_back(std::move(s2));
}

