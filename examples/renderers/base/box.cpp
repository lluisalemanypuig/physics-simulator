#include <base/box.hpp>

// C includes
#include <assert.h>

// C++ includes
#include <cmath>

// Custom includes
#include "include_gl.hpp"

// PRIVATE

static inline
void draw_edge(const vec3& a, const vec3& b) {
	glBegin(GL_LINES);
		glVertex3f(a.x, a.y, a.z);
		glVertex3f(b.x, b.y, b.z);
	glEnd();
}

vec3 box::make_vi(int i) const {
	switch (i) {
		case 0: return vec3(min.x, min.y, max.z);
		case 1: return vec3(max.x, min.y, max.z);
		case 2: return vec3(max.x, min.y, min.z);
		case 3: return vec3(min.x, min.y, min.z);
		case 4: return vec3(min.x, max.y, max.z);
		case 5: return vec3(max.x, max.y, max.z);
		case 6: return vec3(max.x, max.y, min.z);
		case 7: return vec3(min.x, max.y, min.z);
	}

	assert(i < 8);
	return vec3(0.0f,0.0f,0.0f);
}

void box::update_vi() {
	for (int i = 0; i < 8; ++i) {
		vs[i] = make_vi(i);
	}
}

// PUBLIC

box::box() {}
box::~box() {}

void box::set_min_max(const vec3& m, const vec3& M) {
	min = m;
	max = M;
	update_vi();
}

void box::enlarge_box(const vec3& v) {
	min.x = std::min(min.x, v.x);
	min.y = std::min(min.y, v.y);
	min.z = std::min(min.z, v.z);
	max.x = std::max(max.x, v.x);
	max.y = std::max(max.y, v.y);
	max.z = std::max(max.z, v.z);
	update_vi();
}

void box::translate_box(const vec3& t) {
	min += t;
	max += t;
	update_vi();
}

vec3 box::get_min() const { return min; }
vec3 box::get_max() const { return max; }

float box::length_x() const {return abs(max.x - min.x);}
float box::length_y() const {return abs(max.y - min.y);}
float box::length_z() const {return abs(max.z - min.z);}

// punt mig de la capsa
vec3 box::get_middle_point() const {
	return vec3((min.x + max.x)/2.0f,
				(min.y + max.y)/2.0f,
				(min.z + max.z)/2.0f);
}

// punt mig del pla inferior
vec3 box::get_middle_bottom_point() const {
	return vec3((min.x + max.x)/2.0f,
				 min.y,
				(min.z + max.z)/2.0f);
}

// punt mig del pla frontal
// una vegada rotada la capsa respecte l'eix vertical, no s'assegura
// que el punt frontal sigui el que es correspon amb la "part del davant" de la capsa
vec3 box::get_middle_front_point() const {
	return vec3((min.x + max.x)/2.0f,
				(min.y + max.y)/2.0f,
				 max.z);
}

float box::get_diag_length() const {
	return physim::math::dist(min, max);
}

float box::get_half_diag_length() const {
	return get_diag_length()/2.0f;
}

void box::draw_box() const {
	draw_edge(vs[0], vs[1]);
	draw_edge(vs[1], vs[2]);
	draw_edge(vs[2], vs[3]);
	draw_edge(vs[3], vs[0]);

	draw_edge(vs[4], vs[5]);
	draw_edge(vs[5], vs[6]);
	draw_edge(vs[6], vs[7]);
	draw_edge(vs[7], vs[4]);

	for (int i = 0; i < 4; ++i) {
		draw_edge(vs[i], vs[i + 4]);
	}
}

