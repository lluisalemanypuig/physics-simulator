#include <physim/particles/mesh_particle.hpp>

// C includes
#include <assert.h>

// physim includes
#include <physim/math/private/math3.hpp>

namespace physim {
namespace particles {

// PRIVATE

// PUBLIC

mesh_particle::mesh_particle() {
	init();
	__pm3_assign_s(cur_pos, 0.0f);
}

mesh_particle::mesh_particle(const mesh_particle& p) {
	__pm3_assign_v(prev_pos, p.prev_pos);
	__pm3_assign_v(cur_pos, p.cur_pos);
	__pm3_assign_v(cur_vel, p.cur_vel);
	__pm3_assign_v(force, p.force);

	mass = p.mass;
	charge = p.charge;
	fixed = p.fixed;
	index = p.index;
}

mesh_particle::~mesh_particle() { }

// MODIFIERS

void mesh_particle::save_position() {
	__pm3_assign_v(prev_pos, cur_pos);
}

void mesh_particle::init() {
	__pm3_assign_s(prev_pos, 0.0f);
	__pm3_assign_s(cur_vel, 0.0f);
	__pm3_assign_s(force, 0.0f);
	mass = 0.25f;
	charge = 0.0f;
	fixed = false;
}

} // -- namespace particles
} // -- namespace physim
