#include <simulator/simulator.hpp>

namespace sim {

// PRIVATE

// PUBLIC

simulator::simulator() { }

simulator::~simulator() {
	for (geom::geometry *g : scene_fixed) {
		if (g != nullptr) {
			delete g;
		}
	}
	for (particle *p : ps) {
		if (p != nullptr) {
			delete p;
		}
	}
}

// MODIFIERS

void simulator::add_particle() {
	// for now, add an empty particle.
	// In the future, create initialiser class
	particle *p = new particle();
	ps.push_back(p);
}

void simulator::add_particle(particle *p) {
	assert(p != nullptr);
	ps.push_back(p);
}

void simulator::add_particles(size_t n) {
	for (size_t i = 0; i < n; ++i) {
		add_particle();
	}
}

void simulator::add_plane(geom::plane *p) {
	assert(p != nullptr);
	scene_fixed.push_back(p);
}

void simulator::add_triangle(geom::triangle *t) {
	assert(t != nullptr);
	scene_fixed.push_back(t);
}

void simulator::add_sphere(geom::sphere *s) {
	assert(s != nullptr);
	scene_fixed.push_back(s);
}

void simulator::apply_time_step(float dt) {



}

// GETTERS

particle& simulator::get_particle(size_t i) {
	return *ps[i];
}

const particle& simulator::get_particle(size_t i) const {
	return *ps[i];
}

} // -- namespace sim
