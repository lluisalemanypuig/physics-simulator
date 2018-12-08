#pragma once

// C++ includes
#include <string>

// physim includes
#include <physim/geometry/object.hpp>

namespace physim {
namespace input {

	bool read_file
	(
		const std::string& directory,
		const std::string& filename,
		geom::object *mesh
	);

} // -- namespace io
} // -- namespace physim
