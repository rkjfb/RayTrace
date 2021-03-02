#pragma once

#include "Matrix.h"
#include "World.h"
#include "Canvas.h"

namespace ray {

class Camera
{
public:
	Camera(int h, int v, float field) :
		hsize(h), vsize(v), fov(field) {

		float half_view = tanf(field / 2.0f);
		float aspect = (float)h / v;
		
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
		float x = (px + 0.5f) * pixel_size;
		float y = (py + 0.5f) * pixel_size;

		// pixel in world space. note that camera is facing -z, so +x is left.
		float wx = half_width - x;
		float wy = half_height - y;

		Matrix4 inv = transform.inverse();
		Point3 pixel = inv * Point3(wx, wy, -1);
		Point3 origin = inv * Point3(0, 0, 0);
		Vec3 direction = (pixel - origin).norm();

		return Ray(origin, direction);
	}

	Canvas render(const World& world) {
		Canvas image(hsize, vsize);
		auto& pixels = image.pixels();
		std::vector<Intersection> intersections;

		for (int y = 0; y < vsize; y++) {
			for (int x = 0; x < hsize; x++) {
				Ray r = ray(x, y);
				Color c = world.color_at(r, intersections);
				pixels[x][y] = c;
			}
		}

		return image;
	}

	int hsize;
	float half_width;
	int vsize;
	float half_height;
	float fov;
	Matrix4 transform;
	float pixel_size;
};

} // namespace ray
