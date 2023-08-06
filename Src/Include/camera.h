#pragma once

#include "vec.h"


class Camera {
	public:
		Vec3 loc;
		Vec3 dir;

		Camera(Vec3 loc_, Vec3 dir_): loc(loc_), dir(dir_) {};
};