#include <physim/emitter/base_emitter.hpp>

// physim includes
#include <physim/particles/particle_types.hpp>
#include <physim/math/private/math3.hpp>

// Shortcut for free_particle.
typedef physim::particles::base_particle spart;

namespace physim {
using namespace particles;

namespace emitters {

// PRIVATE

// PUBLIC

base_emitter::base_emitter() {
	pos	 = [](spart p) { __pm3_assign_s(p.cur_pos, 0.0f); };
	vel	 = [](spart p) { __pm3_assign_s(p.cur_vel, 0.0f); };
	mass = [](spart p) { p.mass = 1.0f; };
}

base_emitter::base_emitter(const base_emitter& i) {
	pos = i.pos;
	vel = i.vel;
	mass = i.mass;
}

base_emitter::~base_emitter() { }

// SETTERS

void base_emitter::set_pos_initialiser(const base_emit& f) {
	pos = f;
}

void base_emitter::set_vel_initialiser(const base_emit& f) {
	vel = f;
}

void base_emitter::set_mass_initialiser(const base_emit& f) {
	mass = f;
}

// GETTERS

base_emitter *base_emitter::clone() const {
	return new base_emitter(*this);
}

const base_emit& base_emitter::get_pos_initialiser() const {
	return pos;
}

const base_emit& base_emitter::get_vel_initialiser() const {
	return vel;
}

const base_emit& base_emitter::get_mass_initialiser() const {
	return mass;
}

// INITIALISE A PARTICLE

void base_emitter::initialise_particle(base_particle& p) const {
	pos(p);
	vel(p);
	mass(p);
}

} // -- namespace emitters
} // -- namespace physim
