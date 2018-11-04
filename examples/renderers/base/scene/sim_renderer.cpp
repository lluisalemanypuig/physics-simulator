#include "sim_renderer.hpp"

// C includes
#include <assert.h>

#include <iostream>
using namespace std;

// base includes
#include <base/include_gl.hpp>

// physim includes
#include <physim/particles/free_particle.hpp>
#include <physim/meshes/mesh.hpp>
using namespace physim;
using namespace meshes;
using namespace particles;


void render_1dmesh(const mesh *m) {

	for (size_t i = 0; i < m->size() - 1; ++i) {
		const vec3& pos1 = (*m)[i]->cur_pos;
		const vec3& pos2 = (*m)[i + 1]->cur_pos;

		glColor3f(1.0f,1.0f,1.0f);
		glBegin(GL_LINES);
			glVertex3f(pos1.x, pos1.y, pos1.z);
			glVertex3f(pos2.x, pos2.y, pos2.z);
		glEnd();
	}
}

// PUBLIC

sim_renderer::sim_renderer() : renderer() { }
sim_renderer::~sim_renderer() { }

void sim_renderer::set_particle_size(float s) {
	glPointSize(s);
}

void sim_renderer::set_spring_width(float w) {
	glLineWidth(w);
}

void sim_renderer::add_geometry(rgeom *r) {
	assert(r != nullptr);

	geometry.push_back(r);
	box b;
	r->make_box(b);
	B.enlarge_box(b);
}

physim::simulator& sim_renderer::get_simulator() {
	return S;
}

void sim_renderer::apply_time_step() {
	S.apply_time_step();
}

void sim_renderer::render_simulation() const {
	for (rgeom *r : geometry) {
		r->draw();
	}

	glDisable(GL_LIGHTING);

	// render particles
	glBegin(GL_POINTS);
	const std::vector<free_particle *>& ps = S.get_particles();
	for (const free_particle *p : ps) {
		if (p->lifetime < 0.2f) {
			glColor3f(0.0f,0.0f,0.0f);
		}
		else {
			glColor3f(1.0f,1.0f,1.0f);
		}
		glVertex3f(p->cur_pos.x, p->cur_pos.y, p->cur_pos.z);
	}
	glEnd();

	// render springs

	const std::vector<mesh *>& mss = S.get_meshes();
	for (const mesh *m : mss) {

		if (m->get_type() == mesh_type::d1) {
			render_1dmesh(m);
		}
	}
}
