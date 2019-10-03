/*********************************************************************
 * Real-time physics simulation project
 * Copyright (C) 2018-2019 Lluís Alemany Puig
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * Contact: Lluís Alemany Puig (lluis.alemany.puig@gmail.com)
 * 
 ********************************************************************/

#pragma once

// C inlcudes
#include <stddef.h>

// C++ includes
#include <vector>

// physim includes
#include <physim/particles/particle_types.hpp>
#include <physim/particles/sized_particle.hpp>
#include <physim/geometry/geometry.hpp>
#include <physim/math/vec3.hpp>

namespace physim {
namespace particles {

/**
 * @brief The different types of behaviours for an agent.
 *
 * All of these types define types of movement. To make a
 * particle stay put (i.e., not to move at all) set its
 * @ref free_particle::fixed attribute to 'true'.
 *
 * The different behaviour types are formally defined in [1].
 * All of them are strongly dependent on the particle's target
 * (see @ref agent_particle::target).
 *
 * Behaviours can be compined using the bit-wise or operator.
 *
 * [1]: Steering Behaviors For Autonomous Characters.\n
 *		Craig W. Reynolds, Sony Computer Entertainment America\n
 *		919 East Hillsdale Boulevard\n
 *		Foster City, California 94404\n
 * Available online at:\n
 *		https://www.red3d.com/cwr/papers/1999/gdc99steer.html
 */
enum class agent_behaviour_type : int8_t {
	/// No behaviour active.
	none = 1 << 0,
	/**
	 * @brief Seek. The agent moves towards its target.
	 *
	 * Its weight is @ref agent_particle::seek_weight.
	 */
	seek = 1 << 1,
	/**
	 * @brief Flee. The agent moves away from its target.
	 *
	 * Its weight is @ref agent_particle::flee_weight.
	 */
	flee = 1 << 2,
	/**
	 * @brief Arrival. The agent slows down as it approaches its target.
	 *
	 * The distance at which the agent starts to slow down is defined in
	 * @ref agent_particle::arrival_distance.
	 *
	 * Its weight is @ref agent_particle::arrival_weight.
	 */
	arrival = 1 << 3,
	/**
	 * @brief Collision avoidance.
	 *
	 * The agent will try to move away from an obstacle as it moves.
	 *
	 * Its weight is @ref agent_particle::coll_weight.
	 */
	collision_avoidance = 1 << 4,
	/**
	 * @brief Unaligned collision avoidance.
	 *
	 * Agent try to avoid colliding with other agents.
	 *
	 * Its weight is @ref agent_particle::ucoll_weight.
	 */
	unaligned_collision_avoidance = 1 << 5,
	/**
	 * @brief Walk off with.
	 *
	 * Agents nearby will try to align their velocities so that
	 * it looks like they are walking together.
	 */
	walk_with_me = 1 << 6,
};

/**
 * @brief Class implementing an agent particle.
 *
 * An agent particle is useful for defining, using a physically-based
 * model, the movement of an agent (say, a human(oid), car, ...) through
 * an environment, in the presence of other agents and obstacles.
 *
 * Although it is a type of sized particle (see @ref sized_particle),
 * which in turn is a type of free particle (see @ref free_particle),
 * its starting time (see @ref free_particle::starttime) is ignored.
 * Its lifetime, however, it is not (see @ref free_particle::lifetime).
 * The simulation of an agent particle depend on its behaviour
 * (see @ref agent_particle::behaviour).
 */
class agent_particle : public sized_particle {
	private:
		/**
		 * @brief Initialises this class's attributes.
		 *
		 * The attributes of the class take the following values:
		 * - @ref target : vec3(0,0,0)
		 * - @ref orientation : vec3(0,0,0)
		 * - @ref behaviour : @ref agent_behaviour_type::none
		 * - @ref max_speed : 1.0
		 * - @ref max_force : 1.0
		 * - @ref align_weight : 1.0/7.0
		 * - @ref seek_weight : 1.0/7.0
		 * - @ref flee_weight : 1.0/7.0
		 * - @ref arrival_weight : 1.0/7.0
		 * - @ref arrival_distance : 0.0
		 * - @ref coll_weight : 1.0/7.0
		 * - @ref coll_distance : 5.0
		 * - @ref wow_weight : 1.0/7.0
		 * - @ref wow_distance : 5.0
		 * - @ref ucoll_weight : 1.0/7.0
		 * - @ref ucoll_distance : 5.0
		 */
		void partial_init();

	public:
		/**
		 * @brief Target position of this particle.
		 *
		 * A target is a position this agent particle will want to move
		 * to or away from. This will depend on the behaviour of this
		 * particle. See @ref behaviour.
		 */
		math::vec3 target;

		/**
		 * @brief Vector indicating the orientation of the particle.
		 *
		 * This need not be equal to the @ref cur_vel vector,
		 * but it should be initialised with a value equal to the
		 * velocity.
		 */
		math::vec3 orientation;

		/// Behaviour of this agent.
		agent_behaviour_type behaviour;

		/**
		 * @brief Maximum speed allowed. [m/s].
		 *
		 * This particle can move at a velocity with
		 * maximum magnitude stored in this value.
		 */
		float max_speed;
		/**
		 * @brief Maximum force allowed. [m/s].
		 *
		 * This particle can move at a velocity with
		 * maximum magnitude stored in this value.
		 */
		float max_force;

		/// Weight for aligning orientation with velocity.
		float align_weight;
		/// Weight for seek behaviour.
		float seek_weight;
		/// Weight for flee behaviour.
		float flee_weight;
		/// Weight for arrival behaviour.
		float arrival_weight;
		/// Distance from the target to start slowing down at in arrival behaviour.
		float arrival_distance;
		/// Weight for collision avoidance behaviour.
		float coll_weight;
		/// Distance ahead of the agent for collision avoidance.
		float coll_distance;
		/// Weight for unaligned collision avoidance behaviour.
		float ucoll_weight;
		/// Distance ahead of the agent for unaligned collision avoidance.
		float ucoll_distance;
		/// Weight for 'walk with me' behaviour.
		float wow_weight;
		/// Distance ahead of the agent for 'walk with me' behaviour.
		float wow_distance;

	public:
		/// Default constructor.
		agent_particle();
		/// Copy constructor.
		agent_particle(const agent_particle& p);
		/// Destructor.
		virtual ~agent_particle();

		/**
		 * @brief Initialises all particle's attributes, most of them
		 * to null values.
		 *
		 * The attributes of the class take the following values:
		 * - @ref prev_pos : vec3(0,0,0)
		 * - @ref cur_vel : vec3(0,0,0)
		 * - @ref force : vec3(0,0,0)
		 * - @ref mass : 1
		 * - @ref index : no value assigned, since it will be
		 * overwritten by the simulator.
		 * - @ref bouncing : 0.8
		 * - @ref friction : 0.2
		 * - @ref charge : 0
		 * - @ref lifetime : 10
		 * - @ref starttime : 0
		 * - @ref fixed : false
		 * - @ref R : 1.0
		 * - @ref target : vec3(0,0,0)
		 * See @ref partial_init() to see how the attributes of this class
		 * are intialised.
		 */
		void init();

		// GETTERS

		virtual particle_type get_particle_type() const;

		/// Returns whether behaviour @e b is activated or not.
		bool is_behaviour_set(const agent_behaviour_type& b) const;

		// SETTERS

		/**
		 * @brief Sets a type of behaviour.
		 *
		 * Attribute @ref behaviour is modified so that the evaluation of
		 \verbatim
		 behaviour | b
		 \endverbatim
		 * results to a value different from 0, where @e b is the behaviour
		 * type passed as parameter.
		 * @param b Type of behaviour.
		 */
		void set_behaviour(const agent_behaviour_type& b);
		/**
		 * @brief Sets a type of behaviour.
		 *
		 * Attribute @ref behaviour is modified so that the evaluation of
		 \verbatim
		 behaviour | b
		 \endverbatim
		 * results to a value equal to 0, where @e b is the behaviour
		 * type passed as parameter.
		 * @param b Type of behaviour.
		 */
		void unset_behaviour(const agent_behaviour_type& b);

		/**
		 * @brief Unsets all behavours of the agent.
		 *
		 * Sets its behaviour to @ref agent_behaviour_type::none.
		 */
		void unset_all_behaviours();

		// OTHERS

		/**
		 * @brief Computes a weighted steering vector force.
		 *
		 * This vector is the weighted average over the different steering
		 * vectors obtained by the different behaviours activated.
		 *
		 * The behaviours considered in this function are:
		 * - @ref agent_behaviour_type::seek
		 * (function @ref seek_behaviour(math::vec3&)const ).
		 * - @ref agent_behaviour_type::flee
		 * (function @ref flee_behaviour(math::vec3&)const ).
		 * - @ref agent_behaviour_type::arrival
		 * (function @ref arrival_behaviour(math::vec3&)const ).
		 *
		 * The resulting vector of each behaviour will be accumulated to the
		 * output vector of this function.
		 *
		 * Recall that the target is stored in @ref target.
		 * @param[out] weighted_steering Weighted steering vector.
		 */
		void apply_behaviours(math::vec3& weighted_steering) const;

		/**
		 * @brief Computes a weighted steering vector force.
		 *
		 * This vector is the weighted average over the different steering
		 * vectors obtained by the different behaviours activated.
		 *
		 * The behaviours considered in this function are:
		 * - @ref agent_behaviour_type::collision_avoidance (function
		 * @ref collision_avoidance_behaviour
		 * (const std::vector<geometric::geometry *>&, math::vec3&)const ).
		 *
		 * The resulting vector of each behaviour will be accumulated to the
		 * output vector of this function.
		 *
		 * Recall that the target is stored in @ref target.
		 * @param[in] scene The geometry in the simulation.
		 * @param[out] weighted_steering Weighted steering vector.
		 */
		void apply_behaviours
		(const std::vector<geometric::geometry *>& scene,
		 math::vec3& weighted_steering) const;

		/**
		 * @brief Computes a weighted steering vector force.
		 *
		 * This vector is the weighted average over the different steering
		 * vectors obtained by the different behaviours activated.
		 *
		 * The behaviours considered in this function are:
		 * - @ref agent_behaviour_type::unaligned_collision_avoidance (function
		 * @ref unaligned_collision_avoidance_behaviour
		 * (const std::vector<agent_particle>&, math::vec3&)const ).
		 *
		 * The resulting vector of each behaviour will be accumulated to the
		 * output vector of this function.
		 *
		 * Recall that the index of this agent particle is stored in @ref index.
		 * @param[in] agents All the agents in the simulation.
		 * @param[out] weighted_steering Weighted steering vector.
		 */
		void apply_behaviours
		(const std::vector<agent_particle>& agents,
		 math::vec3& weighted_steering) const;

		/* steering behaviours */

		/**
		 * @brief Computes the seek steering force.
		 *
		 * This function must compute a velocity vector multiplied
		 * by weight @ref seek_weight. The result must be assigned to @e v.
		 *
		 * @param[out] v Seek steering vector.
		 * @pre Vector @e v may not be initialised to 0.
		 */
		virtual void seek_behaviour(math::vec3& v) const;
		/**
		 * @brief Computes the flee steering force.
		 *
		 * This function must compute a velocity vector multiplied
		 * by weight @ref flee_weight. The result must be assigned to @e v.
		 *
		 * @param[out] v Flee steering vector.
		 * @pre Vector @e v may not be initialised to 0.
		 */
		virtual void flee_behaviour(math::vec3& v) const;
		/**
		 * @brief Computes the arrival steering force.
		 *
		 * This function must compute a velocity vector multiplied
		 * by weight @ref arrival_weight. The result must be assigned to @e v.
		 *
		 * Recall that the slowing distance considered is stored
		 * in @e v.
		 *
		 * @param[out] v Arrival steering vector.
		 * @pre Vector @e v may not be initialised to 0.
		 */
		virtual void arrival_behaviour(math::vec3& v) const;
		/**
		 * @brief Computes the collision avoidance steering force.
		 *
		 * This function must compute a velocity vector multiplied
		 * by weight @ref coll_weight. The result must be assigned to @e v.
		 *
		 * Rectangles are considered walls. The rest of the geometry
		 * is approximated with spheres.
		 *
		 * @param[in] scene The geometry in the simulation.
		 * @param[out] v Collision avoidance steering vector.
		 * @pre Vector @e v may not be initialised to 0.
		 */
		virtual void collision_avoidance_behaviour
		(const std::vector<geometric::geometry *>& scene, math::vec3& v) const;
		/**
		 * @brief Computes the collision avoidance steering force.
		 *
		 * This function must compute a velocity vector multiplied
		 * by weight @ref ucoll_weight. The result must be assigned to @e v.
		 *
		 * The vector computed is meant to provide the agent a means of
		 * avoiding collision with other agents 'locally', that is, the
		 * ability of choosing what to do without the help of an oracle.
		 *
		 * Only some agents are considered. Those ignored are listed below.
		 * FOV stands for Field of View, it is aligned at this agent's
		 * velocity vector and is of 90º to its left and 90º to its right.
		 *
		 * The following agents are ignored:
		 * - agents too far away (the distance between the agents is more
		 * than @ref ucoll_distance, where the distance considered is
		 * the distance between the current positions minus the sum of both
		 * agent's radius)
		 * - agents whose predicted position is outside the FOV.
		 * - agents whose current position is outside the FOV.
		 *
		 * @param[in] agents All the agents in the simulation.
		 * @param[out] v Unaligned collision avoidance steering vector.
		 * @pre Vector @e v may not be initialised to 0.
		 */
		virtual void unaligned_collision_avoidance_behaviour
		(const std::vector<agent_particle>& agents, math::vec3& v) const;
		/**
		 * @brief Computes the "walk off with" steering force.
		 *
		 * This function must compute a velocity vector multiplied
		 * by weight @ref wow_weight. The result must be assigned to @e v.
		 *
		 * The vector computed is meant to provide the agent a means of
		 * walking together with another agent with similar velocity direction.
		 *
		 *  Only some agents are considered. Those ignored are listed below.
		 * FOV stands for Field of View, it is aligned at this agent's
		 * velocity vector and is of 90º to its left and 90º to its right.
		 *
		 * The following agents are ignored:
		 * - agents too far away (the distance between the agents is more
		 * than @ref wow_distance, where the distance considered is
		 * the distance between the current positions minus the sum of both
		 * agent's radius)
		 * - agents whose predicted position is outside the FOV.
		 * - agents whose current position is outside the FOV.
		 *
		 * @param[in] agents All the agents in the simulation.
		 * @param[out] v "Walk off with" steering vector.
		 * @pre Vector @e v may not be initialised to 0.
		 */
		virtual void wwm_behaviour
		(const std::vector<agent_particle>& agents, math::vec3& v) const;
};

} // -- namespace particles
} // -- namespace physim
