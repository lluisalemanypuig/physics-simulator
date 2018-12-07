#pragma once

// C++ includes
#include <fstream>
#include <vector>
#include <string>

// physim includes
#include <physim/geometry/object.hpp>
#include <physim/math/vec3.hpp>

namespace physim {
namespace io {

	namespace io_private {
		/**
		 * @brief Retrieves the vertex coordinates and the faces
		 *
		 * Private function. Do not call directly.
		 * @param fin Input stream.
		 * @param[out] vertices Vertices of the object read from the file.
		 * @param[out] triangles Triangles of the object read from the file.
		 */
		void __obj_parse_file_lines(
			std::ifstream& fin,
			std::vector<math::vec3>& vertices,
			std::vector<size_t>& triangles
		);

	} // -- namespace io_private

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
	 geom::object *o);

} // -- namespace io
} // -- namespace physim
