#include <physim/initialiser/initialiser.hpp>

// physim includes
#include <physim/math/private/math3.hpp>

namespace physim {
namespace init {

/// Shortcut for particles::free_particle.
typedef particles::free_particle fpart;
/// Shortcut for particles::sized_particle.
typedef particles::sized_particle spart;

// PRIVATE

// PUBLIC

initialiser::initialiser() {
	pos			= [](fpart *p) { __pm3_assign_s(p->cur_pos, 0.0f); };
	vel			= [](fpart *p) { __pm3_assign_s(p->cur_vel, 0.0f); };
	mass		= [](fpart *p) { p->mass = 1.0f; };
	charge		= [](fpart *p) { p->charge = 0.0f; };
	bounce		= [](fpart *p) { p->bouncing = 0.8f; };
	friction	= [](fpart *p) { p->friction = 0.2f; };
	lifetime	= [](fpart *p) { p->lifetime = 10.0f; };
	starttime	= [](fpart *p) { p->starttime = 0.0f; };
	fixed		= [](fpart *p) { p->fixed = false; };
	radius		= [](spart *p) { p->R = 1.0f; };
}

initialiser::initialiser(const initialiser& i) {
	pos = i.pos;
	vel = i.vel;
	mass = i.mass;
	charge = i.charge;
	bounce = i.bounce;
	friction = i.friction;
	lifetime = i.lifetime;
	starttime = i.starttime;
	fixed = i.fixed;
	radius = i.radius;
}

initialiser::~initialiser() { }

// SETTERS

void initialiser::set_pos_initialiser(const partinit_free& f) {
	pos = f;
}

void initialiser::set_vel_initialiser(const partinit_free& f) {
	vel = f;
}

void initialiser::set_mass_initialiser(const partinit_free& f) {
	mass = f;
}

void initialiser::set_charge_initialiser(const partinit_free& f) {
	charge = f;
}

void initialiser::set_bounce_initialiser(const partinit_free& f) {
	bounce = f;
}

void initialiser::set_friction_initialiser(const partinit_free& f) {
	friction = f;
}

void initialiser::set_lifetime_initialiser(const partinit_free& f) {
	lifetime = f;
}

void initialiser::set_starttime_initialiser(const partinit_free& f) {
	starttime = f;
}

void initialiser::set_fixed_initialiser(const partinit_free& f) {
	fixed = f;
}

void initialiser::set_radius_initialiser(const partinit_sized& f) {
	radius = f;
}

// GETTERS

initialiser *initialiser::clone() const {
	return new initialiser(*this);
}

const partinit_free& initialiser::get_pos_initialiser() const {
	return pos;
}

const partinit_free& initialiser::get_vel_initialiser() const {
	return vel;
}

const partinit_free& initialiser::get_mass_initialiser() const {
	return mass;
}

const partinit_free& initialiser::get_charge_initialiser() const {
	return charge;
}

const partinit_free& initialiser::get_bounce_initialiser() const {
	return bounce;
}

const partinit_free& initialiser::get_friction_initialiser() const {
	return friction;
}

const partinit_free& initialiser::get_lifetime_initialiser() const {
	return lifetime;
}

const partinit_free& initialiser::get_starttime_initialiser() const {
	return starttime;
}

const partinit_free& initialiser::get_fixed_initialiser() const {
	return fixed;
}

const partinit_sized& initialiser::get_radius_initialiser() const {
	return radius;
}

// INITIALISE A PARTICLE

void initialiser::initialise_particle(particles::free_particle *p) const {
	pos(p);
	vel(p);
	mass(p);
	charge(p);
	bounce(p);
	friction(p);
	lifetime(p);
	starttime(p);
	fixed(p);
	if (p->is_sized()) {
		particles::sized_particle *sp = static_cast<particles::sized_particle *>(p);
		radius(sp);
	}
}

} // -- namespace init
} // -- namespace physim
