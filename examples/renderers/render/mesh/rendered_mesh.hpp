#pragma once

// C++ includes
#include <vector>
#include <string>

// glm includes
#include <glm/vec2.hpp>

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
		std::vector<material> materials;

		/// Material id per face.
		std::vector<std::string> mat_ids;
		/// Texture coordinates.
		std::vector<glm::vec2> texture_coords;
		/// Texture indices per vertex.
		std::vector<int> texture_idxs;
		/// OpenGL indexes of the textures.
		std::vector<unsigned int> textures_indexes;

	public:
		/// Constructor.
		rendered_mesh();
		/// Destructor.
		~rendered_mesh();

		// SETTERS

		/// Sets the material ids of each face.
		void set_material_ids(const std::vector<std::string>& mat_ids);
		/// Sets the materials used in this mesh.
		void set_materials(const std::vector<material>& mats);
		/// Sets the texture coordinates.
		void set_texture_coords(const std::vector<glm::vec2>& texts);
		/// Sets the texture coordinates indices.
		void set_texture_idxs(const std::vector<int>& text_idxs);
		/// Sets the loaded texture indices.
		void set_textures_indices(const std::vector<unsigned int>& text_idxs);

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
