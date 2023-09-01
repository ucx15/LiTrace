#include <iostream>

#include "utils.hpp"


// Pseudo Random Number Generator
pcg_random_t PCG_STRUCT;
uint32_t pcg32_random_r() {
    uint64_t oldstate = PCG_STRUCT.state;
    PCG_STRUCT.state = oldstate * 6364136223846793005ULL + (PCG_STRUCT.inc|1);
    uint32_t xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
    uint32_t rot = oldstate >> 59u;
    return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}


void print_vec(const Vec3 vec) {
	std::cout << vec.x << ' ' << vec.y << ' ' << vec.z << '\n'; 
}

void print_color(const Color color) {
	std::cout << color.r << ' ' << color.g << ' ' << color.b << '\n'; 
}

void print_ray(const Ray ray) {
	std::cout << "Location:\t";
	print_vec(ray.loc);
	std::cout << "Direction:\t";
	print_vec(ray.dir);
}

// component of vector is between 0 and 1
Vec3 rand_vec3() {
	return Vec3(randf, randf, randf);
}

// component of vector is between -1 and 1
Vec3 rand_vec3_bi() {
	return Vec3(1 - (2*randf), 1 - (2*randf), 1 - (2*randf));
}

// component of vector is between a and b
Vec3 rand_vec3_bw(float a, float b){
	return Vec3(std::max(a, b*randf), std::max(a, b*randf), std::max(a, b*randf));
}


Vec3 rand_vec3_on_sphere(Vec3 normal) {
	Vec3 v = rand_vec3_bi();

	if (v.dot(normal) < 0) {
		return -v;
	}
	else {
		return v;
	}
}


Color rand_color() {
	return Color(randf, randf, randf);
}