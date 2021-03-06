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
#include <string>

// physim includes
#include <physim/geometry/object.hpp>
#include <physim/math/vec3.hpp>

namespace physim {
namespace input {

	/**
	 * @brief Loads a triangular mesh stored in @e filename.
	 *
	 * Anything that are not vertices or vertices indices are ignored.
	 * @param directory Directory in the system.
	 * @param filename File with the mesh in wavefront (obj) format.
	 * @param[out] o Mesh loaded from file.
	 * @return Returns false on error.
	 */
	bool obj_read_file
	(const std::string& directory, const std::string& filename,
	 geometric::object *o);

} // -- namespace io
} // -- namespace physim
