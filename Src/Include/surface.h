#pragma once

#include <iostream>
#include "color.h"


class Surface {
	private:
		Color *m_data;

	public:
		int width;
		int height;
		int pixel_count;
		float aspect_ratio;

		Surface(Color* data, int w, int h);

		void set_at(int x, int y, Color color);
		void fill(Color color);
		void fill_random();
		
		void aces();
		void gamma();
		void tonemap();

		int save_float_buffer(const char *file_path);
		int save_ppm(const char *file_path);
		int save_png(const char *file_path);
};
