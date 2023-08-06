# pragma once

#include "vec.h"

class Light {
	public:
		Vec3 loc;
		float power;

		Light(Vec3 loc_, float power_);
};