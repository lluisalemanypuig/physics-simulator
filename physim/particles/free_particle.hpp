#pragma once

// C inlcudes
#include <stddef.h>

// physim includes
#include <physim/particles/base_particle.hpp>
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
 * All this class' attributes are public.
 */
class free_particle : public base_particle {
	private:
	public:
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

	public:
		/// Default constructor.
		free_particle();
		/// Copy constructor.
		free_particle(const free_particle& p);
		/// Destructor.
		virtual ~free_particle();

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
		 * @brief Initialises all particle's attributes, most of them
		 * to null values.
		 *
		 * The attributes of the class take the following values:
		 * - @ref prev_pos : vec3(0,0,0)
		 * - @ref cur_vel : vec3(0,0,0)
		 * - @ref force : vec3(0,0,0)
		 * - @ref mass : 1
		 * - @ref bouncing : 0.8
		 * - @ref friction : 0.2
		 * - @ref charge : 0
		 * - @ref lifetime : 10
		 * - @ref starttime : 0
		 * - @ref fixed : false
		 *
		 * The current position (@ref cur_pos) is not initialised
		 * since it will be overwritten later in the methods that
		 * also call this one. The other attributes, not mentioned
		 * in the list above, are not modified either for similar
		 * reasons.
		 */
		virtual void init();

		// GETTERS

		virtual particle_type get_particle_type() const;

};

} // -- namespace particles
} // -- namespace sim
