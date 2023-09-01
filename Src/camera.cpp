#include <math.h>
#include <iostream>

#include "camera.h"
#include "settings.h"
#include "utils.h"


Camera::Camera(Vec3 loc_, Vec3 at_, float fov_): loc(loc_), at(at_), fov(fov_) {
	up = Vec3(0, 1, 0);
	m_fov = ((float) fov*PI) / 180.f;

	m_forw = (at - loc).normalize();
	m_right = -m_forw.cross(up).normalize();
	m_up = m_forw.cross(m_right).normalize();

	m_iPH = tanf(m_fov);
	m_iPW = m_iPH * AR;
};


Ray Camera::gen_ray(float x, float y) {
	float RD = x*m_iPW;
	float UD = y*m_iPH;

	Vec3 ray_dir = (m_forw + (m_right*RD) + (m_up*UD));
	return Ray(loc, ray_dir);
}