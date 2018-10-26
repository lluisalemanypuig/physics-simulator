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

// Qt includes
#include <QOpenGLFunctions>

// Custom includes
#include "mesh.hpp"

/**
 * @brief Wavefront format file reader class.
 *
 * This class is used to parse files describing meshes
 * in wavefront format. This is done via the
 * @ref load_object method.
 *
 * This reader does not modify the data read,
 * and does not store the mesh. Instead, the data
 * is dumped into a data structure.
 *
 * Finally, note that this reader is rather simplified.
 * It is not guaranteed that all .obj can be correctly
 * read by this class.
 */
class OBJ_reader {
	private:
		/**
		 * @brief Name of the object begin parsed.
		 *
		 * Only used for debugging purposes.
		 */
		string object_name;
		/**
		 * @brief Directory of the .obj file.
		 *
		 * This directory should also contain the
		 * corresponding .mtl and texture files.
		 */
		string directory;
		/// Name of the .obj file to read.
		string filename;
		/// Contents of the file, line by line.
		vector<string> file_lines;

		// mesh data
		/// Vertices of the mesh.
		vector<glm::vec3> vertices;
		/// Normals per vertex.
		vector<glm::vec3> normals;
		/**
		 * @brief Faces of the mesh.
		 *
		 * Following the wavefront format, they may have three
		 * or four vertices, and can contain material and
		 * texture information.
		 */
		vector<face> faces;

		// materials and textures
		/// Set of materials found in the file.
		vector<material> materials;
		/// Texture coordinates.
		vector<glm::vec2> textures_coords;
		/**
		 * @brief Indexes of textures.
		 *
		 * These indices point to the txextures loaded.
		 */
		vector<unsigned int> textures_indexes;

		/// Frees the memory occupied by the loader.
		void clean();

		#if defined (DEBUG)
		void display_object_info();
		#endif

		/// Reads every line of the file and dumps them into @ref file_lines.
		bool load_file();
		/// Loads the material @e mtl_file in @ref directory.
		bool load_material(const char *mtl_file);

		/**
		 * @brief Retrieves the vertex coordinates, the faces and
		 * their normals from the file.
		 *
		 * The file's lines are stored in @ref file_lines.
		 *
		 * The data parsed goes from line @e A to line @e B.
		 * @param A Start at line @e A.
		 * @param B Start at line @e B.
		 * @pre 0 <= @e A < @e B < @ref file_lines.size()
		 */
		void get_vertices_faces_normals(size_t A, size_t B);

	public:
		/// Default constructor.
		OBJ_reader();
		/// Destructor.
		~OBJ_reader();

		/**
		 * @brief Loads one object stored in a .obj file.
		 * @param dir The directory where the file is.
		 * @param name The name of the .obj file.
		 * @param[out] M The mesh constructed using the data in the file.
		 * @return Returns true on success.
		 */
		bool load_object(const string& dir, const string& name, mesh& M);

};

