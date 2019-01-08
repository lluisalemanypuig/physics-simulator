#include <physim/particles/agent_particle.hpp>

// C++ includes
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

// physim includes
#include <physim/math/private/math3.hpp>
#include <physim/geometry/rectangle.hpp>

#define FULL_MASK static_cast<int>(0xffffffff)
#define ninety static_cast<float>(M_PI)/2.0f

namespace physim {
using namespace math;
using namespace geometric;

namespace particles {

// PRIVATE

void agent_particle::partial_init() {
	__pm3_assign_s(target, 0.0f);
	__pm3_assign_s(orientation, 0.0f);
	behaviour = agent_behaviour_type::none;
	max_speed = 1.0f;
	max_force = 1.0f;
	arrival_distance = 0.0f;

	coll_distance = 5.0f;
	ucoll_distance = 5.0f;

	float w = 1.0f/6.0f;
	align_weight = w;
	seek_weight = w;
	flee_weight = w;
	arrival_weight = w;
	coll_weight = w;
	ucoll_weight = w;
}

// PUBLIC

agent_particle::agent_particle() {
	partial_init();
}

agent_particle::agent_particle(const agent_particle& p) : sized_particle(p) {
	__pm3_assign_v(target, p.target);
	__pm3_assign_v(orientation, p.orientation);
	behaviour = p.behaviour;
	max_speed = p.max_speed;
	max_force = p.max_force;
	arrival_distance = p.arrival_distance;

	align_weight = p.align_weight;
	seek_weight = p.seek_weight;
	flee_weight = p.flee_weight;
	arrival_weight = p.arrival_weight;
	coll_weight = p.coll_weight;
	ucoll_weight = p.ucoll_weight;
	coll_distance = p.coll_distance;
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
(const std::vector<geometric::geometry *>& scene, vec3& weighted_steering)
const
{
	if (behaviour == agent_behaviour_type::none) {
		return;
	}

	vec3 v;

	if (is_behaviour_set(agent_behaviour_type::collision_avoidance)) {
		collision_avoidance_behaviour(scene, v);
		__pm3_add_acc_v(weighted_steering, v);
	}
}

void agent_particle::apply_behaviours
(const vector<agent_particle>& agents, vec3& weighted_steering)
const
{
	if (behaviour == agent_behaviour_type::none) {
		return;
	}

	vec3 v;

	if (is_behaviour_set(agent_behaviour_type::unaligned_collision_avoidance)) {
		unaligned_collision_avoidance_behaviour(agents, v);
		__pm3_add_acc_v(weighted_steering, v);
	}
}

/* steering behaviours */

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
	float ramped_distance = (dist_target/arrival_distance)*max_speed;
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
	__pm3_assign_s(v, 0.0f);
	for (const geometry *g : scene) {
		// skip distant objects
		vec3 geom_pos = g->get_box_center();
		float dist2;
		if (g->get_geom_type() == geometry_type::Rectangle) {
			// consider distance to object
			const rectangle *r = static_cast<const rectangle *>(g);
			dist2 = r->distance(cur_pos);
			dist2 = dist2*dist2;
		}
		else if (g->get_geom_type() == geometry_type::Plane) {
			// consider distance to object
			const plane *p = static_cast<const plane *>(g);
			dist2 = p->dist_point_plane(cur_pos);
			dist2 = dist2*dist2;
		}
		else {
			dist2 = __pm3_dist2(cur_pos, geom_pos) - g->approx_radius();
		}
		if (dist2 > coll_distance*coll_distance) {
			continue;
		}

		bool skip = false;

		// decide, using a second criteria, whether
		// this geometry should be skipped or not
		vec3 repulsion;
		if (g->get_geom_type() == geometry_type::Rectangle) {
			// this is a wall
			const plane& p = static_cast<const rectangle *>(g)->get_plane();
			__pm3_assign_v(repulsion, p.get_normal());
			if (p.dist_point_plane(cur_pos) < 0.0f) {
				__pm3_invert(repulsion, repulsion);
			}
			float angle = __pm3_angle(repulsion, cur_vel);
			if (angle < 1.571f) {
				// the '< 1.571' is counterintuitive, but just
				// do a little drawing and you will know why
				skip = true;
			}
		}
		else if (g->get_geom_type() == geometry_type::Plane) {
			// this is a wall
			const plane *p = static_cast<const plane *>(g);
			__pm3_assign_v(repulsion, p->get_normal());
			if (p->dist_point_plane(cur_pos) < 0.0f) {
				__pm3_invert(repulsion, repulsion);
			}
			float angle = __pm3_angle(repulsion, cur_vel);
			if (angle < 1.571f) {
				// the '< 1.571' is counterintuitive, but just
				// do a little drawing and you will know why
				skip = true;
			}
		}
		else {
			// project 'geom_pos' onto line through current
			// position and director vector current velocity
			vec3 agent_to_object = geom_pos - cur_pos;
			float angle = __pm3_angle(agent_to_object, cur_vel);
			if (angle > 1.571f) {
				skip = true;
			}

			// use cross products to obtain repulsion vector
			// (only if not skip)
			if (not skip) {
				vec3 c;
				__pm3_cross(c, cur_vel, agent_to_object);
				__pm3_cross(repulsion, cur_vel, c);
				normalise(repulsion, repulsion);
			}
		}

		if (skip) {
			continue;
		}

		// compute contribution
		__pm3_add_acc_vs(v, repulsion, coll_weight);
	}
}

#define cosangle3d(u,v) __pm3_dot(u,v)/(__pm3_norm(u)*__pm3_norm(v))
#define angle3d(u,v) std::acos(cosangle3d(u,v))

void agent_particle::unaligned_collision_avoidance_behaviour
(const vector<agent_particle>& agents, vec3& v) const
{
	vec3 X;
	vec3 pos_rep, vel_rep;

	__pm3_assign_s(v, 0.0f);
	float r = ucoll_distance;

	for (const agent_particle& a : agents) {
		if (index == a.index) {
			continue;
		}
		float D = __pm3_dist(cur_pos, a.cur_pos);
		float d = D - (R + a.R);
		if (d > r) {
			continue;
		}

		vec3 pos_dir;
		__pm3_sub_v_v(pos_dir, a.cur_pos, cur_pos);
		float vta_angle = angle3d(pos_dir, cur_vel);
		if (vta_angle > ninety) {
			continue;
		}

		vec3 pred_agent;
		__pm3_add_v_vs(pred_agent, a.cur_pos, a.cur_vel, 0.5f);
		float pred_angle = angle3d(cur_vel, pos_dir);
		if (pred_angle > ninety) {
			continue;
		}

		// copmute repulsion using vector from this position
		// to the agent's position and this velocity
		__pm3_cross(X, pos_dir, cur_vel);
		__pm3_cross(pos_rep, X,cur_vel);
		math::normalise(pos_rep, pos_rep);

		__pm3_add_acc_vs(v, pos_rep, ucoll_weight);
	}
}

} // -- namespace particles
} // -- namespace physim
