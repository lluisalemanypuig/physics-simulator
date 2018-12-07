#pragma once

// C++ includes
#include <iostream>
#include <vector>

// physim includes
#include <physim/geometry/geometry.hpp>
#include <physim/particles/free_particle.hpp>
#include <physim/math/vec3.hpp>

namespace physim {
namespace geom {

/**
 * @brief Class that implements a triangular object.
 *
 * A triangular object is a collection of triangles which may be a triangle
 * soup or a triangular mesh.
 */
class object : public geometry {
	private:
		/// The vertices of this object.
		std::vector<math::vec3> verts;
		/**
		 * @brief Triangles of this object.
		 *
		 * Every three integer values we have a triangle. Each of
		 * this indices points to a value in @ref verts.
		 */
		std::vector<size_t> tris;

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
		 * Translates all vertices in @ref verts in the direction of
		 * vector @e v..
		 * @param v Vector.
		 */
		void set_position(const math::vec3& v);

		/// Sets the vertices of this object. See @ref verts.
		void set_vertices(const std::vector<math::vec3>& vs);
		/// Sets the triangles indices of this object. See @ref tris.
		void set_triangles(const std::vector<size_t>& trs);

		// GETTERS

		geom_type get_geom_type() const;

		/**
		 * @brief Returns the vertices of this object.
		 * @return Returns a constant reference to @ref verts.
		 */
		const std::vector<math::vec3>& get_vertices() const;

		/**
		 * @brief Returns the triangles of this object.
		 * @return Returns a constant reference to @ref tris.
		 */
		const std::vector<size_t>& get_triangles() const;

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

		void display(std::ostream& os = std::cout) const;
};

} // -- namespace geom
} // -- namespace sim
