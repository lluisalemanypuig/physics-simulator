#include <physim/particles/mesh_particle.hpp>

// C includes
#include <assert.h>

// physim includes
#include <physim/math/math_private.hpp>

namespace physim {
namespace particles {

// PRIVATE

void mesh_particle::init() {
	__pm_assign_s(prev_pos, 0.0f);
	__pm_assign_s(cur_vel, 0.0f);
	__pm_assign_s(force, 0.0f);
	mass = 1.0f;
	charge = 0.0f;
	fixed = false;
}

// PUBLIC

mesh_particle::mesh_particle() {
	init();
	__pm_assign_s(cur_pos, 0.0f);
}

mesh_particle::mesh_particle(const mesh_particle& p) {
	__pm_assign_v(prev_pos, p.prev_pos);
	__pm_assign_v(cur_pos, p.cur_pos);
	__pm_assign_v(cur_vel, p.cur_vel);
	__pm_assign_v(force, p.force);

	mass = p.mass;
	charge = p.charge;
	fixed = p.fixed;
	index = p.index;
}

mesh_particle::~mesh_particle() { }

// MODIFIERS

void mesh_particle::save_position() {
	__pm_assign_v(prev_pos, cur_pos);
}

} // -- namespace particles
} // -- namespace physim
