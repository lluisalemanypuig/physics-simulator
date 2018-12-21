#pragma once

// C inlcudes
#include <stddef.h>

// physim includes
#include <physim/particles/particle_types.hpp>
#include <physim/particles/base_particle.hpp>
#include <physim/math/vec3.hpp>

namespace physim {
namespace particles {

/**
 * @brief Class implementing a fluid particle.
 *
 * A particle is a 0-dimensional object, subject to
 * several forces. It can also collide with other
 * objects in the scene (geometrical objects, see
 * namespace @ref physim::geometric) but not with other
 * particles.
 */
class fluid_particle : public base_particle {
	private:
		/**
		 * @brief Initialises this class's attributes.
		 *
		 * The attributes of the class take the following values:
		 * - @ref density : 0.0
		 * - @ref pressure : 0.0
		 * - @ref lifetime : 10.0
		 * - @ref starttime : 0.0
		 */
		void partial_init();

	public:
		/// Density of the particle [Kg/m^3].
		float density;
		/// Pressure of the particle [N/m^2].
		float pressure;
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

	public:
		/// Default constructor.
		fluid_particle();
		/// Copy constructor.
		fluid_particle(const fluid_particle& p);
		/// Destructor.
		virtual ~fluid_particle();

		// MODIFIERS

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
		 * - @ref density : 0.0
		 * - @ref pressure : 0.0
		 * - @ref lifetime : 10.0
		 * - @ref starttime : 0.0
		 */
		virtual void init();

		virtual particle_type get_particle_type() const;

};

} // -- namespace particles
} // -- namespace sim
