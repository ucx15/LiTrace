# pragma once


#include <random>

#include "color.h"
#include "ray.h"


// Constants
constexpr float PI = 3.14159f;


// Macro Expressions
#define randf ((float) rand() / RAND_MAX)


// Functions
void print_vec(Vec3);
void print_ray(Ray);
Color rand_color();
