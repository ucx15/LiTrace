#include <time.h>
#include <fstream>
#include <math.h>


#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write_clean.h"

#include "surface.hpp"
#include "utils.hpp"


#define ACES_a 0.0245786f
#define ACES_b 0.0245786f
#define ACES_c 0.000090537f
#define ACES_d 0.983729f
#define ACES_e 0.4329510f


Surface::Surface(Color* data, int w, int h): width(w), height(h) {
    pixel_count  = width * height;
    aspect_ratio = (float)width/height;
    m_data = data;
}

void Surface::set_at(int x, int y, Color color) {
    m_data[y*width + x] = color;
}

void Surface::fill(Color color) {
	for (int i = 0; i < pixel_count; ++i) {
		m_data[i] = color;
	}
}

void Surface::fill_random() {
	srand(time(NULL));
	for (int i = 0; i < pixel_count; ++i) {
		m_data[i] = rand_color();
	}
}

void Surface::aces() {
    for (int i=0; i<pixel_count; i++) {
        m_data[i].r = std::max(0.f, (float)(m_data[i].r*(m_data[i].r+ACES_a) - ACES_b) / (m_data[i].r * (m_data[i].r*ACES_c + ACES_d) + ACES_e));
        m_data[i].g = std::max(0.f, (float)(m_data[i].g*(m_data[i].g+ACES_a) - ACES_b) / (m_data[i].g * (m_data[i].g*ACES_c + ACES_d) + ACES_e));
        m_data[i].b = std::max(0.f, (float)(m_data[i].b*(m_data[i].b+ACES_a) - ACES_b) / (m_data[i].b * (m_data[i].b*ACES_c + ACES_d) + ACES_e));
    }
}

void Surface::reinhard() {
    for (int i=0; i<pixel_count; i++) {
        m_data[i].r = (float) m_data[i].r/(1+m_data[i].r);
        m_data[i].g = (float) m_data[i].g/(1+m_data[i].g);
        m_data[i].b = (float) m_data[i].b/(1+m_data[i].b);
    }
}

void Surface::gamma() {
    for (int i=0; i<pixel_count; i++) {
        m_data[i].r = sqrtf(m_data[i].r);
        m_data[i].g = sqrtf(m_data[i].g);
        m_data[i].b = sqrtf(m_data[i].b);
    }
}


void Surface::tonemap() {
    // this->aces();  // ACES TMO
    this->reinhard();  // Reinhard TMO
    this->gamma();
}

int Surface::save_float_buffer(const char* file_path) {
    FILE *file = fopen(file_path, "wb");
    if (file == NULL) {
        return -1;
    }

    int size = pixel_count * 3;
    int dim[2] = {width, height};
    fwrite(dim, sizeof(int)*2, 1, file);

    fwrite(m_data, sizeof(float)*size, 1, file);
    fclose(file);

    return 0;
}

int Surface::save_ppm(const char* file_path) {
    FILE *file = fopen(file_path, "wb");
    if (file == NULL) {
        return -1;
    }

    fprintf(file, "P6\n%d %d\n255\n", width, height);
    uint8_t *bytes = new uint8_t[3 * pixel_count];

    int j = 0;
    for (int i = 0; i < pixel_count; i++) {

        bytes[j++] = std::max(0, std::min(0xff, int(255 * m_data[i].r)));  // R
        bytes[j++] = std::max(0, std::min(0xff, int(255 * m_data[i].g)));  // G
        bytes[j++] = std::max(0, std::min(0xff, int(255 * m_data[i].b)));  // B
    }

    fwrite(bytes, 3*pixel_count*sizeof(uint8_t), 1, file);
    fclose(file);

    delete[] bytes;
    return 0;
}


int Surface::save_png(const char* file_name) {
    uint8_t *bytes = new uint8_t[3 * pixel_count];

    int j = 0;
    for (int i = 0; i < pixel_count; i++) {
        bytes[j++] = std::max(0, std::min(0xff, int(255 * m_data[i].r)));  // R
        bytes[j++] = std::max(0, std::min(0xff, int(255 * m_data[i].g)));  // G
        bytes[j++] = std::max(0, std::min(0xff, int(255 * m_data[i].b)));  // B
    }

    stbi_write_png(file_name, width, height, 3, (uint8_t*)bytes, 3*width*sizeof(uint8_t));
    delete[] bytes;
    return 0;
}
