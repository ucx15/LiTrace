# pragma once

#include "color.h"

class Material {
	public:
		Color m_color;
		float absorb;
		float rough;
		float metallic;

		Material();
		Material(Color color);
};