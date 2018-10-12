#pragma once

// C includes
#include <assert.h>

// C++ includes
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <string>
#include <cmath>
#include <list>
using namespace std;

// glm includes
#include <glm/glm.hpp>
using namespace glm;

// Qt includes
#include <QOpenGLFunctions>

// Custom includes
#include "mesh.hpp"

class OBJ_reader {
	private:
		string object_name;
		string directory, filename;
		vector<string> file_lines;

		// mesh data
		vector<vec3> vertices;
		vector<vec3> normals;
		vector<face> faces;

		// materials and textures
		vector<material> materials;
		vector<vec2> textures_coords;
		vector<unsigned int> textures_indexes;

		void clean();

		#if defined (DEBUG)
		void display_object_info();
		#endif

		bool load_file();
		bool load_material(const char *mtl_file);

		/**
		 * @brief Retrieves the vertex coordinates, the faces and
		 * their normals from the file.
		 *
		 * The file lines are stored in @ref file_lines.
		 * @param A Start at line @e A.
		 * @param B Start at line @e B.
		 * @pre 0 <= @e A < @e B < @ref file_lines.size()
		 */
		void get_vertices_faces_normals(size_t A, size_t B);

	public:
		OBJ_reader();
		~OBJ_reader();

		// Pre: filename és el path del fitxer *.obj que conté UN ÚNIC OBJECTE
		// Post: retorna un enter més gran o igual a 1 que representa l'objecte
		//		retorna -1 en cas que no es trobessin els fitxers *.obj, *.mtl o les textures.
		bool load_object(const string& dir, const string& name, mesh& M);

};

