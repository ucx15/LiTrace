#include "ray.h"



Ray::Ray() {
	loc = Vec3();
	dir = Vec3();
}

Ray::Ray(Vec3 loc_, Vec3 dir_) {
	loc = loc_;
	dir = dir_.normalize();
}
