#pragma once

// C++ includes
#include <iostream>

// physim includes
#include <physim/geometry/geometry.hpp>
#include <physim/geometry/plane.hpp>
#include <physim/particles/free_particle.hpp>
#include <physim/math/vec3.hpp>

namespace physim {
namespace geom {

/**
 * @brief Class that implements a triangle.
 *
 * A triangle is, informally, a polygonal object of three
 * sides of arbitrary length, whose endpoints are
 * defined by three vertices (see @ref v1, @ref v2, @ref v3).
 *
 * These three vertices all lie on a plane (see @ref pl),
 * the creation of which depends on the order of the vertices
 * they are given in.
 */
class triangle : public geometry {
	private:
		/// The first vertex of the triangle.
		math::vec3 v1;
		/// The second vertex of the triangle.
		math::vec3 v2;
		/// The third vertex of the triangle.
		math::vec3 v3;

		/// Plane associated to the triangle.
		plane pl;

	public:
		/// Default constructor.
		triangle();
		/**
		 * @brief Constructor with points.
		 *
		 * The plane associated to this triangle (see @ref pl)
		 * is built using these vertices in the same order they
		 * are given in this method.
		 *
		 * See
		 * @ref plane::plane(const math::vec3&,const math::vec3&,const math::vec3&)
		 * to see how the normal is determined.
		 */
		triangle(const math::vec3& p1,const math::vec3& p2,const math::vec3& p3);
		/// Copy constructor.
		triangle(const triangle& t);
		/// Destructor.
		~triangle();

		// OPERATORS

		// SETTERS

		/**
		 * @brief Sets the position of this triangle.
		 *
		 * The vertices of the triangle are translated according
		 * to vector @e v.
		 * @param v Vector representing the direction in which
		 * every vertex moves.
		 */
		void set_position(const math::vec3& v);

		// GETTERS

		/// Returns a constant reference to the assiociated plane (@ref pl).
		const plane& get_plane() const;

		bool is_inside(const math::vec3& p, float tol = 1.e-6f) const;

		geom_type get_geom_type() const;

		bool intersec_segment
		(const math::vec3& p1, const math::vec3& p2) const;
		bool intersec_segment
		(const math::vec3& p1, const math::vec3& p2, math::vec3& p_inter) const;

		// OTHERS

		void update_particle(
			const math::vec3& pp, const math::vec3& pv,
			particles::free_particle *p
		) const;

		void display(std::ostream& os = std::cout) const;
};

} // -- namespace geom
} // -- namespace sim
