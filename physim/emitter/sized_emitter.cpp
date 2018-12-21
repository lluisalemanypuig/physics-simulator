#include <physim/emitter/sized_emitter.hpp>

// physim includes
#include <physim/particles/particle_types.hpp>
#include <physim/math/private/math3.hpp>

// Shortcut for base_particle.
typedef physim::particles::sized_particle spart;

namespace physim {
using namespace particles;

namespace emitters {

// PRIVATE

// PUBLIC

sized_emitter::sized_emitter() : free_emitter() {
	radius = [](spart *p) { p->R = 1.0f; };
}

sized_emitter::sized_emitter(const sized_emitter& i) : free_emitter(i) {
	radius = i.radius;
}

sized_emitter::~sized_emitter() { }

// SETTERS

void sized_emitter::set_radius_initialiser(const sized_emit& f) {
	radius = f;
}

// GETTERS

sized_emitter *sized_emitter::clone() const {
	return new sized_emitter(*this);
}

const sized_emit& sized_emitter::get_radius_initialiser() const {
	return radius;
}

// INITIALISE A PARTICLE

void sized_emitter::initialise_particle(particles::sized_particle *p) const {
	free_emitter::initialise_particle(p);
	radius(p);
}

} // -- namespace emitters
} // -- namespace physim
