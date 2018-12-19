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
	 * @param directory Directory in the system.
	 * @param filename File with the mesh as a triangle soup
	 * (soup) format.
	 * @param[out] o Mesh loaded from file.
	 * @return Returns false on error.
	 */
	bool soup_read_file
	(const std::string& directory, const std::string& filename,
	 geometric::object *o);

} // -- namespace io
} // -- namespace physim
