# pragma once

#include "color.h"

class Material {
	public:
		Color m_color;
		float absorb;
		float rough;

		Material();
		Material(Color color);
};