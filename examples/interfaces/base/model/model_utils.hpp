#pragma once

// C++ includes
#include <string>

// physim includes
#include <physim/math/vec3.hpp>
#include <physim/math/vec4.hpp>
typedef physim::math::vec3 vec3;
typedef physim::math::vec4 vec4;

#define NULL_TEXTURE_NAME "material-without-texture"

/**
 * @brief The material struct.
 */
struct material {
	/// Identifier of this material.
	std::string id;
	/// Filename of the texture material.
	std::string txt_name;
	/// Texture id used on this material.
	unsigned int txt_id;
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
		const std::string& _id,
		const std::string& _txt_name,
		unsigned int _txt_id,
		const vec3& amb,
		const vec3& dif,
		const vec3& spec,
		float ns, float ni,float D,
		int ill
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
enum mesh_state {
	/// No errors in the mesh.
	correct					= 0x000,
	/// The mesh has no vertices.
	no_vertices				= 0x001,
	/// The mesh has no faces.
	no_triangles			= 0x002,
	/// The mesh has no normals.
	no_normals				= 0x004,
	/// The mesh has no normal indexes.
	no_normal_idxs			= 0x008,
	/**
	 * @brief There is a triangle that has a vertex
	 * index out of bounds.
	 *
	 * That index has a value greater or equal than
	 * the amount of vertices.
	 */
	vertex_idx_ob			= 0x010,
	/**
	 * @brief There is a triangle that has a normal
	 * index out of bounds.
	 *
	 * That index has a value greater or equal than the
	 * amount of normal vectors.
	 */
	normal_idx_ob			= 0x020,
	/**
	 * @brief There is a triangle that has a texture coordinate
	 * index out of bounds.
	 *
	 * That index has a value greater or equal than the amount
	 * of texture coordinates.
	 */
	texture_coord_idx_ob	= 0x040,
	/**
	 * @brief There is a triangle whose material was not
	 * found within the collection of loaded materials.
	 */
	material_not_found		= 0x080
};

