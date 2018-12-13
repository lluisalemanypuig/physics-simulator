#pragma once

// C++ includes

// physim includes
#include <physim/geometry/geometry.hpp>
#include <physim/geometry/plane.hpp>
#include <physim/particles/free_particle.hpp>
#include <physim/particles/sized_particle.hpp>
#include <physim/math/vec3.hpp>

namespace physim {
namespace geometry {

/**
 * @brief Class that implements a rectangle.
 *
 * A rectangle is, informally, a polygonal object of four
 * sides of same opposite length, whose endpoints are
 * defined by four vertices (see @ref p0, @ref p1, @ref p2, @ref p3).
 *
 * These four vertices all lie on a plane (see @ref pl),
 * the creation of which depends on the order of the first
 * three vertices they are given in.
 */
class rectangle : public geometry {
	private:
		/// The first vertex of the rectangle.
		math::vec3 p0;
		/// The second vertex of the rectangle.
		math::vec3 p1;
		/// The third vertex of the rectangle.
		math::vec3 p2;
		/// The fourth vertex of the rectangle.
		math::vec3 p3;

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

		/**
		 * @brief Return the points of this rectangle.
		 * @param[out] _p0 First point of the rectangle. See @ref p0.
		 * @param[out] _p1 Second point of the rectangle. See @ref p1.
		 * @param[out] _p2 Third point of the rectangle. See @ref p2.
		 * @param[out] _p3 Fourth point of the rectangle. See @ref p3.
		 */
		void get_points
		(math::vec3& _p0, math::vec3& _p1, math::vec3& _p2, math::vec3& _p3)
		const;

		/**
		 * @brief Project a point to this triangle.
		 *
		 * The projected point is the closest point inside the triangle
		 * to point @e p.
		 * @param[in] p Point to measure the distance to the triangle.
		 * @param[out] proj Projection of @e p onto the closest point to it
		 * inside the triangle.
		 */
		void projection(const math::vec3& p, math::vec3& proj) const;

		/**
		 * @brief Computes the distance between a point and this triangle.
		 * @param[in] p Point to measure the distance to the triangle.
		 * @return Returns the distance between @e p and this triangle.
		 */
		float distance(const math::vec3& p) const;

		bool is_inside(const math::vec3& p, float tol = 1.e-6f) const;

		geometry_type get_geom_type() const;

		bool intersec_segment
		(const math::vec3& _p1, const math::vec3& _p2) const;
		bool intersec_segment
		(const math::vec3& _p1, const math::vec3& _p2, math::vec3& p_inter) const;

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
