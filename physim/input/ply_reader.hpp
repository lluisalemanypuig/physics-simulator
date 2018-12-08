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
	bool ply_read_file
	(const std::string& directory, const std::string& filename,
	 geom::object *o);

} // -- namespace io
} // -- namespace physim
