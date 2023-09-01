#pragma once

#include "vec.hpp"


class Color {
	public:
		float r;
		float g;
		float b;
		
		Color(float r_, float g_, float b_): r(r_), g(g_), b(b_) {}
		Color(float c): r(c), g(c), b(c) {}
		Color(): r(0.f), g(0.f), b(0.f) {}
		Color(Vec3 v): r(v.x), g(v.y), b(v.z) {}
		
		void reset();

		// Unary
		Color operator-();

		// Binary
		bool operator==(Color c);
		Color operator+(Color c);
		Color operator-(Color c);
		Color operator*(float f);
		Color operator*(Color c);
		Color operator/(float f);
		Color operator/(Color f);

		// Assignment Operators
		Color operator+=(Color c);
		Color operator-=(Color c);
		Color operator*=(float f);
		Color operator*=(Color c);
		Color operator/=(float f);
		Color operator/=(Color c);
};
