#include <physim/simulator.hpp>

// C includes
#include <assert.h>

// C++ includes
using namespace std;

// physim includes
#include <physim/fields/gravitational_planet.hpp>
#include <physim/math/private/math3.hpp>

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
using namespace math;
using namespace particles;
using namespace meshes;
using namespace fluids;

// PRIVATE

void simulator::init_particle(base_particle *p) {
	switch (p->get_particle_type()) {
	case particle_type::free_particle:
		free_global_emit->initialise_particle(static_cast<free_particle *>(p));
		break;
	case particle_type::sized_particle:
		sized_global_emit->initialise_particle(static_cast<sized_particle *>(p));
		break;
	default:
		;
	}

	// Update the previous position so that Verlet
	// solver can use it correcly. The other solvers
	// do not need it, however.

	// prev_pos <- pos - vel*dt
	__pm3_sub_v_vs(p->prev_pos, p->cur_pos, p->cur_vel, dt);
}

void simulator::init_mesh(mesh *m) {
	m->make_initial_state();

	mesh_particle **mps = m->get_particles();

	// loop over the mesh's particles and compute
	// their previous position

	for (size_t i = 0; i < m->size(); ++i) {
		// prev_pos <- pos - vel*dt
		__pm3_sub_v_vs(mps[i]->prev_pos, mps[i]->cur_pos, mps[i]->cur_vel, dt);
	}
}

void simulator::init_fluid(fluid *f) {
	f->make_initial_state();

	fluid_particle **fps = f->get_particles();

	// loop over the mesh's particles and compute
	// their previous position

	for (size_t i = 0; i < f->size(); ++i) {
		// prev_pos <- pos - vel*dt
		__pm3_sub_v_vs(fps[i]->prev_pos, fps[i]->cur_pos, fps[i]->cur_vel, dt);
	}
}

// PUBLIC

simulator::simulator(const solver_type& s, float t) {
	__pm3_assign_c(gravity, 0.0f, -9.81f, 0.0f);
	dt = t;
	solver = s;
	visc_drag = 0.05f;
	free_global_emit = new emitters::free_emitter();
	sized_global_emit = new emitters::sized_emitter();
	part_part_collisions = false;
}

simulator::~simulator() {
	clear_simulation();
	delete free_global_emit;
	delete sized_global_emit;
}

// BUILD SIMULATION'S CONTENTS

// ----------- particles

free_particle *simulator::add_free_particle() {
	free_particle *p = new free_particle();
	p->index = fps.size();
	init_particle(p);
	fps.push_back(p);
	return p;
}

sized_particle *simulator::add_sized_particle() {
	sized_particle *p = new sized_particle();
	p->index = sps.size();
	init_particle(p);
	sps.push_back(p);
	return p;
}

void simulator::add_free_particle(free_particle *p) {
	assert(p != nullptr);
	p->index = fps.size();
	fps.push_back(p);

	if (solver == solver_type::Verlet) {
		// Update the previous position for Verlet
		__pm3_sub_v_vs(p->prev_pos, p->cur_pos, p->cur_vel, dt);
	}
}

void simulator::add_sized_particle(sized_particle *p) {
	assert(p != nullptr);
	p->index = sps.size();
	sps.push_back(p);

	if (solver == solver_type::Verlet) {
		// Update the previous position for Verlet
		__pm3_sub_v_vs(p->prev_pos, p->cur_pos, p->cur_vel, dt);
	}
}

void simulator::add_agent_particle(agent_particle *p) {
	assert(p != nullptr);
	p->index = aps.size();
	aps.push_back(p);

	if (solver == solver_type::Verlet) {
		// Update the previous position for Verlet
		__pm3_sub_v_vs(p->prev_pos, p->cur_pos, p->cur_vel, dt);
	}
}

void simulator::add_free_particles(size_t n) {
	for (size_t i = 0; i < n; ++i) {
		add_free_particle();
	}
}

void simulator::add_sized_particles(size_t n) {
	for (size_t i = 0; i < n; ++i) {
		add_sized_particle();
	}
}

void simulator::remove_free_particle(size_t i) {
	remove_from_vector(i, fps);
}

void simulator::remove_sized_particle(size_t i) {
	remove_from_vector(i, sps);
}

void simulator::remove_agent_particle(size_t i) {
	remove_from_vector(i, aps);
}

void simulator::clear_free_particles() {
	for (free_particle *p : fps) {
		delete p;
	}
	fps.clear();
}

void simulator::clear_sized_particles() {
	for (sized_particle *p : sps) {
		delete p;
	}
	sps.clear();
}

void simulator::clear_agent_particles() {
	for (agent_particle *p : aps) {
		delete p;
	}
	aps.clear();
}

void simulator::clear_particles() {
	clear_free_particles();
	clear_sized_particles();
	clear_agent_particles();
}

// ----------- geometry

void simulator::add_geometry(geometric::geometry *g) {
	assert(g != nullptr);
	scene_fixed.push_back(g);
}

void simulator::remove_geometry(size_t i) {
	remove_from_vector(i, scene_fixed);
}

void simulator::clear_geometry() {
	for (geometric::geometry *g : scene_fixed) {
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

void simulator::add_mesh(mesh *m) {
	assert(m != nullptr);
	ms.push_back(m);
	init_mesh(m);
}

void simulator::remove_mesh(size_t i) {
	remove_from_vector(i, ms);
}

void simulator::clear_meshes() {
	for (mesh *m : ms) {
		delete m;
	}
	ms.clear();
}

// ----------- fluids

void simulator::add_fluid(fluid *f) {
	assert(f != nullptr);
	fs.push_back(f);
	init_fluid(f);
}

void simulator::remove_fluid(size_t i) {
	remove_from_vector(i, fs);
}

void simulator::clear_fluids() {
	for (fluid *f : fs) {
		delete f;
	}
	fs.clear();
}

// MODIFIERS

void simulator::clear_simulation() {
	clear_geometry();
	clear_particles();
	clear_fields();
	clear_meshes();
	clear_fluids();
}

void simulator::reset_simulation() {
	int i = 0;
	for (free_particle *p : fps) {
		if (not p->fixed) {
			init_particle(p);
		}
		++i;
	}
}

void simulator::simulate_free_particles() {
	_simulate_free_particles();
}

void simulator::simulate_sized_particles() {
	_simulate_sized_particles();
}

void simulator::simulate_agent_particles() {
	_simulate_agent_particles();
}

void simulator::simulate_meshes() {
	_simulate_meshes();
}

void simulator::simulate_fluids() {
	_simulate_fluids();
}

void simulator::apply_time_step() {
	_simulate_free_particles();
	_simulate_agent_particles();
	_simulate_meshes();
	_simulate_fluids();
	_simulate_sized_particles();
}

// SETTERS

void simulator::set_gravity_acceleration(const math::vec3& g) {
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

void simulator::set_free_emitter(const emitters::free_emitter *f) {
	assert(f != nullptr);

	delete free_global_emit;
	free_global_emit = f->clone();
}

void simulator::set_sized_emitter(const emitters::sized_emitter *s) {
	assert(s != nullptr);

	delete sized_global_emit;
	sized_global_emit = s->clone();
}

void simulator::set_solver(const solver_type& s) {
	solver = s;
}

void simulator::set_particle_particle_collisions(bool a) {
	part_part_collisions = a;
}

// GETTERS

const vector<free_particle *>& simulator::get_free_particles() const {
	return fps;
}
free_particle *simulator::get_free_particle(size_t i) {
	assert(i < fps.size());
	return fps[i];
}
const free_particle *simulator::get_free_particle(size_t i) const {
	assert(i < fps.size());
	return fps[i];
}

const vector<sized_particle *>& simulator::get_sized_particles() const {
	return sps;
}
sized_particle *simulator::get_sized_particle(size_t i) {
	assert(i < sps.size());
	return sps[i];
}
const sized_particle *simulator::get_sized_particle(size_t i) const {
	assert(i < sps.size());
	return sps[i];
}

const vector<agent_particle *>& simulator::get_agent_particles() const {
	return aps;
}
agent_particle *simulator::get_agent_particle(size_t i) {
	assert(i < aps.size());
	return aps[i];
}
const agent_particle *simulator::get_agent_particle(size_t i) const {
	assert(i < aps.size());
	return aps[i];
}

const vector<mesh *>& simulator::get_meshes() const {
	return ms;
}
const mesh *simulator::get_mesh(size_t i) const {
	assert(i < ms.size());
	return ms[i];
}

const vector<fluid *>& simulator::get_fluids() const {
	return fs;
}
const fluid *simulator::get_fluid(size_t i) const {
	assert(i < fs.size());
	return fs[i];
}

const vector<geometric::geometry *>& simulator::get_fixed_objects() const {
	return scene_fixed;
}

const math::vec3& simulator::get_gravity() const {
	return gravity;
}

size_t simulator::n_free_particles() const {
	return fps.size();
}
size_t simulator::n_sized_particles() const {
	return sps.size();
}
size_t simulator::n_agent_particles() const {
	return aps.size();
}
size_t simulator::n_meshes() const {
	return ms.size();
}
size_t simulator::n_fluids() const {
	return fs.size();
}

size_t simulator::n_geometry() const {
	return scene_fixed.size();
}

emitters::free_emitter *simulator::get_free_emitter() {
	return free_global_emit;
}

const emitters::free_emitter *simulator::get_free_emitter() const {
	return free_global_emit;
}

emitters::sized_emitter *simulator::get_sized_emitter() {
	return sized_global_emit;
}

const emitters::sized_emitter *simulator::get_sized_emitter() const {
	return sized_global_emit;
}

float simulator::get_time_step() const {
	return dt;
}

bool simulator::part_part_colls_activated() const {
	return part_part_collisions;
}

} // -- namespace physim
