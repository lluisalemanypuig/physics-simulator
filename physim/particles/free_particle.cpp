#include <physim/particles/free_particle.hpp>

// C includes
#include <assert.h>

// physim includes
#include <physim/math/private/math3.hpp>

namespace physim {
namespace particles {

// PRIVATE

// PUBLIC

free_particle::free_particle() {
	init();
	__pm3_assign_s(cur_pos, 0.0f);
}

free_particle::free_particle(const free_particle& p) {
	__pm3_assign_v(prev_pos, p.prev_pos);
	__pm3_assign_v(cur_pos, p.cur_pos);
	__pm3_assign_v(cur_vel, p.cur_vel);
	__pm3_assign_v(force, p.force);

	mass = p.mass;
	friction = p.friction;
	bouncing = p.bouncing;
	charge = p.charge;
	lifetime = p.lifetime;
	starttime = p.starttime;
	fixed = p.fixed;
	index = p.index;
}

free_particle::~free_particle() { }

// MODIFIERS

void free_particle::reduce_lifetime(float t) {
	assert(t >= 0.0f);
	lifetime -= t;
}

void free_particle::reduce_starttime(float t) {
	assert(t >= 0.0f);
	starttime -= t;
}

void free_particle::save_position() {
	__pm3_assign_v(prev_pos, cur_pos);
}

void free_particle::init() {
	__pm3_assign_s(prev_pos, 0.0f);
	__pm3_assign_s(cur_vel, 0.0f);
	__pm3_assign_s(force, 0.0f);
	mass = 1.0f;
	bouncing = 0.8f;
	friction = 0.2f;
	charge = 0.0f;
	lifetime = 10.0f;
	starttime = 0.0f;
	fixed = false;
}

} // -- namespace particles
} // -- namespace physim
