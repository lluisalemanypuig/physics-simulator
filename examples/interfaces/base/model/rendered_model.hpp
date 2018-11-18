#pragma once

// C++ includes
#include <vector>
#include <string>

// physim includes
#include <physim/math/vec2.hpp>
typedef physim::math::vec2 vec2;

// Custom includes
#include <base/model/model_utils.hpp>
#include <base/model/model.hpp>

/**
 * @brief Rendered model class.
 *
 * This is an abstraction of a model that has to be rendered somehow.
 *
 * This means having to draw its geometry with flat colours, lighting,
 * textures, ...
 *
 * Rendering this model requires calling the appropriate methods
 * within a valid OpenGL context. For example, if one wants to render
 * this model with textures method @ref load_textures must be loaded
 * within an OpenGL context. Same for faster rendering using OpenGL lists
 * (see @ref copmile).
 */
class rendered_model : public model {
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
		std::vector<vec2> texture_coords;
		/// Texture indices per vertex.
		std::vector<int> texture_idxs;
		/// OpenGL indexes of the textures.
		std::vector<unsigned int> textures_indexes;

		/// Index of the compiled list.
		uint list_index;

	public:
		/// Constructor.
		rendered_model();
		/**
		 * @brief Copy constructor
		 *
		 * The index for the glList is not copied. Therefore, it
		 * should be compiled again.
		 */
		rendered_model(const rendered_model& m);
		/// Destructor.
		~rendered_model();

		// SETTERS

		/// Sets the material ids of each face.
		void set_material_ids(const std::vector<std::string>& mat_ids);
		/// Sets the materials used in this mesh.
		void set_materials(const std::vector<material>& mats);
		/// Sets the texture coordinates.
		void set_texture_coords(const std::vector<vec2>& texts);
		/// Sets the texture coordinates indices.
		void set_texture_idxs(const std::vector<int>& text_idxs);

		// GETTERS

		mesh_state state(const mesh_state& ignore = mesh_state::correct) const;

		// MODIFIERS

		/**
		 * @brief Loads the textures for the model.
		 *
		 * In case this model has textures, they should be loaded here.
		 */
		void load_textures();

		/**
		 * @brief Clears the memory occupied by the model.
		 *
		 * This means clearing not only the memory for the vertices,
		 * normals, ..., but also the compiled models.
		 */
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
		void display_mesh_info() const;

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

		/**
		 * @brief Compiles this object into a list.
		 *
		 * Uses glNewList, glEndList to compile the object.
		 * Between these two calls, the function @ref slow_render()
		 * is called.
		 * @return Returns the list index.
		 */
		uint compile();

		/**
		 * @brief Chooses the best way of rendering this model.
		 *
		 * If no list is available (@ref list_index is -1) then
		 * @ref slow_render() is used.
		 */
		void render() const;
};
