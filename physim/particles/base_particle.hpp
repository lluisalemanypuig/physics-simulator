#pragma once

// C inlcudes
#include <stddef.h>

// physim includes
#include <physim/particles/particle_types.hpp>
#include <physim/math/vec3.hpp>

namespace physim {
namespace particles {

/**
 * @brief Class implementing the most basic particle.
 *
 * The most basic particle in this library is a 0-dimensional
 * object that is subject to several forces but not to any
 * other particle's direct interaction through them. It can
 * collide with other objects in the scene (geometrical objects,
 * see namespace @ref physim::geom) and maybe with other
 * particles, depending on the subclass.
 *
 * All this class' attributes are public.
 */
class base_particle {
	private:
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
		base_particle();
		/// Copy constructor.
		base_particle(const base_particle& p);
		/// Destructor.
		virtual ~base_particle();

		// MODIFIERS

		/**
		 * @brief Saves the current position in the particle's state.
		 *
		 * Copies @ref cur_pos into @ref prev_pos.
		 */
		void save_position();

		/**
		 * @brief Initialises all particle's attributes, most of them
		 * to null values.
		 *
		 * The attributes of the class take the following values:
		 * - @ref prev_pos : vec3(0,0,0)
		 * - @ref cur_pos : vec3(0,0,0)
		 * - @ref cur_vel : vec3(0,0,0)
		 * - @ref force : vec3(0,0,0)
		 * - @ref mass : 1
		 * - @ref index : no value assigned, since it will be
		 * overwritten by the simulator.
		 *
		 * The current position (@ref cur_pos) is not initialised
		 * since it will be overwritten later in the methods that
		 * also call this one. The other attributes, not mentioned
		 * in the list above, are not modified either for similar
		 * reasons.
		 */
		virtual void init();

		/// Returns the type of this particle.
		virtual particle_type get_particle_type() const;

};

} // -- namespace particles
} // -- namespace sim
