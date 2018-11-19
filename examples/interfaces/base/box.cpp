#include <base/box.hpp>

// C includes
#include <assert.h>

// C++ includes
#include <limits>
#include <cmath>

// glm includes
#include <glm/glm.hpp>

// base includes
#include <base/include_gl.hpp>

// PRIVATE

static inline
void draw_edge(const glm::vec3& a, const glm::vec3& b) {
	glBegin(GL_LINES);
		glVertex3f(a.x, a.y, a.z);
		glVertex3f(b.x, b.y, b.z);
	glEnd();
}

glm::vec3 box::make_vi(int i) const {
	switch (i) {
		case 0: return glm::vec3(min.x, min.y, max.z);
		case 1: return glm::vec3(max.x, min.y, max.z);
		case 2: return glm::vec3(max.x, min.y, min.z);
		case 3: return glm::vec3(min.x, min.y, min.z);
		case 4: return glm::vec3(min.x, max.y, max.z);
		case 5: return glm::vec3(max.x, max.y, max.z);
		case 6: return glm::vec3(max.x, max.y, min.z);
		case 7: return glm::vec3(min.x, max.y, min.z);
	}

	assert(i < 8);
	return glm::vec3(0.0f,0.0f,0.0f);
}

void box::update_vi() {
	for (int i = 0; i < 8; ++i) {
		vs[i] = make_vi(i);
	}
}

// PUBLIC

box::box() {
	float M = std::numeric_limits<float>::max();
	float m = -M;

	min = glm::vec3(M,M,M);
	max = glm::vec3(m,m,m);
	update_vi();
}
box::~box() {}

void box::set_min_max(const glm::vec3& m, const glm::vec3& M) {
	min = m;
	max = M;
	update_vi();
}

void box::enlarge_box(const glm::vec3& v) {
	min = glm::min(min, v);
	max = glm::max(max, v);
	update_vi();
}

void box::enlarge_box(const box& b) {
	min = glm::min(min, b.min);
	max = glm::max(max, b.max);
	update_vi();
}

void box::translate_box(const glm::vec3& t) {
	min += t;
	max += t;
	update_vi();
}

glm::vec3 box::get_min() const { return min; }
glm::vec3 box::get_max() const { return max; }

float box::length_x() const {return std::abs(max.x - min.x);}
float box::length_y() const {return std::abs(max.y - min.y);}
float box::length_z() const {return std::abs(max.z - min.z);}

// punt mig de la capsa
glm::vec3 box::get_middle_point() const {
	return glm::vec3((min.x + max.x)/2.0f,
				(min.y + max.y)/2.0f,
				(min.z + max.z)/2.0f);
}

// punt mig del pla inferior
glm::vec3 box::get_middle_bottom_point() const {
	return glm::vec3((min.x + max.x)/2.0f,
				 min.y,
				(min.z + max.z)/2.0f);
}

// punt mig del pla frontal
// una vegada rotada la capsa respecte l'eix vertical, no s'assegura
// que el punt frontal sigui el que es correspon amb la "part del davant" de la capsa
glm::vec3 box::get_middle_front_point() const {
	return glm::vec3((min.x + max.x)/2.0f,
				(min.y + max.y)/2.0f,
				 max.z);
}

float box::get_diag_length() const {
	return glm::distance(min, max);
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

