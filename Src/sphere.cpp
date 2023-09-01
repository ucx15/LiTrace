
#include <math.h>

#include "sphere.hpp"


Sphere::Sphere() {}

Sphere::Sphere(Vec3 loc_, float r_) {
	loc = loc_;
	r = r_;
}


float Sphere::intersect(Ray ray) {
	/*
	Returns in the range of (-INF, +INF)
	
	0 means no or not useful Intersection
	Any other value can be treated as Distance 
	between the ray origin and the intersection point on sphere's surface 
	*/

	Vec3 oc = ray.loc - loc;
	float b = 2 * oc.dot(ray.dir);
	float c = oc.dot(oc) - (r*r);
	float d = (b*b) - (4*c);

	if (d >= 0) {
		return (-b - sqrtf(d)) * 0.5f;
	}

	else{
		return 0.f;
	}	
}

Vec3 Sphere::normal(Vec3 v) {
	return (v - loc).normalize();
}
