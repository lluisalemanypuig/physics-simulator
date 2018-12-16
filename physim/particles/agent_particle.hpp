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
 * @brief Class implementing an agent particle.
 *
 * An agent particle is useful for defining, using a physically-based
 * model, the movement of an agent (say, a human(oid)) through an
 * environment, in the presence of other agents and obstacles.
 *
 * Although it is a type of sized particle (see @ref sized_particle),
 * which in turn is a type of free_particle (see @ref free_particle),
 * its starttime (see @ref free_particle::starttime) is ignored.
 * Its lifetime, however, it is not (see @ref free_particle::lifetime).
 */
class agent_particle : public sized_particle {
	private:
		/**
		 * @brief Initialises this class's attributes.
		 *
		 * The attributes of the class take the following values:
		 * - @ref attractor : vec3(0,0,0)
		 * - @ref attractor_acceleration : 0.0
		 * - @ref max_vel : 1.0
		 */
		void partial_init();

	public:
		/**
		 * @brief Position of the attractor.
		 *
		 * An attractor is a force field applied only to this particle.
		 * It uses the @ref attractor_acceleration magnitude to compute the
		 * force it acts upon the particle. Let \f$s\f$ be the position
		 * of the attractor (sink), \f$p\f$ be the current position of
		 * this particle, and \f$a\f$ be the attractor acceleration. The
		 * force is computed as:
		 *
		 * \f$\vec{F} = a \cdot (s - p)\f$
		 *
		 * the agent is assumed to weigh 1 Kg.
		 */
		math::vec3 attractor;

		/// Acceleration of the attractor. [m/s^2]
		float attractor_acceleration;

		/// Maximum magnitude of this particle's velocity. [m/s].
		float max_vel;

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
		 * - @ref attractor : vec3(0,0,0)
		 * - @ref attractor_acceleration : 0.0
		 * - @ref max_vel : 1.0
		 */
		void init();

		// GETTERS

		virtual particle_type get_particle_type() const;
};

} // -- namespace particles
} // -- namespace physim
