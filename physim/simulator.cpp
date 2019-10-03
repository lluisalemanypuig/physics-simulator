/*********************************************************************
 * Real-time physics simulation project
 * Copyright (C) 2018-2019 Lluís Alemany Puig
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * Contact: Lluís Alemany Puig (lluis.alemany.puig@gmail.com)
 * 
 ********************************************************************/

#include <physim/simulator.hpp>

// C includes
#include <assert.h>

// C++ includes
using namespace std;

// physim includes
#include <physim/fields/gravitational_planet.hpp>
#include <physim/math/private/math3.hpp>

namespace physim {
using namespace math;
using namespace particles;
using namespace geometric;
using namespace fields;
using namespace meshes;
using namespace fluids;

// PRIVATE

void simulator::init_particle(base_particle& p) {
	switch (p.get_particle_type()) {
	case particle_type::free_particle:
		free_global_emit->initialise_particle(static_cast<free_particle&>(p));
		break;
	case particle_type::sized_particle:
		sized_global_emit->initialise_particle(static_cast<sized_particle&>(p));
		break;
	default:
		;
	}

	// Update the previous position so that Verlet
	// solver can use it correcly. The other solvers
	// do not need it, however.

	// prev_pos <- pos - vel*dt
	__pm3_sub_v_vs(p.prev_pos, p.cur_pos, p.cur_vel, dt);
}

void simulator::init_mesh(mesh *m) {
	m->make_initial_state();

	mesh_particle *mps = m->get_particles();

	// loop over the mesh's particles and compute
	// their previous position

	for (size_t i = 0; i < m->size(); ++i) {
		// prev_pos <- pos - vel*dt
		__pm3_sub_v_vs(mps[i].prev_pos, mps[i].cur_pos, mps[i].cur_vel, dt);
	}
}

void simulator::init_fluid(fluid *f) {

	fluid_particle *fps = f->get_particles();

	// loop over the mesh's particles and compute
	// their previous position

	for (size_t i = 0; i < f->size(); ++i) {
		// prev_pos <- pos - vel*dt
		__pm3_sub_v_vs(fps[i].prev_pos, fps[i].cur_pos, fps[i].cur_vel, dt);
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

size_t simulator::add_free_particle() {
	free_particle p;
	p.index = fps.size();
	init_particle(p);
	fps.push_back(p);
	return p.index;
}

size_t simulator::add_sized_particle() {
	sized_particle p;
	p.index = sps.size();
	init_particle(p);
	sps.push_back(p);
	return p.index;
}

size_t simulator::add_agent_particle() {
	agent_particle p;
	p.index = aps.size();
	//init_particle(p);

	if (solver == solver_type::Verlet) {
		// Update the previous position for Verlet
		__pm3_sub_v_vs(p.prev_pos, p.cur_pos, p.cur_vel, dt);
	}

	aps.push_back(p);
	return p.index;
}

size_t simulator::add_free_particle(const free_particle& _p) {
	fps.push_back(_p);

	free_particle& p = fps.back();
	p.index = fps.size() - 1;
	if (solver == solver_type::Verlet) {
		// Update the previous position for Verlet
		__pm3_sub_v_vs(p.prev_pos, p.cur_pos, p.cur_vel, dt);
	}
	return p.index;
}

size_t simulator::add_sized_particle(const sized_particle& _p) {
	sps.push_back(_p);

	sized_particle& p = sps.back();
	p.index = sps.size() - 1;
	if (solver == solver_type::Verlet) {
		// Update the previous position for Verlet
		__pm3_sub_v_vs(p.prev_pos, p.cur_pos, p.cur_vel, dt);
	}
	return p.index;
}

size_t simulator::add_agent_particle(const agent_particle& _p) {
	aps.push_back(_p);

	agent_particle& p = aps.back();
	p.index = aps.size() - 1;
	if (solver == solver_type::Verlet) {
		// Update the previous position for Verlet
		__pm3_sub_v_vs(p.prev_pos, p.cur_pos, p.cur_vel, dt);
	}
	return p.index;
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

void simulator::clear_free_particles() {
	fps.clear();
}

void simulator::clear_sized_particles() {
	sps.clear();
}

void simulator::clear_agent_particles() {
	aps.clear();
}

void simulator::clear_particles() {
	clear_free_particles();
	clear_sized_particles();
	clear_agent_particles();
}

// ----------- geometry

size_t simulator::add_geometry(geometry *g) {
	scene_fixed.push_back(g);
	return scene_fixed.size();
}

void simulator::clear_geometry() {
	for (geometry *g : scene_fixed) {
		delete g;
	}
	scene_fixed.clear();
}

// ----------- fields

size_t simulator::add_field(field *f) {
	force_fields.push_back(f);
	return force_fields.size() - 1;
}

void simulator::clear_fields() {
	for (field *f : force_fields) {
		delete f;
	}
	force_fields.clear();
}

// ----------- meshes

size_t simulator::add_mesh(mesh *m) {
	assert(m != nullptr);
	ms.push_back(m);
	init_mesh(m);
	return ms.size() - 1;
}

void simulator::clear_meshes() {
	for (mesh *m : ms) {
		delete m;
	}
	ms.clear();
}

// ----------- fluids

size_t simulator::add_fluid(fluid *f) {
	assert(f != nullptr);
	fs.push_back(f);
	init_fluid(f);
	return fs.size() - 1;
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
	for (free_particle& p : fps) {
		if (not p.fixed) {
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

void simulator::simulate_fluids(size_t nt) {
	assert(nt > 0);
	if (nt == 1) {
		_simulate_fluids();
	}
	else {
		_simulate_fluids(nt);
	}
}

void simulator::apply_time_step() {
	simulate_sized_particles();
	simulate_agent_particles();
	simulate_free_particles();
	simulate_meshes();
	simulate_fluids();
}

void simulator::apply_time_step(size_t nt) {
	simulate_sized_particles();
	simulate_agent_particles();
	simulate_free_particles();
	simulate_meshes();
	simulate_fluids(nt);
}

// SETTERS

void simulator::set_gravity_acceleration(const math::vec3& g) {
	gravitational_planet *f = new gravitational_planet(g);
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

const vector<free_particle>& simulator::get_free_particles() const {
	return fps;
}
free_particle& simulator::get_free_particle(size_t i) {
	assert(i < fps.size());
	return fps[i];
}
const free_particle& simulator::get_free_particle(size_t i) const {
	assert(i < fps.size());
	return fps[i];
}

const vector<sized_particle>& simulator::get_sized_particles() const {
	return sps;
}
sized_particle& simulator::get_sized_particle(size_t i) {
	assert(i < sps.size());
	return sps[i];
}
const sized_particle& simulator::get_sized_particle(size_t i) const {
	assert(i < sps.size());
	return sps[i];
}

const vector<agent_particle>& simulator::get_agent_particles() const {
	return aps;
}
agent_particle& simulator::get_agent_particle(size_t i) {
	return aps[i];
}
const agent_particle& simulator::get_agent_particle(size_t i) const {
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
	return fs[i];
}

const vector<geometry *>& simulator::get_fixed_objects() const {
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
