#include <physim/particles/agent_particle.hpp>

// physim includes
#include <physim/math/private/math3.hpp>

namespace physim {
namespace particles {

// PRIVATE

void agent_particle::partial_init() {
	__pm3_assign_s(target, 0.0f);
	max_speed = 1.0f;
	max_force = 1.0f;
	slowing_distance = 0.0f;
	behaviour = agent_behaviour_type::seek;
}

// PUBLIC

agent_particle::agent_particle() {
	partial_init();
}

agent_particle::agent_particle(const agent_particle& p) : sized_particle(p) {
	__pm3_assign_v(target, p.target);
	max_speed = p.max_speed;
	max_force = p.max_force;
	slowing_distance = p.slowing_distance;
	behaviour = p.behaviour;
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
