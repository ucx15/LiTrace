# pragma once

#include "vec.h"


class Ray {
	public:
		Vec3 loc, dir;

		Ray();
		Ray(Vec3 loc_, Vec3 dir_);
};
