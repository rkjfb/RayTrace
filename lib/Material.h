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

	};
} // namespace ray

