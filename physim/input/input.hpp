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
#include <vector>
#include <string>

// physim includes
#include <physim/geometry/object.hpp>
#include <physim/math/vec2.hpp>
#include <physim/math/vec3.hpp>

namespace physim {
namespace input {

	/**
	 * @brief Reads a file describing a geometrical object.
	 *
	 * The object is typically described as a set of triangles,
	 * not necessarily connected (might be a triangle soup).
	 *
	 * Supported formats:
	 * - Wavefront. File extension: .obj.
	 * - Polygon File Format (a.k.a. Stanford Triangle Format).
	 *		File extension: .ply.
	 * - Triangle Soup Format. File extension: .soup. This file consists of
	 *		a list of triangles, each described with three vertices. A
	 *		vertex is described as usual with three coordinate values.
	 *
	 * @param directory Directory that contains the file to be read.
	 * @param filename The filename describing the object.
	 * @param[out] mesh Object constructed with the contents of the file.
	 * @return Returns true on success.
	 */
	bool read_file
	(
		const std::string& directory,
		const std::string& filename,
		geometric::object *mesh
	);

	/**
	 * @brief Reads a file describing a list of two-dimensional points.
	 *
	 * @param directory Directory that contains the file to be read.
	 * @param filename The filename describing the object.
	 * @param[out] points List of points stored in the file.
	 * @return Returns true on success.
	 */
	bool read_file
	(
		const std::string& directory,
		const std::string& filename,
		std::vector<math::vec2>& points
	);

	/**
	 * @brief Reads a file describing a list of three-dimensional points.
	 *
	 * @param directory Directory that contains the file to be read.
	 * @param filename The filename describing the object.
	 * @param[out] points List of points stored in the file.
	 * @return Returns true on success.
	 */
	bool read_file
	(
		const std::string& directory,
		const std::string& filename,
		std::vector<math::vec3>& points
	);

} // -- namespace io
} // -- namespace physim
