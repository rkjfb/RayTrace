#pragma once
#include "Tuple.h"
#include "Shape.h"
namespace ray {

	// todo: create triangle mesh shape and export to it.

	class Wavefront
	{
	public:
		int ignored = 0;
		std::vector<Point3> vertices;

		Wavefront(const std::string& obj) : group(std::make_unique<Group>()) {
			// OBJ files use 1-based index, push a dummy vertex to keep numbering consistent.
			vertices.push_back(Point3(0, 0, 0));

			parse(obj);
		}

		const std::unique_ptr<Group>& default_group() {
			return group;
		}

	private:
		std::unique_ptr<Group> group;

		void parse(const std::string& obj);
	};

} // namespace ray

