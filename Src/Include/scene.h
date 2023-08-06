#pragma once

#include "color.h"
#include "surface.h"
#include "camera.h"
#include "light.h"
#include "sphere.h"


class Scene {
	public:
		Camera *cam;
		Light *light;

		Sphere *sphere_obj;
		Color background = Color();

		Scene(Camera* cam, Light* light);
};
