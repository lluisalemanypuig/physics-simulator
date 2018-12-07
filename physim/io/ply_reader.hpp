#pragma once

// C++ includes
#include <string>
#include <vector>

// physim includes
#include <physim/geometry/object.hpp>
#include <physim/math/vec3.hpp>

namespace physim {
namespace io {

	namespace io_private {
		bool __ply_load_header
		(std::ifstream& fin, int& n_verts, int& n_faces, std::string& format);

		/* Loading the vertices must be done accordingly to the format. */

		// BINARY format
		//     little endian 1.0
		void __ply_load_vertices_binary_le_1_0
		(std::ifstream& fin, int n_verts, std::vector<math::vec3>& verts);
		void __ply_load_faces_binary_le_1_0
		(std::ifstream& fin, int n_faces, std::vector<size_t>& tris);

		// ASCII format
		//     ascii 1.0
		void __ply_load_vertices_ascii_1_0
		(std::ifstream& fin, int n_verts, std::vector<math::vec3>& verts);
		void __ply_load_faces_ascii_1_0
		(std::ifstream& fin, int n_faces, std::vector<size_t>& tris);

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
	bool ply_read_file
	(const std::string& directory, const std::string& filename,
	 geom::object *o);

} // -- namespace io
} // -- namespace physim
