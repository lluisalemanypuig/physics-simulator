#include <physim/particles/particle.hpp>

// C includes
#include <assert.h>

// physim includes
#include <physim/math/math.hpp>

namespace physim {

// PRIVATE

void particle::init() {
	__pm_assign_c(prev_pos, 0.0f,0.0f,0.0f);
	__pm_assign_c(cur_velocity, 0.0f,0.0f,0.0f);
	__pm_assign_c(force, 0.0f,0.0f,0.0f);
	mass = 1.0f;
	bouncing = 1.0f;
	friction = 0.0f;
	lifetime = 10.0f;
	starttime = 0.0f;
	fixed = false;
}

// PUBLIC

particle::particle() {
	init();
	__pm_assign_c(cur_pos, 0.0f,0.0f,0.0f);
}

particle::particle(const float& x, const float& y, const float& z) {
	init();
	__pm_assign_c(cur_pos, x,y,z);
}

particle::particle(const math::vec3& p) {
	init();
	__pm_assign_v(cur_pos, p);
}

particle::particle(const particle& p) {
	__pm_assign_v(prev_pos, p.prev_pos);
	__pm_assign_v(cur_pos, p.cur_pos);
	__pm_assign_v(cur_velocity, p.cur_velocity);
	__pm_assign_v(force, p.force);

	mass = p.mass;
	friction = p.friction;
	bouncing = p.bouncing;
	lifetime = p.lifetime;
	starttime = p.starttime;
	fixed = p.fixed;
	index = p.index;
}

particle::~particle() { }

// MODIFIERS

void particle::translate(const math::vec3& v) {
	__pm_add_acc_v(cur_pos, v);
}

void particle::accelerate(const math::vec3& v) {
	__pm_add_acc_v(cur_velocity, v);
}

void particle::add_force(const float& x, const float& y, const float& z) {
	__pm_add_acc_c(force, x,y,z);
}
void particle::add_force(const math::vec3& f) {
	__pm_add_acc_v(force, f);
}

void particle::reduce_lifetime(float t) {
	assert(t >= 0.0f);
	lifetime -= t;
}

void particle::reduce_starttime(float t) {
	assert(t >= 0.0f);
	starttime -= t;
}

void particle::save_position() {
	__pm_assign_v(prev_pos, cur_pos);
}

// SETTERS

void particle::set_previous_position(const float& x, const float& y, const float& z) {
	__pm_assign_c(prev_pos, x,y,z);
}
void particle::set_previous_position(const math::vec3& p) {
	__pm_assign_v(prev_pos, p);
}

void particle::set_position(const float& x, const float& y, const float& z) {
	__pm_assign_c(cur_pos, x,y,z);
}
void particle::set_position(const math::vec3& p) {
	__pm_assign_v(cur_pos, p);
}

void particle::set_velocity(const float& x, const float& y, const float& z) {
	__pm_assign_c(cur_velocity, x,y,z);
}
void particle::set_velocity(const math::vec3& vel) {
	__pm_assign_v(cur_velocity, vel);
}

void particle::set_force(const float& x, const float& y, const float& z) {
	__pm_assign_c(force, x,y,z);
}
void particle::set_force(const math::vec3& f) {
	__pm_assign_v(force, f);
}

void particle::set_mass(float m) {
	mass = m;
}

void particle::set_bouncing(float b) {
	bouncing = b;
}

void particle::set_friction(float f) {
	friction = f;
}

void particle::set_lifetime(float lT) {
	lifetime = lT;
}

void particle::set_starttime(float sT) {
	starttime = sT;
}

void particle::set_fixed(bool f) {
	fixed = f;
}

void particle::set_index(size_t i) {
	index = i;
}

// GETTERS

math::vec3& particle::get_previous_position() {
	return prev_pos;
}
const math::vec3& particle::get_previous_position() const {
	return prev_pos;
}

math::vec3& particle::get_position() {
	return cur_pos;
}
const math::vec3& particle::get_position() const {
	return cur_pos;
}

math::vec3& particle::get_velocity() {
	return cur_velocity;
}
const math::vec3& particle::get_velocity() const {
	return cur_velocity;
}

math::vec3& particle::get_force() {
	return force;
}
const math::vec3& particle::get_force() const {
	return force;
}

float particle::get_mass() const {
	return mass;
}

float particle::get_bouncing() const {
	return bouncing;
}

float particle::get_friction() const {
	return friction;
}

float particle::get_lifetime() const {
	return lifetime;
}

float particle::get_starttime() const {
	return starttime;
}

bool particle::is_fixed() const {
	return fixed;
}

size_t particle::get_index() const {
	return index;
}

} // -- namespace sim
