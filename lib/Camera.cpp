#include "pch.h"
#include <chrono>

#include "Camera.h"


using namespace ray;

Camera::Camera(int h, int v, double field) :
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

Ray Camera::ray(int px, int py) const {
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


void Camera::fill_column(const World* world, std::vector<std::vector<Color>>& pixels) {
	for (;;) {
		int localx = x.fetch_add(1);
		if (localx >= hsize) {
			return;
		}

		for (int y = 0; y < vsize; y++) {
			Ray r = ray(localx, y);
			Color c = world->color_at(r,8);
			pixels[localx][y] = c;
		}
	}
}

Canvas Camera::render(const World& world) {
	auto start = std::chrono::steady_clock::now();
	Canvas image(hsize, vsize);

	x.store(0);
	std::vector<std::thread> threads;
	// for (uint32_t i = 0; i < 1; i++) {
	for (uint32_t i = 0; i < std::thread::hardware_concurrency(); i++) {
			threads.emplace_back(std::thread([&] { fill_column(&world, image.pixels()); }));
	}

	while (x < hsize) {
		std::chrono::milliseconds d(200);
		std::this_thread::sleep_for(d);
		std::cout << floor(100 * x / (double)hsize) << "%" << std::endl;
	}

	for (auto& t : threads) {
		t.join();
	}

	auto end = std::chrono::steady_clock::now();
	std::chrono::duration<double> durdiff = end - start;
	double diff = durdiff.count();
	double megapixels_per_sec = hsize * vsize / diff / 1000000;
	std::cout << std::setw(9) << hsize << "x" << vsize << " render time: " << diff << "s " << megapixels_per_sec << " megapixels/sec" << std::endl;

	return image;
}
