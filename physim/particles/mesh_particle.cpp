#include <physim/particles/mesh_particle.hpp>

// C includes
#include <assert.h>

// physim includes
#include <physim/math/private/math3.hpp>

namespace physim {
namespace particles {

// PRIVATE

// PUBLIC

mesh_particle::mesh_particle() : base_particle() {
	init();
	__pm3_assign_s(cur_pos, 0.0f);
}

mesh_particle::mesh_particle(const mesh_particle& p) : base_particle(p) {
	mass = p.mass;
	charge = p.charge;
	fixed = p.fixed;
	index = p.index;
}

mesh_particle::~mesh_particle() { }

// MODIFIERS

void mesh_particle::init() {
	base_particle::init();
	charge = 0.0f;
	fixed = false;
}

particle_type mesh_particle::get_particle_type() const {
	return particle_type::mesh_particle;
}

} // -- namespace particles
} // -- namespace physim
