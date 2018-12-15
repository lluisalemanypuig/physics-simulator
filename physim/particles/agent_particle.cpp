#include <physim/particles/agent_particle.hpp>

// physim includes
#include <physim/math/private/math3.hpp>

namespace physim {
namespace particles {

// PRIVATE

void agent_particle::partial_init() {
	__pm3_assign_s(attractor, 0.0f);
	attractor_acceleration = 0.0f;
	max_vel = 1.0f;
}

// PUBLIC

agent_particle::agent_particle() {
	partial_init();
}

agent_particle::agent_particle(const agent_particle& p) : sized_particle(p) {
	__pm3_assign_v(attractor, p.attractor);
	attractor_acceleration = p.attractor_acceleration;
	max_vel = p.max_vel;
}

agent_particle::~agent_particle() {
}

void agent_particle::init() {
	base_particle::init();
	partial_init();
}

// GETTERS

particle_type agent_particle::get_particle_type() const {
	return particle_type::agent_particle;
}

} // -- namespace particles
} // -- namespace physim
