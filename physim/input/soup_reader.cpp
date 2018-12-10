#include <physim/input/obj_reader.hpp>

// C includes
#include <assert.h>

// C++ includes
#include <numeric>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

namespace physim {
namespace input {

	bool soup_read_file
	(const std::string& dir, const std::string& fname, geometry::object *o)
	{
		assert(o != nullptr);

		// should the '/' be a '\' in windows?
		string full_path = dir + "/" + fname;
		ifstream fin;
		fin.open(full_path.c_str());

		if (not fin.is_open()) {
			#if defined(DEBUG)
			cerr << "physim::input::input_private::obj_read_file - Error:" << endl;
			cerr << "    Could not open file " << endl;
			cerr << "        " << fname << endl;
			cerr << "    in directory" << endl;
			cerr << "        " << dir << endl;
			#endif
			return false;
		}

		vector<math::vec3> vertices;
		vector<size_t> triangles;

		math::vec3 v;
		while (fin >> v.x >> v.y >> v.z) {
			vertices.push_back(v);
		}
		triangles.resize(vertices.size());
		std::iota(triangles.begin(), triangles.end(), 0);

		fin.close();

		#if defined(DEBUG)
		if (vertices.size()%3 != 0) {
			cerr << "physim::input::input_private::obj_read_file - Error:" << endl;
			cerr << "    Number of vertices is not a multiple of 3" << endl;
			cerr << "    so one triangle is incomplete." << endl;
		}
		assert(vertices.size()%3 == 0);
		#endif

		o->set_triangles(vertices, triangles);
		return true;
	}

} // -- namespace io
} // -- namespace physim
