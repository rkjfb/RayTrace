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

		// Moves all sub-groups into a single group and returns it to caller.
		// Destroying internal parser state in the process.
		std::unique_ptr<Group> get_mega_group() {
			auto mega = std::make_unique<Group>();

			for (auto& p : group) {
				std::unique_ptr<Group> g = std::move(p.second);
				std::vector<std::unique_ptr<Shape>> vec = g->extract_shapes();
				// Spatialize as some models might have 1000s of triangles. (eg. MIT teapot has ~6000)
				auto spatialized = NoopGroup::spatialize(std::move(vec));
				for (auto& s : spatialized) {
					mega->add(std::move(s));
				}
			}

			// not expecting parser re-use, but just in case ..
			group.clear();
			currentGroup = "default";

			return mega;
		}

	private:

		void init(std::istream& input) {
			// OBJ files use 1-based index, push a dummy vertex to keep numbering consistent.
			vertices.push_back(Point3(0, 0, 0));
			parse(input);
		}

		std::string currentGroup = "default";
		std::unordered_map<std::string, std::unique_ptr<Group>> group;
		void parse(std::istream& input);
	};

} // namespace ray

