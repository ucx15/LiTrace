#include "color.h"


bool Color::operator==(Color c) {
	return ((r==c.r) && (g==c.g) && (b==c.b));
}

Color Color::operator+(Color c) {
	return Color(r+c.r, g+c.g, b+c.b);
}

Color Color::operator-(Color c) {
	return Color(r-c.r, g-c.g, b-c.b);
}

Color Color::operator*(Color c) {
	return Color(r*c.r, g*c.g, b*c.b);
}

Color Color::operator*(float f) {
	return Color(r*f, g*f, b*f);
}