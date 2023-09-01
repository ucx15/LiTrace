#include "color.hpp"


void Color::reset() {
	r = 0.f;
	g = 0.f;
	b = 0.f;
}


// Unary
Color Color::operator-() {
	return Color(-r, -g, -b);
}


// Binary
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

Color Color::operator/(float f) {
	return Color(r/f, g/f, b/f);
}

Color Color::operator/(Color c) {
	return Color(r/c.r, g/c.g, b/c.b);
}

// Assignment Operators
Color Color::operator+=(Color c) {
	r += c.r;
	g += c.g;
	b += c.b;
	return *this;
}

Color Color::operator-=(Color c) {
	r -= c.r;
	g -= c.g;
	b -= c.b;
	return *this;
}

Color Color::operator*=(float f) {
	r *= f;
	g *= f;
	b *= f;
	return *this;
}

Color Color::operator*=(Color c) {
	r *= c.r;
	g *= c.g;
	b *= c.b;
	return *this;
}

Color Color::operator/=(float f) {
	r /= f;
	g /= f;
	b /= f;
	return *this;
}

Color Color::operator/=(Color c) {
	r /= c.r;
	g /= c.g;
	b /= c.b;
	return *this;
}
