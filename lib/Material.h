#pragma once
#include "Color.h"
#include "Light.h"
#include "Pattern.h"

namespace ray {
	class Material
	{
	public:
		std::unique_ptr<Pattern> pattern;
		double ambient = 0.1;
		double diffuse = 0.9;
		double specular = 0.9;
		double shininess = 200;
		double reflective = 0;
		double transparency = 0;
		double refractive_index = 1;

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

		void duplicate(const Material& other) {
			pattern = other.pattern->clone();
			ambient = other.ambient;
			diffuse = other.diffuse;
			specular = other.specular;
			shininess = other.shininess;
			transparency = other.transparency;
			reflective = other.reflective;
			refractive_index = other.refractive_index;
		}

		// copy assignment
		Material& operator=(const Material& other)
		{
			if (this == &other)
				return *this;

			duplicate(other);

			return *this;
		}

		Material(const Material& other)
		{
			duplicate(other);
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

		static Material glass() {
			Material material;
			material.transparency = 1;
			material.reflective = 1;
			material.refractive_index = 1.52;
			material.pattern = std::make_unique<Solid>(Color::black());
			material.ambient = 0;
			material.diffuse = 0;
			material.specular = 0;
			return material;
		}

		static Material air() {
			Material material = glass();
			material.refractive_index = 1.00029;
			return material;
		}

		static Material water() {
			Material material = glass();
			material.refractive_index = 1.333;
			return material;
		}

		static Material diamond() {
			Material material = glass();
			material.refractive_index = 2.417;
			return material;
		}

		static Material mirror() {
			Material material;
			material.pattern = std::make_unique<Solid>(Color(0.1, 0.1, 0.1));
			material.ambient = 0.1;
			material.diffuse = 0.7;
			material.reflective = 0.9;
			material.specular = 0.3;
			material.shininess = 200.0;
			return material;
		}
	};
} // namespace ray

