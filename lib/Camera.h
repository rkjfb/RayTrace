#pragma once

#include <chrono>
#include <iostream>
#include <iomanip>
#include <atomic>
#include "Matrix.h"
#include "World.h"
#include "Canvas.h"

namespace ray {

class Camera
{
public:
	Camera(int h, int v, double field);
	Ray ray(int px, int py) const;
	void fill_column(const World* world, std::vector<std::vector<Color>>& pixels);
	Canvas render(const World& world);

public:
	int hsize;
	int vsize;
	double fov;
	double pixel_size;
	Matrix4 transform;

private:
	// shared thread state
	std::atomic_int x;

	double half_width;
	double half_height;
};

} // namespace ray
