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
		 * See @ref partial_init() to see how the attributes of this class
		 * are intialised.
		 */
		virtual void init();

		virtual particle_type get_particle_type() const;

};

} // -- namespace particles
} // -- namespace sim
