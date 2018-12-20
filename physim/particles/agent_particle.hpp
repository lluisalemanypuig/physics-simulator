#pragma once

// C inlcudes
#include <stddef.h>

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
enum agent_behaviour_type {
	/// No behaviour active.
	none				= 1 << 0,
	/// Seek. The agent moves towards its target.
	seek				= 1 << 1,
	/// Flee. The agent moves away from its target.
	flee				= 1 << 2,
	/**
	 * @brief Arrival. The agent slows down as it approaches its target.
	 *
	 * The distance at which the agent starts to slow down is defined in
	 * @ref agent_particle::slowing_distance.
	 */
	arrival				= 1 << 3,
	/**
	 * @brief Collision avoidance.
	 *
	 * The agent will try to move away from an obstacle as it moves.
	 */
	collision_avoidance	= 1 << 4
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
		 * - @ref behaviour : @ref agent_behaviour_type::none
		 * - @ref max_speed : 1.0
		 * - @ref max_force : 1.0
		 * - @ref slowing_distance : 0.0
		 * - @ref seek_weight : 1.0
		 * - @ref flee_weight : 1.0
		 * - @ref arrival_weight : 1.0
		 * - @ref coll_avoid_weight : 1.0
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

		/**
		 * @brief Distance from the target to start slowing down at.
		 *
		 * When the agent is in behaviour arrival (see
		 * @ref agent_behaviour_type::arrival) it starts slow down. This
		 * attribute encodes the distance from its target at which this
		 * happens.
		 */
		float slowing_distance;

		/// Weight for seek behaviour.
		float seek_weight;
		/// Weight for flee behaviour.
		float flee_weight;
		/// Weight for arrival behaviour.
		float arrival_weight;
		/// Weight for collision avoidance behaviour.
		float coll_avoid_weight;

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
		 * - @ref behaviour : @ref agent_behaviour_type::none
		 * - @ref max_speed : 1.0
		 * - @ref max_force : 1.0
		 * - @ref slowing_distance : 0.0
		 * - @ref seek_weight : 1.0
		 * - @ref flee_weight : 1.0
		 * - @ref arrival_weight : 1.0
		 * - @ref coll_avoid_weight : 1.0
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
		 * - @ref agent_behaviour_type::flee
		 * - @ref agent_behaviour_type::arrival
		 *
		 * The resulting vector will be accumulated to an internal vector
		 * "as is" to make a new velocity vector.
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
		 * - @ref agent_behaviour_type::collision_avoidance.
		 *
		 * This function is called for every geometrical object in the
		 * scene. It is up to the user to decide what is a "most threatening"
		 * geometrical object.
		 *
		 * The resulting vector will be accumulated to an internal vector
		 * "as is" to make a new velocity vector.
		 *
		 * Recall that the target is stored in @ref target.
		 * @param[in] g Most threatening geometry.
		 * @param[out] weighted_steering Weighted steering vector.
		 */
		void apply_behaviours
		(const geometric::geometry *g, math::vec3& weighted_steering) const;

		/* steering behaviours */

		/**
		 * @brief Computes the seek steering force.
		 *
		 * This function must compute a velocity vector multiplied
		 * by a certain weight. The result must be assigned to @e v.
		 *
		 * @param[out] v Seek steering vector.
		 * @pre Vector @e v may not be initialised to 0.
		 */
		virtual void seek_behaviour(math::vec3& v) const;
		/**
		 * @brief Computes the flee steering force.
		 *
		 * This function must compute a velocity vector multiplied
		 * by a certain weight. The result must be assigned to @e v.
		 *
		 * @param[out] v Seek steering vector.
		 * @pre Vector @e v may not be initialised to 0.
		 */
		virtual void flee_behaviour(math::vec3& v) const;
		/**
		 * @brief Computes the arrival steering force.
		 *
		 * This function must compute a velocity vector multiplied
		 * by a certain weight. The result must be assigned to @e v.
		 *
		 * Recall that the slowing distance considered is stored
		 * in @ref v.
		 *
		 * @param[out] weighted_steering Seek steering vector.
		 * @pre Vector @e v may not be initialised to 0.
		 */
		virtual void arrival_behaviour(math::vec3& v) const;
		/**
		 * @brief Computes the collision avoidance steering force.
		 *
		 * This function must compute a velocity vector multiplied
		 * by a certain weight. The result must be assigned to @e v.
		 *
		 * @param[out] v Seek steering vector.
		 * @pre Vector @e v may not be initialised to 0.
		 */
		virtual void collision_avoidance_behaviour
		(const geometric::geometry *g, math::vec3& v) const;
};

} // -- namespace particles
} // -- namespace physim
