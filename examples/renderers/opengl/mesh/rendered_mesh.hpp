#pragma once

// C++ includes
#include <vector>
#include <string>
using namespace std;

// glm includes

// Custom includes
#include "mesh_utils.hpp"
#include "mesh.hpp"

class rendered_mesh : public mesh {
	private:
		/**
		 * @brief Materials used in the mesh.
		 *
		 * Material per triangle, not per vertex.
		 */
		vector<material> materials;

		/// Material id per face.
		vector<string> mat_ids;
		/// Texture coordinates.
		vector<vec2> texture_coords;
		/// Texture indices per vertex.
		vector<int> texture_idxs;
		/// OpenGL indexes of the textures.
		vector<unsigned int> textures_indexes;

	public:
		/// Constructor.
		rendered_mesh();
		/// Destructor.
		~rendered_mesh();

		// SETTERS

		/// Sets the material ids of each face.
		void set_material_ids(const vector<string>& mat_ids);
		/// Sets the materials used in this mesh.
		void set_materials(const vector<material>& mats);
		/// Sets the texture coordinates.
		void set_texture_coords(const vector<vec2>& texts);
		/// Sets the texture coordinates indices.
		void set_texture_idxs(const vector<int>& text_idxs);
		/// Sets the loaded texture indices.
		void set_textures_indices(const vector<unsigned int>& text_idxs);

		// GETTERS

		mesh_state state() const;

		// MODIFIERS

		void clear();

		// OTHERS

		/**
		 * @brief Outputs information about the mesh.
		 *
		 * Outputs the following information into standard
		 * output:
		 * - Number of vertices.
		 * - Number of faces.
		 * - Number of normals.
		 * - Number of materials.
		 * - Number of texture coordinates.
		 * - Number of textures used to texturise this mesh.
		 * - The definition of the materials used.
		 */
		void display_mesh_info();

		/**
		 * @brief Renders the mesh.
		 *
		 * Renders the faces one by one calling
		 \verbatim
		 glBegin();
		 ...
		 glEnd();
		 \endverbatim
		 * This method assumes that the result
		 * of method @ref is_valid is 'true'.
		 */
		void slow_render() const;
};
