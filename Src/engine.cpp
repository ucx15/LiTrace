#include <iostream>
#include <chrono>
#include <math.h>


#include "engine.h"
#include "ray.h"
#include "camera.h"
#include "light.h"
#include "utils.h"
#include "material.h"


constexpr int MAX_MEM = 4;  // in GBs

constexpr float FAR_CLIP = 1000000.0f;
constexpr float EPSILON = 0.000001f;

constexpr int W = 200;
constexpr int H = 200;

constexpr int SAMPLES = 128;        // Min: 1    Max: as req
constexpr int RAY_BOUNCES = 2;    // Min: 1    Max: as req(Stack Limit)
constexpr int AA_SAMPLES = 1;     // Antialiasing samples
constexpr int UPDATE_FREQ = 4;  // in % of H

// Flags
constexpr bool GI = true;         // Global illumination
constexpr bool SHADOWS = true;     // Shadows


Color Engine::cast_ray(Ray ray, int depth) {
	m_hit_count++;
	float min_dist, hit_dist, sdw_hit_dist, l_dist_sq, l_ints;
	Vec3 N, V, L, hit_pt;
	
	bool is_sdw;
	Color ray_color, sample_color;
	Ray reflect_ray = Ray(Vec3(), Vec3());
	Sphere* closest_sphere, *sdw_sphere;
	Material* hit_mat;


	//TODO: Move to a seperate function
	// Finding the closest sphere
	min_dist = FAR_CLIP;
	closest_sphere = NULL;

	for (int i=0; i<m_scene->sphere_count; ++i) {
		hit_dist = (m_scene->sphere_array[i]).intersect(ray);
		if ((EPSILON < hit_dist) && (hit_dist < min_dist)) {
			min_dist = hit_dist;
			closest_sphere = &(m_scene->sphere_array[i]);
		}
	}

	// Shading if camray hit any spheres
	if (closest_sphere) {
		hit_mat = closest_sphere->material;
		hit_pt = ray.loc + ray.dir*(min_dist + EPSILON);

		N = closest_sphere->normal(hit_pt);
		V = ray.dir;
		L = m_scene->light->loc - hit_pt;

		is_sdw = false;
		if (SHADOWS) {
			for (int sdwobj=0; sdwobj<m_scene->sphere_count; ++sdwobj) {
				sdw_sphere = &m_scene->sphere_array[sdwobj];

				if (closest_sphere == sdw_sphere) continue;
				sdw_hit_dist = sdw_sphere->intersect(Ray(hit_pt, L));
				if (EPSILON < sdw_hit_dist) {
					is_sdw = true;
					break;
				}
			}
		}

		if (!is_sdw) {
		// Direct Illumination
			l_dist_sq = L.magnitude_sq();
			l_ints = (m_scene->light->power * N.dot((L).normalize())) / (4.f * PI * l_dist_sq);
			ray_color = hit_mat->m_color*l_ints;
		}

		//TODO: Specular Highlights
		//TODO: Reflections

		// Diffuse Reflections (Global Illumination)
		if (GI) {
			reflect_ray.loc = hit_pt;
			sample_color.reset();
			if (depth < RAY_BOUNCES) {
				for(int sample_cnt=0; sample_cnt<SAMPLES; ++sample_cnt) {				
					reflect_ray.dir = rand_vec3_on_sphere(N);
					sample_color += this->cast_ray(reflect_ray, depth++) * (N.dot(reflect_ray.dir));
					// sample_color += this->cast_ray(reflect_ray, depth++);
				}

				ray_color +=  hit_mat->m_color * (sample_color/SAMPLES);
			}
		}
	}

	else {
		ray_color = m_scene->background;
	}

	return ray_color;
} 

void Engine::render() {
	m_surface->fill(m_scene->background);

	const int w = m_surface->width;
	const int h = m_surface->height;

	const float cell_h = 1.f/h;
	const float cell_w = 1.f/w;
	const float cx = cell_w/2.f;
	const float cy = cell_h/2.f;

	float nsx, nsy;  // Normal Space Coordinates

	Color pixel_color;

	Ray cam_ray = Ray(m_scene->cam->loc, Vec3());
	
	for (int y=0; y<h; ++y) {
		nsy = cy + 1 - 2*(float)y/(h-1);
		for (int x=0; x<w; ++x) {
			nsx = cx + m_surface->aspect_ratio * (2 * (float)x/(w-1) - 1);

			// TODO: To be replaced with camera object producing rays to be shot
			cam_ray.dir = Vec3(nsx, nsy, -.8f).normalize();

			pixel_color.reset();
			for (int sample=0; sample<AA_SAMPLES; sample++) {
				// cam_ray.dir = cam_ray.dir + (rand_vec3_bi()*cx);
				pixel_color += this->cast_ray(cam_ray, 0);
			}
			m_surface->set_at(x,y, pixel_color/AA_SAMPLES);
		}

		if (!(y%UPDATE_FREQ)) {
			std::cout << "Rendered: " << y << "\r";
		}
	}
	std::cout << '\r';
}


int Engine::pipeline() {
	/*
	NOTE:
	 - All memory must be allocated and deallocated in this function only
	   NOT inside any functions called here
	 
	 - All variables must be referred by direct pointers,
   		NOT by scene pointers in this function.
	*/

	// <-------- SCENE Setup --------->
	// Todo(): Implement disk caching for better I/O speeds. (Currently using new operator)
	uint64_t max_mem  = (uint64_t) MAX_MEM * 1024 * 1024 * 1024;
	uint64_t used_mem = (uint64_t)W*H*sizeof(Color) + (uint64_t)3*W*H*sizeof(uint8_t);

	std::cout << "Config:\n";
	std::cout << "\tW: " << W << "\tH: " << H << '\n';
	std::cout << "\tBounces: " << RAY_BOUNCES << "\t";
	std::cout << "Samples: " << SAMPLES << '\n';
	std::cout << "\tmem usage: " << (float)used_mem / (1024*1024) << " MB\t";
	std::cout << "mem avail: " << (float)max_mem / (1024*1024) << " MB\n\n";

	if (used_mem > max_mem) {
		std::cout << "ERROR: OUT of Memory\nExiting\n";
		return 1;
	}


	// Scene Setup
	Color* pixels = new Color[W*H];
	Surface temp_surface = Surface(pixels, W, H);
	m_surface = &temp_surface;

	Camera temp_cam = Camera(Vec3(0), Vec3(0, 0, -1));
	Light temp_light = Light(Vec3(2, 4, 0), 800);

	Scene temp_scene = Scene(&temp_cam, &temp_light);
	m_scene = &temp_scene;
	// m_scene->background = Color(1.f, 1.f, 1.f)/2.f;
	m_scene->background = Color(.008f);

	m_scene->sphere_count = 3;
	Sphere* spheres = new Sphere[m_scene->sphere_count];

	m_scene->sphere_array = spheres;
	
	spheres[0] = Sphere(Vec3(0, 0, -10), 4);
	spheres[1] = Sphere(Vec3(1, 2, -4), .35);
	spheres[2] = Sphere(Vec3(0, -10004, -10), 10000);

	Material mat_white = Material(Color(1, 1, 1));
	Material mat_red   = Material(Color(1, 0, 0));
	Material mat_cyan  = Material(Color(.1f, .35f, 1.f));

	spheres[0].material = &mat_white;
	spheres[1].material = &mat_red;
	spheres[2].material = &mat_cyan;


	// <-------- Rendering -------->
	auto t1 = std::chrono::high_resolution_clock::now();
	this->render();

	// Applying ToneMapping
	auto t2 = std::chrono::high_resolution_clock::now();
	temp_surface.tonemap();

	// Save the Surface
	auto t3 = std::chrono::high_resolution_clock::now();
	temp_surface.save_png("Out/img.png");

	auto t4 = std::chrono::high_resolution_clock::now();

	// Cleanup
	delete[] spheres;
	delete[] pixels;


	// Logs all the timings
	auto t_render_us  = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
	auto t_encode_us  = std::chrono::duration_cast<std::chrono::microseconds>(t3 - t2).count();
	auto t_save_us    = std::chrono::duration_cast<std::chrono::microseconds>(t4 - t3).count();
	auto t_elapsed_us = std::chrono::duration_cast<std::chrono::microseconds>(t4 - t1).count();

	std::cout << "Performance:\n";
	std::cout << '\t' << m_hit_count << " Rays Traced\n";
	std::cout << '\t' << 1000 * m_hit_count/t_render_us << " checks/ms\n";
	std::cout << '\t' << (float) t_render_us/m_hit_count << " us/check\n\n";

	std::cout << "Timers:\n";
	std::cout << "\trender\t " << t_render_us / 1000.f << " ms\n";
	std::cout << "\tencode\t " << t_encode_us / 1000.f << " ms\n";
	std::cout << "\tsave\t " << t_save_us / 1000.f << " ms\n";
	std::cout << "\nTime Took: \t " << t_elapsed_us / 1000.f << " ms\n";
	return 0;
}
