#include <iostream>


#include "utils.h"


void print_vec(Vec3 vec) {
	std::cout << vec.x << ' ' << vec.y << ' ' << vec.z << '\n'; 
}

void print_ray(Ray ray) {
	std::cout << "Location:\t";
	print_vec(ray.loc);
	std::cout << "Direction:\t";
	print_vec(ray.dir);
}


Color rand_color() {
	return Color(randf, randf, randf);
}
