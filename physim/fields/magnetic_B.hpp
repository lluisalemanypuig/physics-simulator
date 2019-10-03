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
#include <physim/particles/free_particle.hpp>
#include <physim/particles/mesh_particle.hpp>
#include <physim/fields/magnetic.hpp>

namespace physim {
namespace fields {

/**
 * @brief Implementation of a magnetic B-field.
 *
 * The force that acts on a particle is calculated
 * as:
 *
 * \f$ \vec{F} = (q \cdot v_p) \times \vec{B}\f$,
 * where \f$q\f$ is the charge of the particle
 * (see @ref free_particle::charge and
 * @ref mesh_particle::charge), \f$B\f$ is the
 * magnetic field vector (see @ref magnetic::B) and
 * \f$v_p\f$ is the particle's punctual velocity
 * (see @ref free_particle::cur_vel and
 * see @ref mesh_particle::cur_vel).
 */
class magnetic_B : public magnetic {
	private:
		/**
		 * @brief Function that actuall computes the force of this field.
		 *
		 * Works for @ref particles::free_particle and
		 * @ref particles::mesh_particle.
		 */
		template<class P>
		void __compute_force(const P& p, math::vec3& F);

	protected:

	public:
		/// Default constructor
		magnetic_B();
		/// Constructor with position and magnetic field vector.
		magnetic_B(const math::vec3& pos, const math::vec3& b);
		/// Copy constructor.
		magnetic_B(const magnetic_B& f);
		/// Destructor.
		virtual ~magnetic_B();

		// OTHERS

		void compute_force(const particles::free_particle& p, math::vec3& F);
		void compute_force(const particles::mesh_particle& p, math::vec3& F);
		void compute_force(const particles::fluid_particle& p, math::vec3& F);
};

} // -- namespace fields
} // -- namespace physim
