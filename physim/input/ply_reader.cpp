#include <physim/input/ply_reader.hpp>

// C includes
#include <string.h>

// C++ includes
#include <iostream>
#include <fstream>
using namespace std;

namespace physim {
namespace input {

namespace input_private {
	/**
	 * @brief Loads the header of the ply file
	 *
	 * Extracts number of vertices, faces, and whether it is binary
	 * or ASCII format.
	 * @param fin File stream.
	 * @param[out] n_verts Number of vertices.
	 * @param[out] n_faces Number of faces.
	 * @param[out] format Format of the file.
	 * @return Returns true on success.
	 */
	bool __ply_load_header
	(ifstream& fin, int& n_verts, int& n_faces, string& format)
	{
		char line[100];

		fin.getline(line, 100);
		if (strncmp(line, "ply", 3) != 0) {
			#if defined(DEBUG)
			cerr << "physim::input::input_private::__ply_load_header - Error:" << endl;
			cerr << "    Wrong format of file: first line does not contain 'ply'."
				 << endl;
			#endif
			return false;
		}
		n_verts = 0;
		fin.getline(line, 100);
		while (strncmp(line, "end_header", 10) != 0) {

			if (strncmp(line, "element face", 12) == 0) {
				n_faces = atoi(&line[13]);
			}
			else if (strncmp(line, "element vertex", 14) == 0) {
				n_verts = atoi(&line[15]);
			}
			else if (strncmp(line, "format", 6) == 0) {
				format = string(&line[7]);
			}
			else if (strncmp(line, "property float nx", 17) == 0) {
				#if defined(DEBUG)
				cerr << "physim::input::input_private::__ply_load_header - Error:" << endl;
				cerr << "    This model has normals: more vertices than necessary"
					 << endl;
				cerr << "    are in the file and this makes it too difficult to"
					 << endl;
				cerr << "    create the mesh." << endl;
				#endif
				return false;
			}
			fin.getline(line, 100);
		}
		if (n_verts <= 0) {
			#if defined(DEBUG)
			cerr << "physim::input::input_private::__ply_load_header - Error:" << endl;
			cerr << "    Number of vertices read is negative." << endl;
			#endif
			return false;
		}
		return true;
	}

	/* LOADING VERTICES AND MESHES */

	// ---------------- BINARY ----------------
	// ---- LITTLE ENDIAN 1.0
	/// Load vertices in binary, little endian format
	void __ply_load_vertices_binary_le_1_0
	(ifstream& fin, int n_verts, vector<math::vec3>& verts)
	{
		float v1, v2, v3;

		for (int i = 0; i < n_verts; ++i) {
			fin.read((char *)&v1, sizeof(float));
			fin.read((char *)&v2, sizeof(float));
			fin.read((char *)&v3, sizeof(float));

			verts.push_back(v1);
			verts.push_back(v2);
			verts.push_back(v3);
		}
	}

	/// Load faces in binary, little endian format
	void __ply_load_faces_binary_le_1_0
	(ifstream& fin, int n_faces, vector<size_t>& tris)
	{
		// indices of vertices per face
		unsigned int tri[3];
		// number of vertices per face. I hope this number
		// is not larger than 128...
		char n_vertex_face;

		for (int i = 0; i < n_faces; ++i) {
			fin.read(static_cast<char *>(&n_vertex_face), sizeof(char));

			// read vertices indexes
			fin.read((char *)&tri[0], sizeof(int));
			fin.read((char *)&tri[1], sizeof(int));
			fin.read((char *)&tri[2], sizeof(int));
			// put the indices in the vector
			tris.push_back(tri[0]);
			tris.push_back(tri[1]);
			tris.push_back(tri[2]);

			// in case the face has 4 or more vertices...
			for(; n_vertex_face > 3; --n_vertex_face) {
				tri[1] = tri[2];
				fin.read((char *)&tri[2], sizeof(int));
				tris.push_back(tri[0]);
				tris.push_back(tri[1]);
				tris.push_back(tri[2]);
			}
		}
	}

	// ---------------- ASCII ----------------
	// ---- ASCII 1.0

	/// Load vertices in ASCII
	void __ply_load_vertices_ascii_1_0
	(ifstream& fin, int n_verts, vector<math::vec3>& verts)
	{
		float v1,v2,v3;

		for (int i = 0; i < n_verts; ++i) {
			fin >> v1 >> v2 >> v3;

			verts.push_back(v1);
			verts.push_back(v2);
			verts.push_back(v3);
		}
	}

	/// Load faces in ASCII
	void __ply_load_faces_ascii_1_0
	(ifstream& fin, int n_faces, vector<size_t>& tris)
	{
		// indices of vertices per face
		unsigned int tri[3];
		// number of vertices per face
		int n_vertex_face;

		for (int i = 0; i < n_faces; ++i) {
			fin >> n_vertex_face;

			// read vertices indexes
			fin >> tri[0] >> tri[1] >> tri[2];

			// put the indices in the vector
			tris.push_back(tri[0]);
			tris.push_back(tri[1]);
			tris.push_back(tri[2]);

			// in case the face has 4 or more vertices...
			for(; n_vertex_face > 3; --n_vertex_face) {
				tri[1] = tri[2];
				fin >> tri[2];

				tris.push_back(tri[0]);
				tris.push_back(tri[1]);
				tris.push_back(tri[2]);
			}
		}
	}

} // -- namespace io_private

	bool ply_read_file(const std::string& dir, const std::string& fname, geom::object *o)
	{
		// the '/' should be a '\' in windows...
		string filename = dir + "/" + fname;

		ifstream fin;
		int n_verts, n_faces;

		fin.open(filename.c_str(), ios_base::in | ios_base::binary);
		if (not fin.is_open()) {
			#if defined(DEBUG)
			cerr << "physim::input::ply_read_file - Error:" << endl;
			cerr << "    Could not open file '" << filename << "'." << endl;
			#endif
			return false;
		}

		string format;
		bool header_read =
			input_private::__ply_load_header(fin, n_verts, n_faces, format);
		if (not header_read) {
			fin.close();
			#if defined(DEBUG)
			cerr << "physim::input::ply_read_file - Error:" << endl;
			cerr << "    Bad input file format." << endl;
			#endif
			return false;
		}

		vector<math::vec3> vertices;
		vector<size_t> triangles;

		// Load the vertices and the faces from the ply file.
		// Call the appropriate functions depending on the format.
		if (format == "binary_little_endian 1.0") {
			input_private::__ply_load_vertices_binary_le_1_0(fin, n_verts, vertices);
			input_private::__ply_load_faces_binary_le_1_0(fin, n_faces, triangles);
		}
		else if (format == "ascii 1.0") {
			input_private::__ply_load_vertices_ascii_1_0(fin, n_verts, vertices);
			input_private::__ply_load_faces_ascii_1_0(fin, n_faces, triangles);
		}
		else {
			#if defined(DEBUG)
			cerr << "physim::input::ply_read_file - Error:" << endl;
			cerr << "    Unknown file format '" << format << "'." << endl;
			#endif
			return false;
		}

		fin.close();

		o->set_vertices(vertices);
		o->set_triangles(triangles);
		return true;
	}

} // -- namespace PLY_reader
} // -- namespace geoproc
