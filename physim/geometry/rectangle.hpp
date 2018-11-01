#pragma once

// C++ includes
#include <iostream>

// physim includes
#include <physim/geometry/geometry.hpp>
#include <physim/geometry/plane.hpp>
#include <physim/particles/particle.hpp>
#include <physim/math/vec3.hpp>

namespace physim {
namespace geom {

/**
 * @brief Class that implements a rectangle.
 *
 * A rectangle is, informally, a polygonal object of four
 * sides of same opposite length, whose endpoints are
 * defined by four vertices (see @ref v1, @ref v2, @ref v3, @ref v4).
 *
 * These four vertices all lie on a plane (see @ref pl),
 * the creation of which depends on the order of the first
 * three vertices they are given in.
 */
class rectangle : public geometry {
	private:
		/// The first vertex of the rectangle.
		math::vec3 v1;
		/// The second vertex of the rectangle.
		math::vec3 v2;
		/// The third vertex of the rectangle.
		math::vec3 v3;
		/// The fourth vertex of the rectangle.
		math::vec3 v4;

		/// Minimum x,y,z-coordinates.
		math::vec3 min;
		/// Maximum x,y,z-coordinates.
		math::vec3 max;

		/// Plane associated to the rectangle.
		plane pl;

	public:
		/// Default constructor.
		rectangle();
		/**
		 * @brief Constructor with points.
		 *
		 * The plane associated to this rectangle (see @ref pl)
		 * is built using the first three vertices in the same
		 * order they are given in this method.
		 *
		 * See
		 * @ref plane::plane(const math::vec3&,const math::vec3&,const math::vec3&)
		 * to see how the normal is determined.
		 *
		 * The four points must lie on the same plane, that is,
		 * the fourth point must lie on the plane made with the
		 * first three vertices.
		 */
		rectangle(
			const math::vec3& p1,const math::vec3& p2,
			const math::vec3& p3,const math::vec3& p4
		);
		/// Copy constructor.
		rectangle(const rectangle& t);
		/// Destructor.
		~rectangle();

		// OPERATORS

		// SETTERS

		/**
		 * @brief Sets the position of this rectangle.
		 *
		 * The vertices of the rectangle are translated according
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

		void update_particle
		(const math::vec3& pred_pos, const math::vec3& pred_vel, particle *p) const;

		void display(std::ostream& os = std::cout) const;
};

} // -- namespace geom
} // -- namespace sim
