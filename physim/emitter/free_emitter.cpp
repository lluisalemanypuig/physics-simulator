#include <physim/emitter/free_emitter.hpp>

// physim includes
#include <physim/particles/particle_types.hpp>
#include <physim/math/private/math3.hpp>

// Shortcut for free_particle.
typedef physim::particles::free_particle fpart;

namespace physim {
using namespace particles;

namespace emitters {

// PRIVATE

// PUBLIC

free_emitter::free_emitter() : base_emitter() {
	charge		= [](fpart& p) { p.charge = 0.0f; };
	bounce		= [](fpart& p) { p.bouncing = 0.8f; };
	friction	= [](fpart& p) { p.friction = 0.2f; };
	lifetime	= [](fpart& p) { p.lifetime = 10.0f; };
	starttime	= [](fpart& p) { p.starttime = 0.0f; };
	fixed		= [](fpart& p) { p.fixed = false; };
}

free_emitter::free_emitter(const free_emitter& i) : base_emitter(i) {
	charge = i.charge;
	bounce = i.bounce;
	friction = i.friction;
	lifetime = i.lifetime;
	starttime = i.starttime;
	fixed = i.fixed;
}

free_emitter::~free_emitter() { }

// SETTERS

void free_emitter::set_charge_initialiser(const free_emit& f) {
	charge = f;
}

void free_emitter::set_bounce_initialiser(const free_emit& f) {
	bounce = f;
}

void free_emitter::set_friction_initialiser(const free_emit& f) {
	friction = f;
}

void free_emitter::set_lifetime_initialiser(const free_emit& f) {
	lifetime = f;
}

void free_emitter::set_starttime_initialiser(const free_emit& f) {
	starttime = f;
}

void free_emitter::set_fixed_initialiser(const free_emit& f) {
	fixed = f;
}

// GETTERS

free_emitter *free_emitter::clone() const {
	return new free_emitter(*this);
}

const free_emit& free_emitter::get_charge_initialiser() const {
	return charge;
}

const free_emit& free_emitter::get_bounce_initialiser() const {
	return bounce;
}

const free_emit& free_emitter::get_friction_initialiser() const {
	return friction;
}

const free_emit& free_emitter::get_lifetime_initialiser() const {
	return lifetime;
}

const free_emit& free_emitter::get_starttime_initialiser() const {
	return starttime;
}

const free_emit& free_emitter::get_fixed_initialiser() const {
	return fixed;
}

// INITIALISE A PARTICLE

void free_emitter::initialise_particle(free_particle& p) const {
	base_emitter::initialise_particle(p);
	charge(p);
	bounce(p);
	friction(p);
	lifetime(p);
	starttime(p);
	fixed(p);
}

} // -- namespace emitters
} // -- namespace physim
