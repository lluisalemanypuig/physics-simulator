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
#include <physim/fields/field.hpp>
#include <physim/math/vec3.hpp>

namespace physim {
namespace fields {

/**
 * @brief Force field caused by a particle.
 */
class punctual : public field {
	protected:
		/// Center of the field vector. [m]
		math::vec3 pos;
	public:
		/**
		 * @brief Default constructor.
		 *
		 * Position @ref pos is initialised at (0,0,0).
		 */
		punctual();
		/// Constructor with position.
		punctual(const math::vec3& p);
		/// Copy constructor.
		punctual(const punctual& f);
		/// Destructor
		virtual ~punctual();

		// SETTERS

		/// Sets the position of the field. See @ref pos.
		void set_position(const math::vec3& p);

		// GETTERS

		/// Returns the position of the field. See @ref pos.
		const math::vec3& get_position() const;
};

} // -- namespace fields
} // -- namespace physim
