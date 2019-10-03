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
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

namespace physim {
namespace input {

namespace input_private {

	/**
	 * @brief Retrieves the vertex coordinates and the faces
	 *
	 * Private function. Do not call directly.
	 * @param fin Input stream.
	 * @param[out] vertices Vertices of the object read from the file.
	 * @param[out] triangles Triangles of the object read from the file.
	 */
	void __obj_parse_file_lines
	(ifstream& fin, vector<math::vec3>& vertices, vector<size_t>& triangles)
	{
		string line;

		// face information
		size_t VN[4], NI[4], TC[4];
		// vertex information
		float x, y, z;

		size_t i = 0;
		while (getline(fin, line)) {
			if (line[0] == 'v') {
				// parse vertex information
				if (line[1] == ' ') {
					// vertex coordinate
					sscanf(line.c_str(), "v %f %f %f", &x, &y, &z);
					vertices.push_back(math::vec3(x, y, z));
				}
				else if (line[1] == 'n') {
					// vertex normal vector
					// -> ignore
				}
				else if (line[1] == 't') {
					// vertex texture coordinate
					// -> ignore
				}
			}
			else if (line[0] == 'u' and line[1] == 's' and line[2] == 'e') {
				// use material
				// -> ignore
			}
			else if (line[0] == 'f' and line[1] == ' ') {
				// parse face information
				long C = std::count(line.begin(), line.end(), ' ');
				if (C == 3) {
					// triangle
					if (line.find("//") != string::npos) {
						sscanf(
							line.c_str(),
							"f %ld//%ld %ld//%ld %ld//%ld",
							&VN[0],&NI[0], &VN[1],&NI[1], &VN[2],&NI[2]
						);
					}
					else if (line.find("/") != string::npos) {
						sscanf(
							line.c_str(), "f %ld/%ld/%ld %ld/%ld/%ld %ld/%ld/%ld",
							&VN[0],&TC[0],&NI[0], &VN[1],&TC[1],&NI[1],
							&VN[2],&TC[2],&NI[2]
						);
					}
					else {
						sscanf(line.c_str(),"f %ld %ld %ld",&VN[0],&VN[1],&VN[2]);
					}

					// make the face
					for (int i = 0; i < 3; ++i) {
						triangles.push_back(VN[i] - 1);
					}
				}
				else if (C == 4) {
					// quad
					if (line.find("//") != string::npos) {
						sscanf(
							line.c_str(),
							"f %ld//%ld %ld//%ld %ld//%ld %ld//%ld",
							&VN[0],&NI[0], &VN[1],&NI[1], &VN[2],&NI[2], &VN[3],&NI[3]
						);
					}
					else if (line.find("/") != string::npos) {
						sscanf(
							line.c_str(),
							"f %ld/%ld/%ld %ld/%ld/%ld %ld/%ld/%ld %ld/%ld/%ld",
							&VN[0],&TC[0],&NI[0], &VN[1],&TC[1],&NI[1],
							&VN[2],&TC[2],&NI[2], &VN[3],&TC[3],&NI[3]
						);
					}
					else {
						sscanf(
							line.c_str(),"f %ld %ld %ld %ld",
							&VN[0],&VN[1],&VN[2],&VN[3]
						);
					}

					// make two triangular faces
					for (int i = 0; i < 3; ++i) {
						triangles.push_back(VN[i] - 1);
					}

					// use the other vertices
					VN[1] = VN[2]; VN[2] = VN[3];
					// gather info
					for (int i = 0; i < 3; ++i) {
						triangles.push_back(VN[i] - 1);
					}
				}
				else {
					#if defined(DEBUG)
					cerr << "physim::input::input_private::__obj_parse_file_lines - Error" << endl;
					cerr << "    Line " << i << " does not contain a triangle or a quad" << endl;
					cerr << "    Found: " << line << endl;
					#endif
				}
			}
			else if (line[0] == 's' and line[1] == ' ') {
				// smooth information
				// -> ignore
			}
			++i;
		}
	}
} // -- namespace io_private

	bool obj_read_file
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

		// parse the vertices and faces information
		input_private::__obj_parse_file_lines(fin, vertices, triangles);
		fin.close();

		o->set_triangles(vertices, triangles);
		return true;
	}

} // -- namespace io
} // -- namespace physim
