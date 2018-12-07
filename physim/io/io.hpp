#pragma once

// C++ includes
#include <string>

// physim includes
#include <physim/geometry/object.hpp>

namespace physim {
namespace io {

	bool read_mesh
	(
		const std::string& directory,
		const std::string& filename,
		geom::object *mesh
	);

} // -- namespace io
} // -- namespace physim
