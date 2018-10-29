#include <physim/initialiser/initialiser.hpp>

namespace physim {
namespace init {

// PRIVATE

// PUBLIC

initialiser::initialiser() {
	pos			= [](particle *) {};
	vel			= [](particle *) {};
	mass		= [](particle *) {};
	bounce		= [](particle *) {};
	friction	= [](particle *) {};
	lifetime	= [](particle *) {};
	starttime	= [](particle *) {};
	fixed		= [](particle *) {};
}

initialiser::initialiser(const initialiser& i) {
	pos = i.pos;
	vel = i.vel;
	mass = i.mass;
	bounce = i.bounce;
	friction = i.friction;
	lifetime = i.lifetime;
	starttime = i.starttime;
	fixed = i.fixed;
}

initialiser::~initialiser() { }

// SETTERS

void initialiser::set_pos_initialiser(const partinit& f) {
	pos = f;
}

void initialiser::set_vel_initialiser(const partinit& f) {
	vel = f;
}

void initialiser::set_mass_initialiser(const partinit& f) {
	mass = f;
}

void initialiser::set_charge_initialiser(const partinit& f) {
	charge = f;
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

void initialiser::set_starttime_initialiser(const partinit& f) {
	starttime = f;
}

void initialiser::set_fixed_initialiser(const partinit& f) {
	fixed = f;
}

// GETTERS

initialiser *initialiser::clone() const {
	return new initialiser(*this);
}

const partinit& initialiser::get_pos_initialiser() const {
	return pos;
}

const partinit& initialiser::get_vel_initialiser() const {
	return vel;
}

const partinit& initialiser::get_mass_initialiser() const {
	return mass;
}

const partinit& initialiser::get_charge_initialiser() const {
	return charge;
}

const partinit& initialiser::get_bounce_initialiser() const {
	return bounce;
}

const partinit& initialiser::get_friction_initialiser() const {
	return friction;
}

const partinit& initialiser::get_lifetime_initialiser() const {
	return lifetime;
}

const partinit& initialiser::get_starttime_initialiser() const {
	return starttime;
}

const partinit& initialiser::get_fixed_initialiser() const {
	return fixed;
}

// INITIALISE A PARTICLE

void initialiser::initialise_particle(particle *p) const {
	pos(p);
	vel(p);
	mass(p);
	bounce(p);
	friction(p);
	lifetime(p);
	starttime(p);
	fixed(p);
}

} // -- namespace init
} // -- namespace physim
