#include <simulator/simulator.hpp>

namespace sim {

// PRIVATE

// PUBLIC

simulator::simulator() {
	gravity = vec3(0.0f, -9.81f, 0.0f);
	stime = 0.0f;
}

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

void simulator::reset_simulation() {
	stime = 0.0f;
}

void simulator::apply_time_step(float dt) {
	assert(dt > 0.0f);


}

// SETTERS

void simulator::set_gravity(const vec3& g) {
	gravity = g;
}

// GETTERS

const particle& simulator::get_particle(size_t i) const {
	return *ps[i];
}

const vector<particle *>& simulator::get_all_particles() const {
	return ps;
}

const vector<geom::geometry *>& simulator::get_fixed_objects() const {
	return scene_fixed;
}

float simulator::get_current_time() const {
	return stime;
}

} // -- namespace sim
