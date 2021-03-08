#include "pch.h"
#include "Pattern.h"
#include "Shape.h"
#include "Perlin.h"

using namespace ray;

Color Pattern::pattern_at_shape(const Shape& shape, const Point3& p) const {
	Point3 object_point = shape.transform.inverse_multiply(p);
	Point3 pattern_point = transform.inverse_multiply(object_point);
	return pattern_at(pattern_point);
}

Color PerlinPattern::pattern_at(const Point3& p) const {

		float perlin_vec[3] = { (float)p.x, (float)p.y, (float)p.z };

		Point3 newp(p);

		float f = scale*perlin_noise3(perlin_vec);
		newp.x += f;

		f = scale * perlin_noise3(perlin_vec);
		newp.y += f;

		f = scale * perlin_noise3(perlin_vec);
		newp.z += f;

		return jitter->pattern_at(jitter->transform.inverse_multiply(newp));
	}
