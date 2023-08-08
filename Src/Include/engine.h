#pragma once

#include "surface.h"
#include "scene.h"


class Engine {
	public:	
		uint64_t m_hit_count = 0;

		Scene* m_scene;
		Surface* m_surface;

		Engine() {};
		Color cast_ray(Ray ray, int depth);
		void render();
		int pipeline();
};
