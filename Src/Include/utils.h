# pragma once

#include "color.h"
#include "ray.h"


// Constants
constexpr float PI = 3.14159f;
constexpr int RAND_GEN_LIM = 40;

// Macro Expressions
// #define randf ((float) rand() / RAND_MAX)
#define randf ((float) pcg32_random_r() / UINT32_MAX)


// Functions
uint32_t pcg32_random_r();

void print_vec(const Vec3 vec);
void print_color(const Color color);
void print_ray(const Ray ray);


// components of vector is between 0 and 1
Vec3 rand_vec3();

// components of vector is between -1 and 1
Vec3 rand_vec3_bi();

// component of vector is between a and b
Vec3 rand_vec3_bw(float a, float b);

Vec3 rand_vec3_on_sphere(Vec3 normal);

Color rand_color();


// Structs
struct pcg_random_t{
	uint64_t state = 100;
	uint64_t inc = 100;
};
