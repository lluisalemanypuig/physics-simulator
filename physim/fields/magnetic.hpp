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
#include <physim/fields/punctual.hpp>
#include <physim/math/vec3.hpp>

namespace physim {
namespace fields {

/**
 * @brief Punctual magnetic field.
 *
 * Described by a position @ref pos and a magentic
 * field vector @ref B.
 *
 * There are several types of magnetic fields.
 * As an example, see @ref magnetic_B.
 */
class magnetic : public punctual {
	protected:
		/// Magnetic field vector [T].
		math::vec3 B;

	public:
		/**
		 * @brief Default constructor.
		 *
		 * The magnetic field vector @ref B is initialised
		 * to (0,0,0).
		 */
		magnetic();
		/// Constructor with magnetic field vector.
		magnetic(const math::vec3& pos, const math::vec3& b);
		/// Copy constructor.
		magnetic(const magnetic& f);
		/// Destructor.
		virtual ~magnetic();

		// SETTERS

		/// Sets this magnetic field vector. See @ref B.
		void set_vector(const math::vec3& v);

		// GETTERS

		/// Returns this field's magnetic vector. See @ref B.
		const math::vec3& get_vector() const;

};

} // -- namespace fields
} // -- namespace physim
