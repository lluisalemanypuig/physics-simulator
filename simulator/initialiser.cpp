#include "initialiser.hpp"

namespace physim {

// PRIVATE

// PUBLIC

initialiser::initialiser() {
	pos			= [](particle *) {};
	vel			= [](particle *) {};
	force		= [](particle *) {};
	bounce		= [](particle *) {};
	friction	= [](particle *) {};
	lifetime	= [](particle *) {};
	fixed		= [](particle *) {};
}

initialiser::initialiser(const initialiser& i) {
	pos = i.pos;
	vel = i.vel;
	force = i.force;
	bounce = i.bounce;
	friction = i.friction;
	lifetime = i.lifetime;
	fixed = i.fixed;
}

initialiser::~initialiser() { }

// OPERATORS

void initialiser::operator()(particle *p) const {
	initialise_particle(p);
}

initialiser& initialiser::operator= (const initialiser& i) {
	pos = i.pos;
	vel = i.vel;
	force = i.force;
	bounce = i.bounce;
	friction = i.friction;
	lifetime = i.lifetime;
	fixed = i.fixed;
	return *this;
}

// SETTERS

void initialiser::set_pos_initialiser(const partinit& f) {
	pos = f;
}

void initialiser::set_vel_initialiser(const partinit& f) {
	vel = f;
}

void initialiser::set_force_initialiser(const partinit& f) {
	force = f;
}

void initialiser::set_bounce_initialiser(const partinit& f) {
	bounce = f;
}

void initialiser::set_friction_initialiser(const partinit& f) {
	friction = f;
}

void initialiser::set_lifetime_initialiser(const partinit& f) {
	lifetime = f;
}

void initialiser::set_fixed_initialiser(const partinit& f) {
	fixed = f;
}

// GETTERS

const partinit& initialiser::set_pos_initialiser() const {
	return pos;
}

const partinit& initialiser::set_vel_initialiser() const {
	return vel;
}

const partinit& initialiser::set_force_initialiser() const {
	return force;
}

const partinit& initialiser::set_bounce_initialiser() const {
	return bounce;
}

const partinit& initialiser::set_friction_initialiser() const {
	return friction;
}

const partinit& initialiser::set_lifetime_initialiser() const {
	return lifetime;
}

const partinit& initialiser::set_fixed_initialiser() const {
	return fixed;
}

// INITIALISE A PARTICLE

void initialiser::initialise_particle(particle *p) const {
	pos(p);
	vel(p);
	force(p);
	bounce(p);
	friction(p);
	lifetime(p);
	fixed(p);
}

} // -- namespace physim
