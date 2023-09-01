#pragma once

#include "vec.h"
#include "ray.h"

class Camera {
	private:
	float m_fov;  // in radians

	Vec3 m_forw;
	Vec3 m_right;
	Vec3 m_up;

	float m_iPH;
	float m_iPW;

	public:
		Vec3 loc;
		Vec3 at; // dir in which camera is pointing
		
		Vec3 up;
		float fov;  // in degrees

		Camera(Vec3 loc_, Vec3 dir_, float fov_);
		Ray gen_ray(float x, float y);

};