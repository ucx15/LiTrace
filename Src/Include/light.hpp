# pragma once

#include "vec.hpp"

class Light {
	public:
		Vec3 loc;
		float power;

		Light(Vec3 loc_, float power_);
};