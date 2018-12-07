#include <physim/io/obj_reader.hpp>

// C includes
#include <assert.h>

// C++ includes
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
using namespace std;

namespace physim {
namespace io {

namespace io_private {
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
						triangles.push_back(VN[i]);
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
					// use the first 3 vertices: 0,1,2
					// gather info
					for (int i = 0; i < 3; ++i) {
						triangles.push_back(VN[i]);
					}

					// use the other vertices
					VN[1] = VN[2]; VN[2] = VN[3];
					// gather info
					for (int i = 0; i < 3; ++i) {
						triangles.push_back(VN[i]);
					}
				}
				else {
					cerr << "io::parse_file_lines - Error" << endl;
					cerr << "    Line " << i << " does not contain a triangle or a quad" << endl;
					cerr << "    Found: " << line << endl;
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

	bool load_object(const string& dir, const string& fname, geom::object *o)
	{
		// the '/' should be a '\' in windows...
		string full_path = dir + "/" + fname;
		ifstream fin;
		fin.open(full_path.c_str());
		if (not fin.is_open()) {
			cerr << "OBJ_reader::load_object - Error:" << endl;
			cerr << "    Could not open file " << endl;
			cerr << "        " << fname << endl;
			cerr << "    in directory" << endl;
			cerr << "        " << dir << endl;
			return false;
		}

		vector<math::vec3> vertices;
		vector<size_t> triangles;

		// parse the vertices and faces information
		io_private::__obj_parse_file_lines(fin, vertices, triangles);
		fin.close();

		o->set_vertices(vertices);
		o->set_triangles(triangles);
		return true;
	}

} // -- namespace io
} // -- namespace physim
