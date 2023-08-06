#pragma once

#include "surface.h"
#include "scene.h"


class Engine {
	public:	
		int m_hit_count = 0;
		Scene* m_scene;
		Surface* m_surface;

		Engine() {};
		void render();
		void pipeline();
};
