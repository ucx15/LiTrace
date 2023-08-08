#pragma once

#include "color.h"
#include "surface.h"
#include "camera.h"
#include "light.h"
#include "sphere.h"


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
