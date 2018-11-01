#include <physim/particles/mesh_particle.hpp>

// C includes
#include <assert.h>

// physim includes
#include <physim/math/math.hpp>

namespace physim {
namespace particles {

// PRIVATE

void mesh_particle::init() {
	__pm_assign_s(prev_pos, 0.0f);
	__pm_assign_s(cur_velocity, 0.0f);
	__pm_assign_s(force, 0.0f);
	mass = 1.0f;
	charge = 0.0f;
	fixed = false;
}

// PUBLIC

mesh_particle::mesh_particle() {
	init();
	__pm_assign_s(cur_pos, 0.0f);
}

mesh_particle::mesh_particle(const mesh_particle& p) {
	__pm_assign_v(prev_pos, p.prev_pos);
	__pm_assign_v(cur_pos, p.cur_pos);
	__pm_assign_v(cur_velocity, p.cur_velocity);
	__pm_assign_v(force, p.force);

	mass = p.mass;
	charge = p.charge;
	fixed = p.fixed;
	index = p.index;
}

mesh_particle::~mesh_particle() { }

// MODIFIERS

void mesh_particle::translate(const math::vec3& v) {
	__pm_add_acc_v(cur_pos, v);
}

void mesh_particle::accelerate(const math::vec3& v) {
	__pm_add_acc_v(cur_velocity, v);
}

void mesh_particle::add_force(const float& x, const float& y, const float& z) {
	__pm_add_acc_c(force, x,y,z);
}
void mesh_particle::add_force(const math::vec3& f) {
	__pm_add_acc_v(force, f);
}

void mesh_particle::save_position() {
	__pm_assign_v(prev_pos, cur_pos);
}

// SETTERS

void mesh_particle::set_previous_position(const float& x, const float& y, const float& z) {
	__pm_assign_c(prev_pos, x,y,z);
}
void mesh_particle::set_previous_position(const math::vec3& p) {
	__pm_assign_v(prev_pos, p);
}

void mesh_particle::set_position(const float& x, const float& y, const float& z) {
	__pm_assign_c(cur_pos, x,y,z);
}
void mesh_particle::set_position(const math::vec3& p) {
	__pm_assign_v(cur_pos, p);
}

void mesh_particle::set_velocity(const float& x, const float& y, const float& z) {
	__pm_assign_c(cur_velocity, x,y,z);
}
void mesh_particle::set_velocity(const math::vec3& vel) {
	__pm_assign_v(cur_velocity, vel);
}

void mesh_particle::set_force(const float& x, const float& y, const float& z) {
	__pm_assign_c(force, x,y,z);
}
void mesh_particle::set_force(const math::vec3& f) {
	__pm_assign_v(force, f);
}

void mesh_particle::set_mass(float m) {
	mass = m;
}

void mesh_particle::set_charge(float c) {
	charge = c;
}

void mesh_particle::set_fixed(bool f) {
	fixed = f;
}

void mesh_particle::set_index(size_t i) {
	index = i;
}

// GETTERS

math::vec3& mesh_particle::get_previous_position() {
	return prev_pos;
}
const math::vec3& mesh_particle::get_previous_position() const {
	return prev_pos;
}

math::vec3& mesh_particle::get_position() {
	return cur_pos;
}
const math::vec3& mesh_particle::get_position() const {
	return cur_pos;
}

math::vec3& mesh_particle::get_velocity() {
	return cur_velocity;
}
const math::vec3& mesh_particle::get_velocity() const {
	return cur_velocity;
}

math::vec3& mesh_particle::get_force() {
	return force;
}
const math::vec3& mesh_particle::get_force() const {
	return force;
}

float mesh_particle::get_mass() const {
	return mass;
}

float mesh_particle::get_charge() const {
	return charge;
}

bool mesh_particle::is_fixed() const {
	return fixed;
}

size_t mesh_particle::get_index() const {
	return index;
}

} // -- namespace particles
} // -- namespace physim
