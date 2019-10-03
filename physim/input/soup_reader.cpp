/*********************************************************************
 * Real-time physics simulation project
 * Copyright (C) 2018-2019 Lluís Alemany Puig
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * Contact: Lluís Alemany Puig (lluis.alemany.puig@gmail.com)
 * 
 ********************************************************************/

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
	(const std::string& dir, const std::string& fname, geometric::object *o)
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
