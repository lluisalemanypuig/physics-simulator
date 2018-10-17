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
 * @brief Types of states in which the mesh can be in.
 *
 * These are used to inform the user of the type of errors
 * that are in the structure of the mesh.
 */
enum class mesh_state : int8_t {
	/// No errors in the mesh.
	correct = 0,
	/// The mesh has no vertices.
	no_vertices,
	/// The mesh has no faces.
	no_faces,
	/**
	 * @brief There is a face that has a vertex index out of bounds.
	 *
	 * That index has a value greater or equal than the amount
	 * of vertices.
	 */
	vertex_idx_ob,
	/**
	 * @brief There is a face that has a normal index out of bounds.
	 *
	 * That index has a value greater or equal than the amount
	 * of normal vectors.
	 */
	normal_idx_ob,
	/**
	 * @brief There is a face that has a texture coordinate index out of bounds.
	 *
	 * That index has a value greater or equal than the amount
	 * of texture coordinates.
	 */
	texture_coord_idx_ob
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

	private:
		/// Computes the normal of the plane containing
		/// face @e f.
		vec3 face_normal(const face& F) const;

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
		 * @brief Returns the state of the mesh.
		 *
		 * See @ref mesh_state for details.
		 */
		mesh_state state() const;

		// MODIFIERS

		/**
		 * @brief Computes normals for the faces.
		 *
		 * Compute the normals of the faces. Replaces any
		 * existent normals in the mesh with the new ones.
		 *
		 * Since the faces' vertices are sorted in counterclockwise
		 * order, it is easy to compute normals using the
		 * cross product so that the normal points 'outside'
		 * the mesh.
		 *
		 * The normals are normalised.
		 */
		void make_normals_flat();
		/**
		 * @brief Computes normals for the vertices.
		 *
		 * Compute the normals of the vertices. Replaces any
		 * existent normals in the mesh with the new ones.
		 *
		 * The normal for a vertex is computed as the
		 * average of the normals of the neighbouring faces.
		 * A face's normal is the normal of the plane that
		 * contains a face.
		 *
		 * The normals are normalised.
		 */
		void make_normals_smooth();
		/**
		 * @brief Scales this mesh so that it fits in a unit length box.
		 *
		 * The mesh need not be centered at the (0,0,0).
		 */
		void scale_to_unit();

		/**
		 * @brief Clears the memory occupied by this mesh.
		 *
		 * Clears the contents of @ref vertices, @ref normals,
		 * @ref faces, @ref materials, @ref textures_coords,
		 * @ref textures_indexes.
		 *
		 * It also clears the memory occupied by the loaded
		 * textures, identified with the indexes in @ref texture_indexes.
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

