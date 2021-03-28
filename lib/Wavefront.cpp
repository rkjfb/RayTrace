#include "pch.h"
#include <sstream>
#include <algorithm>
#include <regex>

#include "Wavefront.h"

using namespace ray;

void Wavefront::parse(const std::string& obj) {
	std::stringstream ss(obj);
	std::string line;

	std::string strDouble = R"REGEX(\s+(-?\d*\.?\d+))REGEX";
	std::regex reDouble(strDouble);
	std::string strCommand = R"REGEX(\s*([a-z]+))REGEX";
	std::regex reCommand(strCommand + strDouble + strDouble + strDouble);

	while (std::getline(ss, line)) {
		bool matched = false;
		std::smatch match;
		if (std::regex_search(line, match, reCommand)) {
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
					group->add(std::move(t));
					last = latest;
				}

				matched = true;
			}
		}

		if (!matched) {
			ignored++;
		}
	}

}

