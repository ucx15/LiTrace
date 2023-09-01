#include <iostream>
#include <chrono>
#include <math.h>


#include "engine.h"
#include "ray.h"
#include "camera.h"
#include "light.h"
#include "utils.h"
#include "material.h"
#include "settings.h"


Color Engine::cast_ray(Ray ray, int depth) {
	m_hit_count++;
	float min_dist, hit_dist, sdw_hit_dist, l_dist_sq, l_ints;
	Vec3 N, V, L, hit_pt, reflection_vec;

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


		// Shadow Testing
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


		// Lambert Shading (Direct Illumination)
		if (!is_sdw) {
			l_dist_sq = L.magnitude_sq();
			l_ints = (m_scene->light->power * N.dot((L).normalize())) / (4.f * PI * l_dist_sq);
			ray_color = hit_mat->m_color * l_ints * (1-hit_mat->metallic);
		}


		//TODO: Specular Highlights


		// (Global Illumination)
		if (GI) {
			reflect_ray.loc = hit_pt;
			sample_color.reset();

			// Diffuse Reflections
			if (depth < RAY_BOUNCES) {
				for(int sample_cnt=0; sample_cnt<SAMPLES; ++sample_cnt) {
					// d - 2(d.n)n  ->  reflection of a vec d along vec n
					reflection_vec = (V - N*2*V.dot(N));
					reflect_ray.dir = (reflection_vec + (rand_vec3_on_sphere(N)*hit_mat->rough)).normalize();
					
					sample_color += this->cast_ray(reflect_ray, depth+1) * (N.dot(reflect_ray.dir));
				}
				ray_color +=  hit_mat->m_color * (sample_color/SAMPLES);
			}

		}
	}


	// Fallback if ray hits nothing (return world background color)
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

	float nsx, nsy;  // Normal Space Coordinates

	Color pixel_color;
	Ray cam_ray = Ray();


	for (int y=0; y<h; ++y) {
		nsy = 1 - 2 * (float)y/(h-1);
		for (int x=0; x<w; ++x) {
			nsx = 1 - 2 * (float)x/(w-1);

			// Generating Camera ray
			cam_ray = m_scene->cam->gen_ray(nsx, nsy);

			// AntiAliasing
			pixel_color.reset();
			for (int sample=0; sample<AA_SAMPLES; sample++) {
				cam_ray.dir = cam_ray.dir + rand_vec3_bi()*cx;
				pixel_color += this->cast_ray(cam_ray, 0);
			}
			m_surface->set_at(x,y, pixel_color/AA_SAMPLES);
		}
	
		if (!(y%UPDATE_FREQ)) {
			std::cout << float(100.f*y)/H << '\r';
		}

	}
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

	Camera temp_cam = Camera(Vec3(0, 0, 8.f), Vec3(0, 0, -1), FOV);
	Light temp_light = Light(Vec3(0, 10, 2), 3E3f);

	Scene temp_scene = Scene(&temp_cam, &temp_light);
	m_scene = &temp_scene;
	m_scene->background = Color(.01f);

	m_scene->sphere_count = 4;
	Sphere* spheres = new Sphere[m_scene->sphere_count];

	m_scene->sphere_array = spheres;
	
	spheres[0] = Sphere(Vec3(-10, 0, -10), 5);
	spheres[1] = Sphere(Vec3(  0, 0, -10), 5);
	spheres[2] = Sphere(Vec3( 10, 0, -10), 5);
	spheres[3] = Sphere(Vec3(0, -10005, -10), 10000);

	Material mat_lft   = Material(Color(1, 0.01f, 0));
	Material mat_mid = Material(Color(1, 1, 1));
	Material mat_rgt  = Material(Color(0, 0.1f, 1));
	Material mat_grnd = Material(Color(1, 1, 1));

	mat_mid.metallic = 1.f;
	mat_mid.rough = 0.1f;

	mat_rgt.rough = .1f;

	spheres[0].material = &mat_lft;
	spheres[1].material = &mat_mid;
	spheres[2].material = &mat_rgt;
	spheres[3].material = &mat_grnd;


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
