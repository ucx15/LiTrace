#include <iostream>
#include <chrono>
#include <math.h>
#include <random>


#include "engine.h"
#include "ray.h"
#include "camera.h"
#include "light.h"
#include "utils.h"



Color SPH_COLOR = Color(0, .1, 1);


void Engine::render() {
	m_surface->fill(m_scene->background);

	int w = m_surface->width;
	int h = m_surface->height;

	float nsx, nsy;  // Normal Space Coordinates

	Color pixel_color;
	Ray cam_ray = Ray(m_scene->cam->loc, Vec3());

	float t, l_dist_sq, l_ints;
	Vec3 N, hit_pt, L;

	for (int y = 0; y < h; ++y) {
		nsy = 1 - 2*(float)y/(h-1);
		for (int x = 0; x < w; ++x) {
			nsx = m_surface->aspect_ratio * (2 * (float)x/(w-1) - 1);
			
			cam_ray.dir = Vec3(nsx, nsy, -1).normalize();

			t = m_scene->sphere_obj->intersect(cam_ray);

			if (t and (t > 0)) {
				hit_pt = (cam_ray.dir*t);
				N = m_scene->sphere_obj->normal(hit_pt);
				m_surface->set_at(x, y, Color(N));
				// continue;

				L = (m_scene->light->loc - hit_pt);
				l_dist_sq = L.magnitude_sq();

				l_ints = (m_scene->light->power * N.dot(L.normalize())) / (4.f * PI * l_dist_sq);
				pixel_color = SPH_COLOR * (l_ints);
				
				m_hit_count++;
				m_surface->set_at(x, y, pixel_color);
			}
		}
		std::cout << '\t' << 100 * y / (m_surface->height - 1) << "%\r";

	}
	std::cout << "\n\n";
}


void Engine::pipeline() {

	// <-------- SCENE Setup --------->
	int W = 1600;
	int H = 1600;

	Surface temp_surface = Surface(W, H);
	m_surface = &temp_surface;

	Camera temp_cam = Camera(Vec3(0), Vec3(0, 0, -1));
	Light temp_light = Light(Vec3(4, 4, 0), 300);

	Scene temp_scene = Scene(&temp_cam, &temp_light);
	m_scene = &temp_scene;


	Sphere obj = Sphere(Vec3(0, 0, -5), 2.5);
	m_scene->sphere_obj = &obj;
	m_scene->background = Color(.008f);


	// <-------- Rendering -------->
	std::cout << "Memory:\n\trender\t" << W*H*sizeof(Color)/1024 << "K\n\tsave\t" << 3*W*H*sizeof(uint8_t) / 1024 << "K\n\n";
	std::cout << "Progress:\n";
	
	auto t1 = std::chrono::high_resolution_clock::now();
	this->render();

	// Applying ToneMapping
	auto t2 = std::chrono::high_resolution_clock::now();
	temp_surface.tonemap();

	// Save the Surface
	auto t3 = std::chrono::high_resolution_clock::now();
	// temp_surface.save_ppm("img.ppm");
	temp_surface.save_png("Out/img.png");

	auto t4 = std::chrono::high_resolution_clock::now();

	// Cleanup
	// delete[] pixels;

	// Logs all the timings
	auto t_render_us  = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
	auto t_encode_us  = std::chrono::duration_cast<std::chrono::microseconds>(t3 - t2).count();
	auto t_save_us    = std::chrono::duration_cast<std::chrono::microseconds>(t4 - t3).count();
	auto t_elapsed_us = std::chrono::duration_cast<std::chrono::microseconds>(t4 - t1).count();

	std::cout << "Performance:\n";
	std::cout << '\t' << m_hit_count << " Hits  @ " << (W*H) << " CHECKS\t (" << W*H - m_hit_count << " ignored)\n\n";
	
	std::cout << "Timers:\n";
	std::cout << "\trender\t " << t_render_us / 1000.f << " ms\n";
	std::cout << "\tencode\t " << t_encode_us / 1000.f << " ms\n";
	std::cout << "\tsave\t " << t_save_us / 1000.f << " ms\n";
	std::cout << "\nTime Took: \t " << t_elapsed_us / 1000.f << " ms\n";
}
