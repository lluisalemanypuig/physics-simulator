#pragma once

// C inlcudes
#include <stddef.h>

// physim includes
#include <physim/particles/particle_types.hpp>
#include <physim/particles/sized_particle.hpp>
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
 * [1]: Steering Behaviors For Autonomous Characters.\n
 *		Craig W. Reynolds, Sony Computer Entertainment America\n
 *		919 East Hillsdale Boulevard\n
 *		Foster City, California 94404\n
 * Available online at:\n
 *		https://www.red3d.com/cwr/papers/1999/gdc99steer.html
 */
enum class agent_behaviour_type : int8_t {
	/// Seek. The agent moves towards its target.
	seek = 0,
	/// Flee. The agent moves away from its target.
	flee,
	/**
	 * @brief Arrival. The agent slows down as it approaches its target.
	 *
	 * The distance at which the agent starts to slow down is defined in
	 * @ref agent_particle::slowing_distance.
	 */
	arrival
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
		 * - @ref max_speed : 1.0
		 * - @ref max_force : 1.0
		 * - @ref slowing_distance : 0.0
		 * - @ref behaviour : @ref agent_behaviour_type::seek
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

		/// Behaviour of this agent.
		agent_behaviour_type behaviour;

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
		 * - @ref max_speed : 1.0
		 * - @ref max_force : 1.0
		 * - @ref slowing_distance : 0.0
		 * - @ref behaviour : @ref agent_behaviour_type::seek
		 */
		void init();

		// GETTERS

		virtual particle_type get_particle_type() const;
};

} // -- namespace particles
} // -- namespace physim
