#pragma once

#include <chrono>
#include <iostream>
#include <iomanip>
#include "Matrix.h"
#include "World.h"
#include "Canvas.h"

namespace ray {

class Camera
{
public:
	Camera(int h, int v, double field) :
		hsize(h), vsize(v), fov(field) {

		double half_view = tan(field / 2.0f);
		double aspect = (double)h / v;
		
		if (aspect >= 1) {
			half_width = half_view;
			half_height = half_view / aspect;
		}
		else {
			half_width = half_view * aspect;
			half_height = half_view;
		}

		pixel_size = half_width * 2 / hsize;
	}

	Ray ray(int px, int py) const {
		// offset from edge of canvas to center of pixel
		double x = (px + 0.5f) * pixel_size;
		double y = (py + 0.5f) * pixel_size;

		// pixel in world space. note that camera is facing -z, so +x is left.
		double wx = half_width - x;
		double wy = half_height - y;

		Matrix4 inv = transform.inverse();
		Point3 pixel = inv * Point3(wx, wy, -1);
		Point3 origin = inv * Point3(0, 0, 0);
		Vec3 direction = (pixel - origin).norm();

		return Ray(origin, direction);
	}

	Canvas render(const World& world) {
		auto start = std::chrono::steady_clock::now();
		Canvas image(hsize, vsize);
		auto& pixels = image.pixels();

		for (int y = 0; y < vsize; y++) {
			for (int x = 0; x < hsize; x++) {
				Ray r = ray(x, y);
				Color c = world.color_at(r);
				pixels[x][y] = c;
			}
		}
		auto end = std::chrono::steady_clock::now();
		std::chrono::duration<double> durdiff = end - start;
		double diff = durdiff.count();
		double megapixels_per_sec = hsize * vsize / diff / 1000000;
		std::cout << std::setw(9) << hsize << "x" << vsize <<" render time: " << diff << "s " << megapixels_per_sec << " megapixels/sec" << std::endl;

		return image;
	}

	int hsize;
	double half_width;
	int vsize;
	double half_height;
	double fov;
	Matrix4 transform;
	double pixel_size;
};

} // namespace ray
