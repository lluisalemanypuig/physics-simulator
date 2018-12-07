#include <physim/io/io.hpp>

// C++ includes
#include <string>

// physim includes
#include <physim/geometry/object.hpp>
#include <physim/io/ply_reader.hpp>
#include <physim/io/obj_reader.hpp>

namespace physim {
namespace io {

	bool read_mesh
	(
		const std::string& directory,
		const std::string& filename,
		geom::object *mesh
	)
	{
		// retrieve extension
		size_t i = filename.length() - 1;
		while (i > 0 and filename[i] != '.') {
			--i;
		}
		++i;
		std::string extension = filename.substr(i, filename.length() - i);

		if (extension == "obj") {
			// read .obj file
			return obj_read_file(directory, filename, mesh);
		}
		if (extension == "ply") {
			// read .obj file
			return ply_read_file(directory, filename, mesh);
		}

		return false;
	}

} // -- namespace io
} // -- namespace physim
