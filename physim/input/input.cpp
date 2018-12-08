#include <physim/input/input.hpp>

// C++ includes
#include <string>
using namespace std;

// physim includes
#include <physim/geometry/object.hpp>
#include <physim/input/ply_reader.hpp>
#include <physim/input/obj_reader.hpp>

namespace physim {
namespace input {

	bool read_file
	(
		const std::string& directory,
		const std::string& filename,
		geometry::object *mesh
	)
	{
		// retrieve extension
		size_t i = filename.length() - 1;
		while (i > 0 and filename[i] != '.') {
			--i;
		}

		#if defined(DEBUG)
		if (i == 0) {
			cerr << "physim::input::read_file - Error:" << endl;
			cerr << "    File '" << filename << "' has no extension." << endl;
			return false;
		}
		#endif

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

		#if defined(DEBUG)
		cerr << "physim::input::read_file - Error:" << endl;
		cerr << "    Extension '" << extension << "' not recognised" << endl;
		#endif

		return false;
	}

} // -- namespace io
} // -- namespace physim
