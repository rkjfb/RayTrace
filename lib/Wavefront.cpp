#include "pch.h"
#include <sstream>
#include <algorithm>
#include <regex>

#include "Wavefront.h"

using namespace ray;

void Wavefront::parse(std::istream& input) {
	std::string strDouble = R"REGEX(\s+(-?\d*\.?\d+))REGEX";
	std::regex reDouble(strDouble);
	std::string strCommandNum = R"REGEX(^\s*([a-z]+))REGEX";
	std::regex reCommandNum(strCommandNum + strDouble + strDouble + strDouble);
	std::regex reCommandGroup(R"REGEX(^\s*g\s+([a-zA-Z]+))REGEX");

	std::string line;
	while (std::getline(input, line)) {
		bool matched = false;
		std::smatch match;
		if (std::regex_search(line, match, reCommandNum)) {
			if (match[1] == "v") {
				// eg. v -1.2 3 0.000
				double x = stod(match[2]);
				double y = stod(match[3]);
				double z = stod(match[4]);
				vertices.push_back(Point3(x, y, z));
				matched = true;
			} else if (match[1] == "f") {
				// eg. f 1 2 3 4 5
				// note that vertex indices are 1-based
				std::vector<int> index;
				index.push_back(stoi(match[2]));
				index.push_back(stoi(match[3]));
				index.push_back(stoi(match[4]));

				// C++ regex doesn't allow repeat captures, so we have to iterate to get subsequent matches
				std::for_each(std::sregex_iterator(match.suffix().first, match.suffix().second, reDouble), std::sregex_iterator(),
					[&](const auto& innerMatch) {
						index.push_back(stoi(innerMatch[1]));
					});

				// fan triangulation.
				Point3 first = vertices[index[0]];
				Point3 last = vertices[index[1]];
				for (int i = 2; i < index.size(); i++) {
					Point3 latest = vertices[index[i]];
					auto t = std::make_unique<Triangle>(first, last, latest);
					if (group.count(currentGroup) != 1) {
						group[currentGroup] = std::make_unique<Group>();
					}
					group[currentGroup]->add(std::move(t));
					last = latest;
				}

				matched = true;
			}
		}
		else if (std::regex_search(line, match, reCommandGroup)) {
			currentGroup = match[1];
			matched = true;
		}

		if (!matched) {
			ignored++;
		}
	}

}

