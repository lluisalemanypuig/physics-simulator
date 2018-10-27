#pragma once

#include <string>
using namespace std;

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
using namespace glm;

#if !defined (TEXTURE_START)
	#define TEXTURE_START 1
#else
	// display error message
#endif

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
	no_triangles,
	/**
	 * @brief There is a triangle that has a vertex
	 * index out of bounds.
	 *
	 * That index has a value greater or equal than
	 * the amount of vertices.
	 */
	vertex_idx_ob,
	/**
	 * @brief There is a triangle that has a normal
	 * index out of bounds.
	 *
	 * That index has a value greater or equal than the
	 * amount of normal vectors.
	 */
	normal_idx_ob,
	/**
	 * @brief There is a triangle that has a texture coordinate
	 * index out of bounds.
	 *
	 * That index has a value greater or equal than the amount
	 * of texture coordinates.
	 */
	texture_coord_idx_ob,
	/**
	 * @brief There is a triangle whose material was not
	 * found within the collection of loaded materials.
	 */
	material_not_found
};

