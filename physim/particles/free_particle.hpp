#pragma once

// C inlcudes
#include <stddef.h>

// physim includes
#include <physim/math/vec3.hpp>

namespace physim {
namespace particles {

/**
 * @brief Class implementing a free particle.
 *
 * A free particle is a 0-dimensional object, subject
 * to several forces but not to any other particle's
 * direct interaction through them. It can collide with
 * other objects in the scene (geometrical objects, see
 * namespace @ref physim::geom) but not with other
 * particles.
 *
 * Although this class' attributes are public, it provides
 * a number of methods to 'set' and 'get' their values for
 * a more user-friendly usage.
 */
class free_particle {
	private:

		/**
		 * @brief Initialises all particle's attributes, most of them
		 * to null values.
		 *
		 * The attributes of the class take the following values:
		 * - @ref prev_pos : vec3(0,0,0)
		 * - @ref cur_velocity : vec3(0,0,0)
		 * - @ref force : vec3(0,0,0)
		 * - @ref mass : 1
		 * - @ref bouncing : 1
		 * - @ref friction : 0
		 * - @ref charge : 0
		 * - @ref lifetime : 10
		 * - @ref starttime : 0
		 * - @ref fixed : false
		 * - @ref index : no value assigned, since it will be
		 * overwritten by the simulator.
		 *
		 * The current position (@ref cur_pos) is not initialised
		 * since it will be overwritten later in the methods that
		 * also call this one.
		 */
		void init();

	public:
		/// Previous position of the particle [m].
		math::vec3 prev_pos;
		/// Current position of the particle [m].
		math::vec3 cur_pos;
		/// Current velocity of the particle [m/s].
		math::vec3 cur_vel;
		/// Force currently applied to the particle [N].
		math::vec3 force;

		/// Mass of the particle [Kg].
		float mass;
		/// Bouncing coefficient of the particle.
		float bouncing;
		/// Friction coefficient of the particle.
		float friction;
		/// Electrical charge of the particle [C].
		float charge;
		/**
		 * @brief Lifetime of the particle [s].
		 *
		 * Once the simulation has run for a time larger than
		 * @ref lifetime the particle must be reset.
		 */
		float lifetime;
		/**
		 * @brief Starting time of a particle [s].
		 *
		 * The amount of time that has to pass for this particle
		 * to start 'living'. In the simulation, this particle
		 * will not start moving until this value is equal to or
		 * less than 0.
		 */
		float starttime;
		/**
		 * @brief Is this particle fixed?
		 *
		 * If the particle, it will be ignored by the solver, therefore
		 * not taken into account in the simulation (gravity nor any
		 * other force will have any effect on it).
		 */
		bool fixed;

		/**
		 * @brief Index of the particle.
		 *
		 * This index is automatically set when added to the
		 * simulator object. The collection of indexes determine
		 * the order in which particles have been added to it.
		 * The indexes start at 0.
		 *
		 * It can be used to initialise its attributes through
		 * the @ref init::initialiser class.
		 */
		size_t index;

	public:
		/// Default constructor.
		free_particle();
		/// Copy constructor.
		free_particle(const free_particle& p);
		/// Destructor.
		~free_particle();

		// MODIFIERS

		/**
		 * @brief Decreases the lifetime by @e t.
		 *
		 * @param t A value equal to or greater than 0.
		 */
		void reduce_lifetime(float t);
		/**
		 * @brief Decreases the starttime by @e t.
		 *
		 * @param t A value equal to or greater than 0.
		 */
		void reduce_starttime(float t);

		/**
		 * @brief Saves the current position in the particle's state.
		 *
		 * Copies @ref cur_pos into @ref prev_pos.
		 */
		void save_position();

};

} // -- namespace particles
} // -- namespace sim
