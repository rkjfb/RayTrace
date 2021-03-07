#pragma once
#include "Color.h"
#include "Light.h"
#include "Pattern.h"

namespace ray {
	class Material
	{
	public:
		std::unique_ptr<Pattern> pattern;
		double ambient = 0.1f;
		double diffuse = 0.9f;
		double specular = 0.9f;
		double shininess = 200.0f;

		Material() : pattern(std::make_unique<Solid>(Color::white())) {}

		friend std::ostream& operator<<(std::ostream& os, const Material& m) {
			return os << "Material(" << m.pattern << ", " << m.ambient << ", " << m.diffuse << ", " << m.specular << ", " << m.shininess << ")";
		}

		bool operator==(const Material& rhs) const {
			return *pattern == *rhs.pattern && IsEqual(ambient, rhs.ambient) && IsEqual(diffuse, rhs.diffuse) && IsEqual(specular, rhs.specular) && IsEqual(shininess, rhs.shininess);
		}
		bool operator!=(const Material& rhs) const {
			return !operator==(rhs);
		}

		// copy assignment
		Material& operator=(const Material& other)
		{
			if (this == &other)
				return *this;

			pattern = other.pattern->clone();
			ambient = other.ambient;
			diffuse = other.diffuse;
			specular = other.specular;
			shininess = other.shininess;

			return *this;
		}

		Color lighting(const PointLight& light, const Shape& shape, const Point3& point, const Vec3& eye, const Vec3& normal, bool in_shadow) const {
			Color baseColor = pattern->pattern_at_shape(shape, point);
			Color effcolor = baseColor * light.intensity;
			Color ambcolor = effcolor * ambient;
			Color diffcolor = Color::black();
			Color speccolor = Color::black();

			if (!in_shadow) {
				Vec3 lightv = (light.position - point).norm();
				double dotlight = lightv.dot(normal);
				if (dotlight > 0) {
					diffcolor = effcolor * static_cast<double>(diffuse * dotlight);
					Vec3 reflectv = (-lightv).reflect(normal);
					double doteye = reflectv.dot(eye);
					if (doteye > 0) {
						double factor = pow(doteye, shininess);
						speccolor = light.intensity * static_cast<double>(specular * factor);
					}
				}
			}

			return ambcolor + diffcolor + speccolor;
		}

	};
} // namespace ray

