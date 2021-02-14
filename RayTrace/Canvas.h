#pragma once
#include "Color.h"
namespace ray {
	class Canvas
	{
	public:
		Canvas(size_t width, size_t height) {
			grid.assign(width, std::vector<Color>(height, Color(0, 0, 0)));
		}

		size_t width() const {
			return grid.size();
		}
		size_t height() const {
			return grid[0].size();
		}
		std::vector<std::vector<Color>>& pixels() {
			return grid;
		}
		void write_ppm(std::ostream& s) {
			int r = 255;
			s << "P3" << std::endl;
			s << width() << " " << height() << std::endl;
			s << r << std::endl;
		}

	private:
		std::vector<std::vector<Color>> grid;
	};

} // namespace ray