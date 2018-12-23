#include <physim/particles/fluid_particle.hpp>

// C includes
#include <assert.h>

// physim includes
#include <physim/math/private/math3.hpp>

namespace physim {
namespace particles {

// PRIVATE

void fluid_particle::partial_init() {
	density = 0.0f;
	pressure = 0.0f;
}

// PUBLIC

fluid_particle::fluid_particle() : base_particle() {
	partial_init();
}

fluid_particle::fluid_particle(const fluid_particle& p) : base_particle(p) {
	density = p.density;
	pressure = p.pressure;
}

fluid_particle::~fluid_particle() { }

// MODIFIERS

void fluid_particle::init() {
	base_particle::init();
	partial_init();
}

particle_type fluid_particle::get_particle_type() const {
	return particle_type::fluid_particle;
}

} // -- namespace particles
} // -- namespace physim
