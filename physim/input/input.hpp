#pragma once

// C++ includes
#include <string>

// physim includes
#include <physim/geometry/object.hpp>

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
		geometry::object *mesh
	);

} // -- namespace io
} // -- namespace physim
