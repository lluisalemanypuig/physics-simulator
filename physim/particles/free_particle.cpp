#include <physim/particles/free_particle.hpp>

// C includes
#include <assert.h>

// physim includes
#include <physim/math/private/math3.hpp>

namespace physim {
namespace particles {

// PRIVATE

// PUBLIC

free_particle::free_particle() : base_particle() {
	free_particle::init();
}

free_particle::free_particle(const free_particle& p) : base_particle(p) {
	friction = p.friction;
	bouncing = p.bouncing;
	charge = p.charge;
	lifetime = p.lifetime;
	starttime = p.starttime;
	fixed = p.fixed;
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

void free_particle::init() {
	base_particle::init();
	bouncing = 0.8f;
	friction = 0.2f;
	charge = 0.0f;
	lifetime = 10.0f;
	starttime = 0.0f;
	fixed = false;
}

particle_type free_particle::get_particle_type() const {
	return particle_type::free_particle;
}

} // -- namespace particles
} // -- namespace physim
