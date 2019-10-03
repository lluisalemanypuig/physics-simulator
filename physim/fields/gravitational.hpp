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
#include <physim/fields/punctual.hpp>
#include <physim/math/vec3.hpp>

namespace physim {
namespace fields {

/**
 * @brief Gravitational field.
 *
 * This class implements the force that a gravitational field
 * applies on a particle of mass m.
 *
 * This gravitational field is caused by some body of mass
 * @ref M, at position @ref punctual::pos.
 *
 * Therefore the force is computed using
 * \f$-G \cdot M \cdot m \cdot \frac{p - O}{||p - O||}\f$
 * where
 * - \f$G\f$ is the gravitational constant.
 * - \f$M\f$ is the mass of the particle causing the field.
 * - \f$p\f$ is the particle's position.
 * - \f$O\f$ is the position of the particle causing the field.
 */
class gravitational : public punctual {
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
		/// Mass of object causing the gravitational field. [Kg]
		float M;

	public:
		/// Default constructor.
		gravitational();
		/// Constructor with position and mass.
		gravitational(const math::vec3& pos, float M);
		/// Copy constructor.
		gravitational(const gravitational& f);
		/// Destructor.
		virtual ~gravitational();

		// SETTERS

		/// Sets this gravitational field's mass. See @ref M.
		void set_mass(float M);

		// GETTERS

		/// Returns this field's mass. See @ref M.
		float get_mass() const;

		// OTHERS

		void compute_force(const particles::free_particle& p, math::vec3& F);
		void compute_force(const particles::mesh_particle& p, math::vec3& F);
		void compute_force(const particles::fluid_particle& p, math::vec3& F);
};

} // -- namespace fields
} // -- namespace physim
