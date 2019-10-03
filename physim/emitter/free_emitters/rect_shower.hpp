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

// C++ includes
#include <random>

// physim includes
#include <physim/emitter/free_emitters/rect_source.hpp>

namespace physim {
namespace emitters {
namespace free_emitters {

/**
 * @brief A shower emitter.
 *
 * Provides a position initialiser function so that generated
 * particles behave like a shower: their velocity is set to
 * negative values of y, and null values of x and z.
 *
 * Need to set the source size through
 * @ref rect_source::set_rectangular_source method
 * so that the initialser's source is defined.
 */
class rect_shower : public rect_source {
	protected:
		/**
		 * @brief Sets the velocity initialser.
		 *
		 * The velocity is set to negative values of y, and
		 * null values of x and z.
		 */
		void make_vel_init();

	public:
		/// Default constructor.
		rect_shower();
		/// Copy constructor.
		rect_shower(const rect_shower& w);
		/// Destructor.
		~rect_shower();

		// GETTERS

		free_emitter *clone() const;

};

} // -- namespace free_emitters
} // -- namespace emitters
} // -- namespace physim
