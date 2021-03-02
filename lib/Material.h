#pragma once
#include "Color.h"
#include "Light.h"

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

		Color lighting(const PointLight& light, const Point3& point, const Vec3& eye, const Vec3& normal, bool in_shadow) const {
			Color effcolor = color * light.intensity;
			Color ambcolor = effcolor * ambient;
			Color diffcolor = Color::black();
			Color speccolor = Color::black();

			if (!in_shadow) {
				Vec3 lightv = (light.position - point).norm();
				double dotlight = lightv.dot(normal);
				if (dotlight > 0) {
					diffcolor = effcolor * static_cast<float>(diffuse * dotlight);
					Vec3 reflectv = (-lightv).reflect(normal);
					double doteye = reflectv.dot(eye);
					if (doteye > 0) {
						double factor = pow(doteye, shininess);
						speccolor = light.intensity * static_cast<float>(specular * factor);
					}
				}
			}

			return ambcolor + diffcolor + speccolor;
		}

	};
} // namespace ray

