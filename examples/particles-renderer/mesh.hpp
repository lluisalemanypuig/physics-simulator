#pragma once

// C++ includes
#include <iostream>
#include <vector>
using namespace std;

// glm includes
#include <glm/glm.hpp>
using namespace glm;

// Qt includes
#include <QOpenGLFunctions>

#if !defined (TEXTURE_START)
	#define TEXTURE_START 1
#else
	// display error message
#endif

/**
 * @brief The face struct.
 */
struct face {
	/// Identifier of the material used for this face.
	string M_ID;
	/// Does this face have 4 vertices?
	bool is_quad;
	/// Does this face has to be smoothed?
	bool smooth;
	/**
	 * @brief Indexes of the normals of all vertices.
	 *
	 * In case the face is a triangle the last
	 * index should be ignored. The @e i-th value
	 * of this array corresponds to the vertex of
	 * the @e i-th vertex of this face.
	 *
	 * A value equal to '-1' indicates a non-valid
	 * index.
	 */
	int normal_index[4];

	/// Indexes of the vertices of this face.
	int vertex_index[4];
	/**
	 * @brief Indexes of the texture coordinates.
	 *
	 * In case the face is a triangle the last
	 * index should be ignored. The @e i-th value
	 * of this array corresponds to the vertex of
	 * the @e i-th vertex of this face.
	 */
	int text_coord[4];

	/**
	 * @brief Constructor.
	 * @param nidxs Normal indexes. See @ref normal_index.
	 * @param verts Vertex indexes. See @ref vertex_index.
	 * @param texts Texture indexes. See @ref text_coord.
	 * @param mat_id Material id. See @ref M_ID.
	 * @param isquad Specify whether the face is a quad or not. See @ref is_quad.
	 * @param s This should be true if the face should be smoothed. See @ref smooth.
	 */
	face(int nidxs[4], int verts[4], int texts[4], const string& mat_id, bool isquad, bool s);

	/// Copy cosntructor.
	face(const face& f);
};

/**
 * @brief The material struct.
 */
struct material {
	/// Identifier of this material.
	string ID;
	/// Texture used on this material.
	int textureID;
	/// Illumination model.
	int illum;
	/// Specular exponent.
	float Ns;
	/// ¿?
	float Ni;
	/// Transparency.
	float d;
	/// Ambient colour, with transparency.
	vec4 Ka;
	/// Diffuse colour, with transparency.
	vec4 Kd;
	/// Specular colour, with transparency.
	vec4 Ks;

	/**
	 * @brief Constructor.
	 * @param amb Ambient colour, without transparency.
	 * @param dif Diffuse colour, without transparency.
	 * @param spec Specular colour, without transparency.
	 * @param ns Specular exponent.
	 * @param ni ¿?
	 * @param D Transparency
	 * @param ill Illumination model.
	 * @param textID Texture identifier.
	 * @param id Material's identifier.
	 */
	material
	(
		const vec3& amb,
		const vec3& dif,
		const vec3& spec,
		float ns, float ni,float D,
		int ill, int textID, const string& id
	);

	/// Copy constructor.
	material(const material& m);
};

/**
 * @brief Implements a mesh that need not be triangular.
 */
class mesh {
	private:
		/// Mesh name. Used for debugging purposes.
		string mesh_name;

		// mesh data
		/// Vertices of the mesh.
		vector<vec3> vertices;
		/// Normals of the mesh.
		vector<vec3> normals;
		/// Faces of the mesh. See @ref face.
		vector<face> faces;

		// materials and textures
		/// Materials used in the mesh. See @ref material.
		vector<material> materials;
		/// Texture coordinates.
		vector<vec2> textures_coords;
		/// OpenGL indexes of the textures.
		vector<unsigned int> textures_indexes;

	public:
		/// Default constructor.
		mesh();
		/// Destructor.
		~mesh();

		// SETTERS

		/// Sets the name of the mesh.
		void set_name(const string& name);
		/// Sets the vertices of the mesh.
		void set_vertices(const vector<vec3>& verts);
		/// Sets the faces of the mesh.
		void set_faces(const vector<face>& faces);
		/// Sets the normals of the mesh.
		void set_normals(const vector<vec3>& normals);
		/// Sets the materials used in this mesh.
		void set_materials(const vector<material>& mats);
		/// Sets the texture coordinates.
		void set_texture_coords(const vector<vec2>& texts);
		/// Sets the texture coordinates.
		void set_texture_indexes(const vector<unsigned int>& text_idxs);

		// GETTERS

		/**
		 * @brief Returns true if there are no errors in the
		 * mesh's structure.
		 *
		 * The mesh's structure is not valid in the following
		 * scenarios:
		 * - the mesh has no vertices.
		 * - the mesh has no faces.
		 * - any of the indexes in arrays @ref face::normal_index,
		 *   @ref face::vertex_index, @ref text_coord is out
		 *   of bounds.
		 */
		bool is_valid() const;

		// MODIFIERS

		/**
		 * @brief Computes smoothing normals.
		 *
		 * Assuming that the normals in @ref normals
		 * are face normals, compute a normal for each
		 * vertex that is equal to the average of the
		 * normals of the faces adjacent to that vertex.
		 *
		 * The normals computed are normalised.
		 */
		void smooth();
		/**
		 * @brief Scales this mesh so that it fits in a unit length box.
		 *
		 * The mesh need not be centered at the (0,0,0).
		 */
		void scale_to_unit();

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
		void render() const;
};

