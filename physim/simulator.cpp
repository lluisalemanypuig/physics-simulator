#include <physim/simulator.hpp>

// C includes
#include <assert.h>

// physim includes
#include <physim/fields/gravitational_planet.hpp>
#include <physim/math/math.hpp>

namespace physim {

// PRIVATE

void simulator::init_particle(particles::free_particle *p) {
	global_init->initialise_particle(p);

	// Update the previous position so that Verlet
	// solver can use it correcly. The other solvers
	// do not need it, however.

	// prev_pos <- pos - vel*dt
	__pm_sub_v_vs(p->get_previous_position(), p->get_position(), p->get_velocity(), dt);
}

// PUBLIC

simulator::simulator(const solver_type& s, float t) {
	__pm_assign_c(gravity, 0.0f, -9.81f, 0.0f);
	stime = 0.0f;
	dt = t;
	solver = s;
	global_init = new init::initialiser();
}

simulator::~simulator() {
	clear_geometry();
	clear_particles();
	delete global_init;
}

// BUILD SIMULATION'S CONTENTS

// ----------- particles

const particles::free_particle *simulator::add_particle() {
	particles::free_particle *p = new particles::free_particle();
	p->set_index(ps.size());
	init_particle(p);
	ps.push_back(p);
	return p;
}

void simulator::add_particle(particles::free_particle *p) {
	assert(p != nullptr);
	p->set_index(ps.size());
	ps.push_back(p);
}

void simulator::add_particles(size_t n) {
	for (size_t i = 0; i < n; ++i) {
		add_particle();
	}
}

void simulator::remove_particle(size_t i) {
	assert(i < ps.size());

	// in the position i:
	// 1. free the memory,
	// 2. put a valid pointer to another particle,
	// to the vector: shrink it to delete the
	// unused position at the end

	delete ps[i];
	size_t N = ps.size();
	std::swap(ps[i], ps[N - 1]);
	ps.pop_back();
}

void simulator::clear_particles() {
	for (particles::free_particle *p : ps) {
		delete p;
	}
	ps.clear();
}

// ----------- geometry

void simulator::add_geometry(geom::geometry *g) {
	assert(g != nullptr);
	scene_fixed.push_back(g);
}

void simulator::remove_geometry(size_t i) {
	assert(i < scene_fixed.size());

	// in the position i:
	// 1. free the memory,
	// 2. put a valid pointer to another particle,
	// to the vector: shrink it to delete the
	// unused position at the end

	delete scene_fixed[i];
	size_t N = scene_fixed.size();
	std::swap(scene_fixed[i], scene_fixed[N - 1]);
	scene_fixed.pop_back();
}

void simulator::clear_geometry() {
	for (geom::geometry *g : scene_fixed) {
		delete g;
	}
	scene_fixed.clear();
}

// ----------- fields

void simulator::add_field(fields::field *f) {
	assert(f != nullptr);
	force_fields.push_back(f);
}

void simulator::remove_field(size_t i) {
	assert(i < force_fields.size());

	// in the position i:
	// 1. free the memory,
	// 2. put a valid pointer to another particle,
	// to the vector: shrink it to delete the
	// unused position at the end

	delete force_fields[i];
	size_t N = force_fields.size();
	std::swap(force_fields[i], force_fields[N - 1]);
	force_fields.pop_back();
}

void simulator::clear_fields() {
	for (fields::field *f : force_fields) {
		delete f;
	}
	force_fields.clear();
}

// MODIFIERS

void simulator::clear_simulation() {
	clear_particles();
	clear_geometry();
	stime = 0.0f;
}

void simulator::reset_simulation() {
	stime = 0.0f;
	int i = 0;
	for (particles::free_particle *p : ps) {
		if (not p->is_fixed()) {
			init_particle(p);
		}
		++i;
	}
}

void simulator::apply_time_step() {
	simulate_free_particles();
	stime += dt;
}

// SETTERS

void simulator::add_gravity_acceleration(const math::vec3& g) {
	fields::gravitational_planet *f = new fields::gravitational_planet(g);
	force_fields.push_back(f);
}

void simulator::set_time_step(float t) {
	dt = t;
}

void simulator::set_initialiser(const init::initialiser *f) {
	assert(f != nullptr);

	delete global_init;
	global_init = f->clone();
}

void simulator::set_solver(const solver_type& s) {
	solver = s;
}

// GETTERS

const std::vector<particles::free_particle *>& simulator::get_particles() const {
	return ps;
}

const particles::free_particle& simulator::get_particle(size_t i) const {
	return *ps[i];
}

const std::vector<geom::geometry *>& simulator::get_fixed_objects() const {
	return scene_fixed;
}

float simulator::get_current_time() const {
	return stime;
}

const math::vec3& simulator::get_gravity() const {
	return gravity;
}

size_t simulator::n_particles() const {
	return ps.size();
}

size_t simulator::n_geometry() const {
	return scene_fixed.size();
}

init::initialiser *simulator::get_initialiser() {
	return global_init;
}

const init::initialiser *simulator::get_initialiser() const {
	return global_init;
}

} // -- namespace physim
