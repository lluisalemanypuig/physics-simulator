#include <physim/simulator.hpp>

// C includes
#include <assert.h>

// physim includes
#include <physim/fields/gravitational_planet.hpp>
#include <physim/math/math_private.hpp>

template<class V>
inline void remove_from_vector(size_t i, V& v) {
	assert(i < v.size());

	// in the position i:
	// 1. free the memory,
	// 2. put a valid pointer to another particle,
	// to the vector: shrink it to delete the
	// unused position at the end

	delete v[i];
	size_t N = v.size();
	std::swap(v[i], v[N - 1]);
	v.pop_back();
}

namespace physim {

// PRIVATE

void simulator::init_particle(particles::free_particle *p) {
	global_init->initialise_particle(p);

	// Update the previous position so that Verlet
	// solver can use it correcly. The other solvers
	// do not need it, however.

	// prev_pos <- pos - vel*dt
	__pm_sub_v_vs(p->prev_pos, p->cur_pos, p->cur_vel, dt);
}

void simulator::init_mesh(meshes::mesh *m) {
	m->make_initial_state();

	particles::mesh_particle **mps = m->get_particles();

	// loop over the mesh's particles and compute
	// their previous position

	for (size_t i = 0; i < m->size(); ++i) {
		// prev_pos <- pos - vel*dt
		__pm_sub_v_vs(mps[i]->prev_pos, mps[i]->cur_pos, mps[i]->cur_vel, dt);
	}
}

// PUBLIC

simulator::simulator(const solver_type& s, float t) {
	__pm_assign_c(gravity, 0.0f, -9.81f, 0.0f);
	stime = 0.0f;
	dt = t;
	solver = s;
	visc_drag = 0.05f;
	global_init = new init::initialiser();
}

simulator::~simulator() {
	clear_simulation();
	delete global_init;
}

// BUILD SIMULATION'S CONTENTS

// ----------- particles

const particles::free_particle *simulator::add_particle() {
	particles::free_particle *p = new particles::free_particle();
	p->index = ps.size();
	init_particle(p);
	ps.push_back(p);
	return p;
}

void simulator::add_particle(particles::free_particle *p) {
	assert(p != nullptr);
	p->index = ps.size();
	ps.push_back(p);
}

void simulator::add_particles(size_t n) {
	for (size_t i = 0; i < n; ++i) {
		add_particle();
	}
}

void simulator::remove_particle(size_t i) {
	remove_from_vector(i, ps);
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
	remove_from_vector(i, scene_fixed);
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
	remove_from_vector(i, force_fields);
}

void simulator::clear_fields() {
	for (fields::field *f : force_fields) {
		delete f;
	}
	force_fields.clear();
}

// ----------- meshes

void simulator::add_mesh(meshes::mesh *m) {
	assert(m != nullptr);
	ms.push_back(m);
	init_mesh(m);
}

void simulator::remove_mesh(size_t i) {
	remove_from_vector(i, ms);
}

void simulator::clear_meshes() {
	for (meshes::mesh *m : ms) {
		delete m;
	}
	ms.clear();
}

// MODIFIERS

void simulator::clear_simulation() {
	clear_geometry();
	clear_particles();
	clear_fields();
	clear_meshes();
	stime = 0.0f;
}

void simulator::reset_simulation() {
	stime = 0.0f;
	int i = 0;
	for (particles::free_particle *p : ps) {
		if (not p->fixed) {
			init_particle(p);
		}
		++i;
	}
}

void simulator::apply_time_step() {
	simulate_free_particles();
	simulate_meshes();
	stime += dt;
}

// SETTERS

void simulator::add_gravity_acceleration(const math::vec3& g) {
	fields::gravitational_planet *f = new fields::gravitational_planet(g);
	force_fields.push_back(f);
}

void simulator::set_time_step(float t) {
	assert(t >= 0.0f);
	dt = t;
}

void simulator::set_viscous_drag(float d) {
	assert(d >= 0.0f);
	visc_drag = d;
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
	assert(i < ps.size());
	return *ps[i];
}

const std::vector<meshes::mesh *>& simulator::get_meshes() const {
	return ms;
}

const meshes::mesh& simulator::get_mesh(size_t i) const {
	assert(i < ms.size());
	return *ms[i];
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
