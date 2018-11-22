#include <physim/particles/sized_particle.hpp>

namespace physim {
namespace particles {

sized_particle::sized_particle() : free_particle() { }

sized_particle::sized_particle(const sized_particle& p) : free_particle(p) {
	R = p.R;
}

sized_particle::~sized_particle() {
}

// MODIFIERS

void sized_particle::init() {
	free_particle::init();
	R = 1.0f;
}

// GETTERS

bool sized_particle::is_sized() const {
	return true;
}

} // -- namespace particles
} // -- namespace physim
