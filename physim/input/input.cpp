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

#include <physim/input/input.hpp>

// C++ includes
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// physim includes
#include <physim/geometry/object.hpp>
#include <physim/input/ply_reader.hpp>
#include <physim/input/obj_reader.hpp>
#include <physim/input/soup_reader.hpp>
#include <physim/math/vec3.hpp>

namespace physim {
using namespace math;

namespace input {

	bool read_file
	(
		const std::string& directory,
		const std::string& filename,
		geometric::object *mesh
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
		if (extension == "soup") {
			// read .soup file
			return soup_read_file(directory, filename, mesh);
		}

		#if defined(DEBUG)
		cerr << "physim::input::read_file - Error:" << endl;
		cerr << "    Extension '" << extension << "' not recognised" << endl;
		#endif

		return false;
	}

	bool read_file(
		const std::string& dir,
		const std::string& fname,
		std::vector<math::vec2>& points
	)
	{
		string full_name = dir + "/" + fname;

		ifstream fin;
		fin.open(full_name.c_str());
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

		float x, y;
		while (fin >> x >> y) {
			vec2 v(x,y);
			points.push_back(v);
		}

		fin.close();
		return true;
	}

	bool read_file(
		const std::string& dir,
		const std::string& fname,
		std::vector<math::vec3>& points
	)
	{
		string full_name = dir + "/" + fname;

		ifstream fin;
		fin.open(full_name.c_str());
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

		float x, y, z;
		while (fin >> x >> y >> z) {
			vec3 v(x,y,z);
			points.push_back(v);
		}

		fin.close();
		return true;
	}

} // -- namespace io
} // -- namespace physim
