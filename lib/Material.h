#pragma once
#include "Color.h"
namespace ray {
	class Material
	{
	public:
		Color color = Color(1,1,1);
		float ambient = 0.1f;
		float diffuse = 0.9f;
		float specular = 0.9f;
		float shininess = 200.0f;

		friend std::ostream& operator<<(std::ostream& os, const Material& m) {
			return os << "Material(" << m.ambient << ", " << m.diffuse << ", " << m.specular << ", " << m.shininess << ")";
		}

		bool operator==(const Material& rhs) const {
			return color == rhs.color && IsEqual(ambient, rhs.ambient) && IsEqual(diffuse, rhs.diffuse) && IsEqual(specular, rhs.specular) && IsEqual(shininess, rhs.shininess);
		}
		bool operator!=(const Material& rhs) const {
			return !operator==(rhs);
		}

	};
} // namespace ray

