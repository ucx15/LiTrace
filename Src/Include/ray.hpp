# pragma once

#include "vec.hpp"


class Ray {
	public:
		Vec3 loc, dir;

		Ray();
		Ray(Vec3 loc_, Vec3 dir_);
};
