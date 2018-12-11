#pragma once

// C++ includes
#include <vector>

// physim includes
#include <physim/geometry/geometry.hpp>
#include <physim/geometry/triangle.hpp>
#include <physim/particles/free_particle.hpp>
#include <physim/math/vec3.hpp>
#include <physim/structures/object_partition.hpp>

namespace physim {
namespace geometry {

/**
 * @brief Class that implements a triangular object.
 *
 * A triangular object is a collection of triangles which may be a triangle
 * soup or a triangular mesh.
 */
class object : public geometry {
	private:
		/// The triangles of this object.
		std::vector<triangle> tris;

		/// Partition of the object for faster intersection tests.
		structures::object_partition octree;

	public:
		/// Default constructor.
		object();
		/// Copy constructor.
		object(const object& p);
		/// Destructor.
		~object();

		// OPERATORS

		// SETTERS

		/**
		 * @brief Sets the position of this object.
		 *
		 * Translates all triangles in @ref tris in the direction of
		 * vector @e v.
		 * @param v Vector.
		 */
		void set_position(const math::vec3& v);

		/// Sets the vertices of this object. See @ref verts.
		void set_triangles
		(const std::vector<math::vec3>& vs, const std::vector<size_t>& trs);

		// GETTERS

		/**
		 * @brief Returns the underlying structure that partitions this object.
		 * @return Returns a constant reference to @ref octree.
		 */
		const structures::object_partition& get_partition() const;

		geometry_type get_geom_type() const;

		/**
		 * @brief Returns the triangles of this object.
		 * @return Returns a constant reference to @ref tris.
		 */
		const std::vector<triangle>& get_triangles() const;

		/**
		 * @brief Returns true if @e p is inside the object.
		 *
		 * Returns true iff @e p is inside any of triangles. See
		 * @ref triangle::is_inside.
		 */
		bool is_inside(const math::vec3& p, float tol = 1.e-6f) const;

		bool intersec_segment
		(const math::vec3& p1, const math::vec3& p2) const;
		bool intersec_segment
		(const math::vec3& p1, const math::vec3& p2, math::vec3& p_inter) const;

		bool intersec_sphere(const math::vec3& c, float R) const;

		// OTHERS

		void update_particle(
			const math::vec3& pp, const math::vec3& pv,
			particles::free_particle *p
		) const;

		void correct_position(
			const math::vec3& pred_pos, const particles::sized_particle *p,
			math::vec3& correct_pos
		) const;

		void update_particle(
			const math::vec3& pp, const math::vec3& pv,
			particles::sized_particle *p
		) const;

		void display() const;
};

} // -- namespace geom
} // -- namespace sim
