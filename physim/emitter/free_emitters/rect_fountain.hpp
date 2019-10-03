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
 * @brief A fountain emitter.
 *
 * Provides a position initialiser function so that generated
 * particles behave like a fountain: their velocity is set to
 * positive values of the normal to the plane, and values of x
 * and z set as a function of their distance to the center plus
 * some random value.
 *
 * Need to set the source size through
 * @ref rect_source::set_rectangular_source method
 * so that the initialser's source is defined.
 */
class rect_fountain : public rect_source {
	protected:
		/**
		 * @brief Sets the velocity initialser.
		 *
		 * The velocity is set so that those points that are:
		 * - farther from the rectangle's center have a smaller-magnitude,
		 * "flatter" vector velocity, i.e., the angle between the
		 * vector and the plane is small.
		 * - closer to the center have larger-magnitude and
		 * "more vertical" vector velocity, i.e., the angle between
		 * the vector and the plane is large.
		 */
		void make_vel_init();

	public:
		/// Default constructor.
		rect_fountain();
		/**
		 * @brief Copy constructor.
		 *
		 * The function @ref base_emitter::pos is not copied.
		 * Instead, it is remade (function @ref make_vel_init is
		 * called again).
		 */
		rect_fountain(const rect_fountain& f);
		/// Destructor.
		~rect_fountain();

};

} // -- namespace free_emitters
} // -- namespace emitters
} // -- namespace physim
