#include <physim/particles/sized_particle.hpp>

namespace physim {
namespace particles {

// PRIVATE

void sized_particle::partial_init() {
	R = 1.0f;
}

// PUBLIC

sized_particle::sized_particle() : free_particle() {
	partial_init();
}

sized_particle::sized_particle(const sized_particle& p) : free_particle(p) {
	R = p.R;
}

sized_particle::~sized_particle() {
}

// MODIFIERS

void sized_particle::init() {
	free_particle::init();
	partial_init();
}

// GETTERS

particle_type sized_particle::get_particle_type() const {
	return particle_type::sized_particle;
}

} // -- namespace particles
} // -- namespace physim
