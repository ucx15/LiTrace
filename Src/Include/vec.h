#pragma once

class Vec3 {
	public:
		float x,y,z;

		Vec3(float x_, float y_, float z_): x(x_), y(y_), z(z_) {}
		Vec3(float x_): x(x_), y(x_), z(x_) {}
		Vec3(): x(0.f), y(0.f), z(0.f) {}

		// Unary Operators
		Vec3 operator-();

		// Binary Operators
		bool operator==(Vec3 v);
		Vec3 operator+(Vec3 v);
		Vec3 operator-(Vec3 v);
		Vec3 operator*(float f);

		float magnitude();
		float magnitude_sq();

		float dot(Vec3 v);
		Vec3 cross(Vec3 v);		
		Vec3 normalize();
};
