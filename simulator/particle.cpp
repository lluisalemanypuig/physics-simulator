#include <simulator/particle.hpp>

namespace sim {

// PRIVATE

void particle::init() {
	prev_pos = vec3(0.0f,0.0f,0.0f);
	cur_pos = vec3(0.0f,0.0f,0.0f);
	force = vec3(0.0f,0.0f,0.0f);
	velocity = vec3(0.0f,0.0f,0.0f);
	friction = 0.0f;
	bouncing = 1.0f;
	lifetime = 10.0f;
	fixed = false;
}

// PUBLIC

particle::particle() {
	init();
	cur_pos = vec3(0.0f,0.0f,0.0f);
}

particle::particle(const float& x, const float& y, const float& z) {
	init();
	cur_pos = vec3(x,y,z);
}

particle::particle(const vec3& pos) {
	init();
	cur_pos = pos;
}

particle::particle(const particle& p) {
	cur_pos = p.cur_pos;
	prev_pos = p.prev_pos;
	force = p.force;
	velocity = p.velocity;

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
	cur_pos += v;
}

void particle::acceleterate(const vec3& v) {
	velocity += v;
}

void particle::reduce_lifetime(float t) {
	assert(t >= 0.0f);
	lifetime -= t;
}

// SETTERS

void particle::set_position(const float& x, const float& y, const float& z) {
	cur_pos = vec3(x,y,z);
}
void particle::set_position(const vec3& pos) {
	cur_pos = pos;
}

void particle::set_prev_position(const float& x, const float& y, const float& z) {
	prev_pos = vec3(x,y,z);
}
void particle::set_prev_position(const vec3& pos) {
	prev_pos = pos;
}

void particle::set_velocity(const float& x, const float& y, const float& z) {
	velocity = vec3(x,y,z);
}
void particle::set_velocity(const vec3& vel) {
	velocity = vel;
}

void particle::set_force(const float& x, const float& y, const float& z) {
	force = vec3(x,y,z);
}
void particle::set_force(const vec3& f) {
	force = f;
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

vec3 particle::get_current_position() const {
	return cur_pos;
}

vec3 particle::get_previous_position() const {
	return prev_pos;
}

vec3 particle::get_force() const {
	return force;
}

vec3 particle::get_velocity() const {
	return velocity;
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
