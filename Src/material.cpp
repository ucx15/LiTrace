#include "material.hpp"


Material::Material() {
	m_color = Color(0.f);
	absorb = 0.f;
	rough = 1.f;
	metallic = 0.f;
};

Material::Material(Color color) {
	m_color = color;
	absorb = 0.f;
	rough = 1.f;
};