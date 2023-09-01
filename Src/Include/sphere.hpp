#pragma once


#include "ray.hpp"
#include "material.hpp"

class Sphere {
	public:
		Vec3 loc;
		float r;
		
		Material* material;

		Sphere();
		Sphere(Vec3 loc_, float r_);

		float intersect(Ray ray);
		Vec3 normal(Vec3 v);
};