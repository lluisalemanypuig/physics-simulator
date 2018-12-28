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
		 */
		void partial_init();

	public:
		/// Density of the particle \f$\rho_i\f$. [Kg/m^3]
		float density;
		/// Pressure of the particle \f$p_i\f$. [N/m^2], [Kg/(m*s^2)]
		float pressure;

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
		 */
		virtual void init();

		virtual particle_type get_particle_type() const;

};

} // -- namespace particles
} // -- namespace sim
