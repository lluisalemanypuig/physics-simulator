#pragma once

// C++ includes
#include <iostream>
using namespace std;

// glm includes
#include <glm/glm.hpp>
using namespace glm;

// physim includes
#include <physim/geometry/geometry.hpp>
#include <physim/geometry/plane.hpp>
#include <physim/particles/particle.hpp>

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
		vec3 v1;
		/// The second vertex of the triangle.
		vec3 v2;
		/// The third vertex of the triangle.
		vec3 v3;

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
		 * See @ref plane::plane(const vec3&,const vec3&,const vec3&)
		 * to see how the normal is determined.
		 */
		triangle(const vec3& p1,const vec3& p2,const vec3& p3);
		/// Copy constructor.
		triangle(const triangle& t);
		/// Destructor.
		~triangle();

		// OPERATORS

		inline friend
		ostream& operator<< (ostream& os, const triangle& t) {
			os << "I am a triangle" << endl;
			os << "    with vertices:" << endl;
			os << "        - Point({" << t.v1.x << "," << t.v1.y << "," << t.v1.z << "})" << endl;
			os << "        - Point({" << t.v2.x << "," << t.v2.y << "," << t.v2.z << "})" << endl;
			os << "        - Point({" << t.v3.x << "," << t.v3.y << "," << t.v3.z << "})" << endl;
			os << "    and plane equation:" << endl;
			const vec3& n = t.pl.get_normal();
			os << "        " << n.x << "*x + " << n.y << "*y + " << n.z << "*z + "
			   << t.pl.get_constant() << " = 0" << endl;

			return os;
		}

		// SETTERS

		/**
		 * @brief Sets the position of this triangle.
		 *
		 * The vertices of the triangle are translated according
		 * to vector @e v.
		 * @param v Vector representing the direction in which
		 * every vertex moves.
		 */
		void set_position(const vec3& v);

		// GETTERS

		/// Returns a constant reference to the assiociated plane (@ref pl).
		const plane& get_plane() const;

		bool is_inside(const vec3& p, float tol = 1.e-6f) const;

		geom_type get_geom_type() const;

		bool intersec_segment(const vec3& p1, const vec3& p2) const;
		bool intersec_segment(const vec3& p1, const vec3& p2, vec3& p_inter) const;

		// OTHERS

		void update_upon_collision(const vec3& pred_pos, const vec3& pred_vel, particle *p) const;
		void display(ostream& os = cout) const;
};

} // -- namespace geom
} // -- namespace sim
