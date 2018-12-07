#include <physim/particles/mesh_particle.hpp>

// C includes
#include <assert.h>

// physim includes
#include <physim/math/private/math3.hpp>

namespace physim {
namespace particles {

// PRIVATE

void mesh_particle::partial_init() {
	charge = 0.0f;
	fixed = false;
}

// PUBLIC

mesh_particle::mesh_particle() : base_particle() {
	partial_init();
}

mesh_particle::mesh_particle(const mesh_particle& p) : base_particle(p) {
	charge = p.charge;
	fixed = p.fixed;
}

mesh_particle::~mesh_particle() { }

// MODIFIERS

void mesh_particle::init() {
	base_particle::init();
	partial_init();
}

particle_type mesh_particle::get_particle_type() const {
	return particle_type::mesh_particle;
}

} // -- namespace particles
} // -- namespace physim
