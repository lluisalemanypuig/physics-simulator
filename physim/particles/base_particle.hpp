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
 * see namespace @ref physim::geometric) and maybe with other
 * particles, depending on the subclass.
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
		 * the different initialiser functions.
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
		 */
		virtual void init();

		/// Returns the type of this particle.
		virtual particle_type get_particle_type() const;

};

} // -- namespace particles
} // -- namespace sim
