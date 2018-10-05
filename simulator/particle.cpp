#include <simulator/particle.hpp>

namespace physim {

// PRIVATE

// PUBLIC

particle::particle() {
	init();
	pos = vec3(0.0f,0.0f,0.0f);
}

particle::particle(const float& x, const float& y, const float& z) {
	init();
	pos = vec3(x,y,z);
}

particle::particle(const vec3& p) {
	init();
	pos = p;
}

particle::particle(const particle& p) {
	pos = p.pos;
	force = p.force;
	prev_velocity = p.prev_velocity;
	cur_velocity = p.cur_velocity;

	friction = p.friction;
	bouncing = p.bouncing;
	lifetime = p.lifetime;
	fixed = p.fixed;
}

particle::~particle() { }

// MODIFIERS

void particle::add_force(const float& x, const float& y, const float& z) {
	force += vec3(x,y,z);
}
void particle::add_force(const vec3& f) {
	force += f;
}

void particle::translate(const vec3& v) {
	pos += v;
}

void particle::acceleterate(const vec3& v) {
	cur_velocity += v;
}

void particle::reduce_lifetime(float t) {
	assert(t >= 0.0f);
	lifetime -= t;
}

void particle::init() {
	pos = vec3(0.0f,0.0f,0.0f);
	force = vec3(0.0f,0.0f,0.0f);
	prev_velocity = vec3(0.0f,0.0f,0.0f);
	cur_velocity = vec3(0.0f,0.0f,0.0f);
	friction = 0.0f;
	bouncing = 1.0f;
	lifetime = 10.0f;
	fixed = false;
}

void particle::save_velocity() {
	prev_velocity = cur_velocity;
}

// SETTERS

void particle::set_position(const float& x, const float& y, const float& z) {
	pos = vec3(x,y,z);
}
void particle::set_position(const vec3& p) {
	pos = p;
}

void particle::set_previous_velocity(const float& x, const float& y, const float& z) {
	prev_velocity = vec3(x,y,z);
}
void particle::set_previous_velocity(const vec3& vel) {
	prev_velocity = vel;
}

void particle::set_velocity(const float& x, const float& y, const float& z) {
	cur_velocity = vec3(x,y,z);
}
void particle::set_velocity(const vec3& vel) {
	cur_velocity = vel;
}

void particle::set_force(const float& x, const float& y, const float& z) {
	force = vec3(x,y,z);
}
void particle::set_force(const vec3& f) {
	force = f;
}

void particle::set_friction(float f) {
	friction = f;
}

void particle::set_bouncing(float b) {
	bouncing = b;
}

void particle::set_lifetime(float lT) {
	lifetime = lT;
}

void particle::set_fixed(bool f) {
	fixed = f;
}

// GETTERS

vec3 particle::get_position() const {
	return pos;
}

vec3 particle::get_force() const {
	return force;
}

vec3 particle::get_previous_velocity() const {
	return prev_velocity;
}

vec3 particle::get_velocity() const {
	return cur_velocity;
}

float particle::get_friction() const {
	return friction;
}

float particle::get_bouncing() const {
	return bouncing;
}

float particle::get_lifetime() const {
	return lifetime;
}

bool particle::is_fixed() const {
	return fixed;
}

} // -- namespace sim
