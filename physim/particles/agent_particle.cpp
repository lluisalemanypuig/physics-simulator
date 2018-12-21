#include <physim/particles/agent_particle.hpp>

// C++ includes
#include <algorithm>
#include <vector>
using namespace std;

// physim includes
#include <physim/math/private/math3.hpp>

#define FULL_MASK static_cast<int>(0xffffffff)

namespace physim {
using namespace math;
using namespace geometric;

namespace particles {

// PRIVATE

void agent_particle::partial_init() {
	__pm3_assign_s(target, 0.0f);
	behaviour = agent_behaviour_type::none;
	max_speed = 1.0f;
	max_force = 1.0f;
	slowing_distance = 0.0f;

	float w = 1.0f/4.0f;
	seek_weight = w;
	flee_weight = w;
	arrival_weight = w;
	coll_avoid_weight = w;
}

// PUBLIC

agent_particle::agent_particle() {
	partial_init();
}

agent_particle::agent_particle(const agent_particle& p) : sized_particle(p) {
	__pm3_assign_v(target, p.target);
	behaviour = p.behaviour;
	max_speed = p.max_speed;
	max_force = p.max_force;
	slowing_distance = p.slowing_distance;

	seek_weight = p.seek_weight;
	flee_weight = p.flee_weight;
	arrival_weight = p.arrival_weight;
	coll_avoid_weight = p.coll_avoid_weight;
}

agent_particle::~agent_particle() {
}

void agent_particle::init() {
	base_particle::init();
	partial_init();
}

// GETTERS

particle_type agent_particle::get_particle_type() const {
	return particle_type::agent_particle;
}

bool agent_particle::is_behaviour_set(const agent_behaviour_type& b) const {
	int k = static_cast<int>(behaviour) & static_cast<int>(b);
	return k != 0;
}

// SETTERS

void agent_particle::set_behaviour(const agent_behaviour_type& b) {
	int k = static_cast<int>(behaviour) | static_cast<int>(b);
	behaviour = static_cast<agent_behaviour_type>(k);
}

void agent_particle::unset_behaviour(const agent_behaviour_type& b) {
	int k = static_cast<int>(behaviour) & (FULL_MASK - static_cast<int>(b));
	behaviour = static_cast<agent_behaviour_type>(k);
}

void agent_particle::unset_all_behaviours() {
	behaviour = agent_behaviour_type::none;
}

// OTHERS

/* steering behaviours */

void agent_particle::apply_behaviours(vec3& weighted_steering) const {
	if (behaviour == agent_behaviour_type::none) {
		return;
	}

	vec3 v;

	if (is_behaviour_set(agent_behaviour_type::seek)) {
		seek_behaviour(v);
		__pm3_add_acc_v(weighted_steering, v);
	}
	if (is_behaviour_set(agent_behaviour_type::flee)) {
		flee_behaviour(v);
		__pm3_add_acc_v(weighted_steering, v);
	}
	if (is_behaviour_set(agent_behaviour_type::arrival)) {
		arrival_behaviour(v);
		__pm3_add_acc_v(weighted_steering, v);
	}
}

void agent_particle::apply_behaviours
(const vector<geometry *>& scene, vec3& weighted_steering)
const
{
	if (behaviour == agent_behaviour_type::none) {
		return;
	}

	vec3 v;

	if (is_behaviour_set(agent_behaviour_type::collision_avoidance)) {
		collision_avoidance_behaviour(scene, weighted_steering);
		__pm3_add_acc_v(weighted_steering, v);
	}
}

void agent_particle::seek_behaviour(vec3& v) const {
	vec3 des_vel;
	__pm3_sub_v_v(des_vel, target, cur_pos);
	normalise(des_vel, des_vel);
	__pm3_mul_acc_s(des_vel, max_speed);

	/*
	vec3 steer;
	__pm3_sub_v_v(steer, des_vel, cur_vel);
	__pm3_assign_vs(v, steer, seek_weight);
	*/
	__pm3_sub_v_v_mul_s(v, des_vel, cur_vel, seek_weight);
}

void agent_particle::flee_behaviour(vec3& v) const {
	vec3 des_vel;
	__pm3_sub_v_v(des_vel, cur_pos, target);
	normalise(des_vel, des_vel);
	__pm3_mul_acc_s(des_vel, max_speed);

	/*
	vec3 steer;
	__pm3_sub_v_v(steer, des_vel, cur_vel);
	__pm3_assign_vs(v, steer, flee_weight);
	*/
	__pm3_sub_v_v_mul_s(v, des_vel, cur_vel, flee_weight);
}

void agent_particle::arrival_behaviour(vec3& v) const {
	vec3 offset_target;
	__pm3_sub_v_v(offset_target, target, cur_pos);
	float dist_target = __pm3_norm(offset_target);
	float ramped_distance = (dist_target/slowing_distance)*max_speed;
	float clipped_speed = std::min(ramped_distance, max_speed);

	vec3 des_vel;
	__pm3_assign_vs(des_vel, offset_target, clipped_speed/dist_target);

	/*
	vec3 steer;
	__pm3_sub_v_v(steer, des_vel, cur_vel);
	__pm3_assign_vs(v, steer, arrival_weight);
	*/
	__pm3_sub_v_v_mul_s(v, des_vel, cur_vel, arrival_weight);
}

void agent_particle::collision_avoidance_behaviour
(const vector<geometry *>& scene, vec3& v)
const
{

}

} // -- namespace particles
} // -- namespace physim
