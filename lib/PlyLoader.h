#pragma once
#include "Tuple.h"
#include "Shape.h"

namespace ray {

	class PlyLoader
	{
	public:
		PlyLoader(const std::string& filename);
		std::unique_ptr<Group> get_mega_group() {
			auto mega = std::make_unique<Group>();
			for (auto& s : shape) {
				mega->add(std::move(s));
			}
			shape.clear();
			return mega;
		}

	private:

		std::vector<std::unique_ptr<Shape>> shape;
	};

} // namespace ray

