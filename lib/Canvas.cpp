#include "pch.h"
#include "Canvas.h"
#include <array>
#include <algorithm>
#include <string>
namespace ray {
	void Canvas::write_ppm(std::ostream& s) {
		// color range
		int range = 255;
		// PPM header
		s << "P3" << std::endl;
		s << width() << " " << height() << std::endl;
		s << range << std::endl;
		// per-pixel data
		for (size_t y = 0; y < height(); y++) {
			std::string line;
			for (size_t x = 0; x < width(); x++) {
				auto& pixel = grid[x][y];
				std::array<float, 3> colors = { pixel.r, pixel.g, pixel.b };

				for (size_t i = 0; i < colors.size(); i++) {
					float c = std::clamp(colors[i], 0.0f, 1.0f);
					std::string num = std::to_string(std::lround(c * range));
					// 70 character line length limit.
					if (line.size() + 1 + num.size() > 70) {
						s << line << std::endl;
						line = num;
					}
					else
					{
						if (x != 0 || i != 0) line += " ";
						line += num;
					}
				}
			}
			// flush last part of line.
			if (line.size() > 0) {
				s << line << std::endl;
			}
		}
	}

}