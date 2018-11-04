#include "sim_renderer.hpp"

// C includes
#include <assert.h>

#include <iostream>
using namespace std;

// base includes
#include <base/include_gl.hpp>

// physim includes
#include <physim/particles/free_particle.hpp>
using namespace physim;
using namespace particles;

sim_renderer::sim_renderer() : renderer() {
	part_size = 2.0f;
}
sim_renderer::~sim_renderer() { }

physim::simulator& sim_renderer::get_simulator() {
	return S;
}

void sim_renderer::add_geometry(rgeom *r) {
	assert(r != nullptr);

	geometry.push_back(r);
	box b;
	r->make_box(b);
	B.enlarge_box(b);
}

void sim_renderer::apply_time_step() {
	S.apply_time_step();
}

void sim_renderer::render_simulation() const {
	for (rgeom *r : geometry) {
		r->draw();
	}

	glDisable(GL_LIGHTING);
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
}
