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

// physim includes
#include <physim/particles/particle_types.hpp>
#include <physim/particles/free_particle.hpp>
#include <physim/math/vec3.hpp>

namespace physim {
namespace particles {

/**
 * @brief Class implementing a sized particle.
 *
 * A free particle is a 0-dimensional object, subject
 * to several forces but not to any other particle's
 * direct interaction through them. It can collide with
 * other objects in the scene (geometrical objects, see
 * namespace @ref physim::geometric) and also with other
 * paricles.
 *
 * A sized particle is, then, a particle with some size
 * (the radius, see @ref R) along the three axis. The loci
 * of point defined by the sphere centered at the current
 * position of the particle (see @ref base_particle::cur_pos)
 * and radius @e R are part of the particle. Notice,
 * however, that this is not a sphere.
 *
 * All this class' attributes are public.
 */
class sized_particle : public free_particle {
	private:
		/**
		 * @brief Initialises this class's attributes.
		 *
		 * The attributes of the class take the following values:
		 * - @ref R : 1.0
		 */
		void partial_init();

	public:
		/// Radius of the particle [m].
		float R;

	public:
		/// Default constructor.
		sized_particle();
		/// Copy constructor.
		sized_particle(const sized_particle& p);
		/// Destructor.
		virtual ~sized_particle();

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
		 * See @ref partial_init() to see how the attributes of this class
		 * are intialised.
		 */
		void init();

		// GETTERS

		virtual particle_type get_particle_type() const;
};

} // -- namespace particles
} // -- namespace physim
