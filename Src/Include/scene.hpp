#pragma once

#include "color.hpp"
#include "surface.hpp"
#include "camera.hpp"
#include "light.hpp"
#include "sphere.hpp"


class Scene {
	public:
		Camera* cam;
		Light* light;

		int sphere_count = 0;
		int plane_count = 0;

		Sphere* sphere_array;  // sphere array 
		// Plane* plane_array;  // plane array 
		
		Color background = Color();

		Scene(Camera* cam, Light* light);
};
