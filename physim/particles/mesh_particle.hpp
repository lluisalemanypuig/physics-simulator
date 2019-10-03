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
 * @brief Class implementing a mesh particle.
 *
 * A particle is a 0-dimensional object, subject to
 * several forces. It can also collide with other
 * objects in the scene (geometrical objects, see
 * namespace @ref physim::geometric) but not with other
 * particles.
 */
class mesh_particle : public base_particle {
	private:
		/**
		 * @brief Initialises this class's attributes.
		 *
		 * The attributes of the class take the following values:
		 * - @ref charge : 0
		 * - @ref fixed : false
		 */
		void partial_init();

	public:
		/// Electrical charge of the particle [C].
		float charge;

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
		mesh_particle();
		/// Copy constructor.
		mesh_particle(const mesh_particle& p);
		/// Destructor.
		virtual ~mesh_particle();

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
