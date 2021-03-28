#pragma once
#include <fstream>
#include <unordered_map>
#include "Tuple.h"
#include "Shape.h"

namespace ray {

	// todo: create triangle mesh shape and export to it.

	class Wavefront
	{
	public:
		int ignored = 0;
		std::vector<Point3> vertices;

		Wavefront(std::ifstream& file) {
			init(file);
		}

		Wavefront(const std::string& obj) {
			std::stringstream ss(obj);
			init(ss);
		}

		const std::unique_ptr<Group>& named_group(const std::string& name) {
			return group[name];
		}

		const std::unique_ptr<Group>& default_group() {
			return group["default"];
		}

	private:

		void init(std::istream& input) {
			group[currentGroup] = std::make_unique<Group>();
			// OBJ files use 1-based index, push a dummy vertex to keep numbering consistent.
			vertices.push_back(Point3(0, 0, 0));
			parse(input);
		}

		std::string currentGroup = "default";
		std::unordered_map<std::string, std::unique_ptr<Group>> group;
		void parse(std::istream& input);
	};

} // namespace ray

