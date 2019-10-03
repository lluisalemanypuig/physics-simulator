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
#include <physim/particles/fluid_particle.hpp>
#include <physim/math/vec3.hpp>

namespace physim {
namespace fields {

/**
 * @brief Abstract class for force fields.
 *
 * Any force field is interpreted as a vector field:
 * given a position from the field's source there is
 * a force vector that affects a particle in that
 * position.
 *
 * The source is usually regarded as punctual, i.e.,
 * the origin of the field is a point. However, this
 * is considered as a type of field, and is implemented
 * in class @ref fields::punctual.
 */
class field {
	private:
	protected:
	public:
		/// Default constructor.
		field();
		/// Copy constructor.
		field(const field& f);
		/// Destructor.
		virtual ~field();

		// OTHERS

		/**
		 * @brief Compute the force vector acting on a particle.
		 *
		 * The contents of parameter @e F is undefined when calling
		 * this function, thus the contents should be overwritten.
		 * @param[in] p Particle where the force is applied to.
		 * @param[out] F The force from this field acting on the
		 * particle.
		 */
		virtual void compute_force(const particles::free_particle& p, math::vec3& F) = 0;
		/**
		 * @brief Compute the force vector acting on a particle.
		 *
		 * The contents of parameter @e F is undefined when calling
		 * this function, thus the contents should be overwritten.
		 * @param[in] p Particle where the force is applied to.
		 * @param[out] F The force from this field acting on the
		 * particle.
		 */
		virtual void compute_force(const particles::mesh_particle& p, math::vec3& F) = 0;
		/**
		 * @brief Compute the force vector acting on a particle.
		 *
		 * The contents of parameter @e F is undefined when calling
		 * this function, thus the contents should be overwritten.
		 * @param[in] p Particle where the force is applied to.
		 * @param[out] F The force from this field acting on the
		 * particle.
		 */
		virtual void compute_force(const particles::fluid_particle& p, math::vec3& F) = 0;
};

} // -- namespace fields
} // -- namespace physim
