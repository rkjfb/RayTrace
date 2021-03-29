#include "pch.h"
#include <sstream>
#include <algorithm>
#include <regex>

#include "Wavefront.h"

using namespace ray;

std::vector<double> Wavefront::parseDoubles(const std::string& line) {
	std::regex reNumber(R"REGEX(\s*(-?\d*\.?\d+))REGEX");
	std::vector<double> vec;
	// C++ regex doesn't allow repeat captures, so we have to iterate to get subsequent matches
	std::for_each(std::sregex_iterator(line.begin(), line.end(), reNumber), std::sregex_iterator(),
		[&](const auto& innerMatch) {
			vec.push_back(stod(innerMatch[1]));
		});

	return vec;
}

// eg. f 1 2 3 4 5
// eg. f 1/2/3 2/3/4 3/4/5
// eg. f 1//3 2//4 3//5
bool Wavefront::parseFace(const std::string& line) {
	std::string strNumber = R"REGEX(\s*(\d*)\s*)REGEX";
	// note that vertex indices are 1-based
	std::vector<int> vindex;
	std::vector<int> nindex;

	if (line.find('/') == std::string::npos) {
		// No forward slash case.
		std::regex reNumber(strNumber);
		// C++ regex doesn't allow repeat captures, so we have to iterate to get subsequent matches
		std::for_each(std::sregex_iterator(line.begin(), line.end(), reNumber), std::sregex_iterator(),
			[&](const auto& innerMatch) {
				if (innerMatch[1] != "") {
					vindex.push_back(stoi(innerMatch[1]));
				}
			});
	}
	else {
		// Forward slash case.
		std::regex reCluster(strNumber + "/" + strNumber + "/" + strNumber);
		std::for_each(std::sregex_iterator(line.begin(), line.end(), reCluster), std::sregex_iterator(),
			[&](const auto& innerMatch) {
				vindex.push_back(stoi(innerMatch[1]));
				// texture index is 2
				nindex.push_back(stoi(innerMatch[3]));
			});
	}

	if (vindex.size() < 3) {
		// Need at least 3 vertices to make a shape.
		return false;
	}

	bool haveNormals = false;
	if (nindex.size() != 0) {
		haveNormals = true;
		if (nindex.size() != vindex.size()) {
			// All or nothing on normal indices.
			return false;
		}
	}

	// fan triangulation.
	int first = 0;
	int last = 1;
	for (int i = 2; i < vindex.size(); i++) {
		int latest = i;

		Point3 p1 = vertices[vindex[first]];
		Point3 p2 = vertices[vindex[last]];
		Point3 p3 = vertices[vindex[latest]];
		std::unique_ptr<Shape> shape;
		if (haveNormals) {
			Vec3 n1 = normals[nindex[first]];
			Vec3 n2 = normals[nindex[last]];
			Vec3 n3 = normals[nindex[latest]];
			shape = std::make_unique<SmoothTriangle>(p1, p2, p3, n1, n2, n3);
		}
		else {
			shape = std::make_unique<Triangle>(p1, p2, p3);
		}

		if (group.count(currentGroup) != 1) {
			group[currentGroup] = std::make_unique<Group>();
		}

		group[currentGroup]->add(std::move(shape));
		last = latest;
	}

	return true;
}


void Wavefront::parse(std::istream& input) {
	std::regex reCommand(R"REGEX(^\s*([a-z]+)\s+)REGEX");
	std::regex reCommandGroup(R"REGEX(^\s*g\s+([a-zA-Z]+))REGEX");

	std::string line;
	while (std::getline(input, line)) {
		bool matched = false;
		std::smatch match;
		if (std::regex_search(line, match, reCommand)) {
			std::string remain = match.suffix().str();

			if (match[1] == "v") {
				// eg. v -1.2 3 0.000
				std::vector<double> v = parseDoubles(remain);
				if (v.size() == 3) {
					vertices.push_back(Point3(v[0], v[1], v[2]));
					matched = true;
				}
			} else if (match[1] == "vn") {
				// eg. vn -1.2 3 0.000
				std::vector<double> v = parseDoubles(remain);
				if (v.size() == 3) {
					normals.push_back(Vec3(v[0], v[1], v[2]));
					matched = true;
				}
			} else if (match[1] == "f") {
				matched = parseFace(remain);
			}
			else if (match[1] == "g") {
				std::smatch gmatch;
				if (std::regex_search(line, gmatch, reCommandGroup)) {
					currentGroup = gmatch[1];
					matched = true;
				}
			}
		}

		if (!matched) {
			ignored++;
		}
	}
}

