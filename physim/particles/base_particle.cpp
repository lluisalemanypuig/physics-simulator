#include <physim/particles/base_particle.hpp>

// C includes
#include <assert.h>

// physim includes
#include <physim/math/private/math3.hpp>

namespace physim {
namespace particles {

base_particle::base_particle() {
	init();
	__pm3_assign_s(cur_pos, 0.0f);
}

base_particle::base_particle(const base_particle& p) {
	__pm3_assign_v(prev_pos, p.prev_pos);
	__pm3_assign_v(cur_pos, p.cur_pos);
	__pm3_assign_v(cur_vel, p.cur_vel);
	__pm3_assign_v(force, p.force);
	mass = p.mass;
	index = p.index;
}

base_particle::~base_particle() {

}

// MODIFIERS

void base_particle::save_position() {
	__pm3_assign_v(prev_pos, cur_pos);
}

void base_particle::init() {
	__pm3_assign_s(prev_pos, 0.0f);
	__pm3_assign_s(cur_vel, 0.0f);
	__pm3_assign_s(force, 0.0f);
	mass = 1.0f;
}

particle_type base_particle::get_particle_type() const {
	return particle_type::base_particle;
}

} // -- namespace particles
} // -- namespace physim
