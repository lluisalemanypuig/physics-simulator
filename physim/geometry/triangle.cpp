#include <physim/geometry/triangle.hpp>

// C++ includes
#include <iostream>
using namespace std;

// physim includes
#include <physim/math/private/math3.hpp>
#include <physim/geometry/sphere.hpp>

inline float triangle_area
(const physim::math::vec3 p1, const physim::math::vec3 p2, const physim::math::vec3 p3)
{
	physim::math::vec3 C;
	__pm3_cross_diff(C, p1,p2,p3);
	return __pm3_norm(C)/2.0f;
}

namespace physim {
namespace geometry {

// LOCAL-DEFINED

// PRIVATE

// PUBLIC

triangle::triangle() : geometry() { }

triangle::triangle
(const math::vec3& p1,const math::vec3& p2,const math::vec3& p3)
	: geometry(), pl(plane(p1,p2,p3))
{
	__pm3_assign_v(v1, p1);
	__pm3_assign_v(v2, p2);
	__pm3_assign_v(v3, p3);
}

triangle::triangle(const triangle& t)
	: geometry(t), pl(t.pl)
{
	__pm3_assign_v(v1, t.v1);
	__pm3_assign_v(v2, t.v2);
	__pm3_assign_v(v3, t.v3);
}

triangle::~triangle() { }

// SETTERS

void triangle::set_points
(const math::vec3& p1,const math::vec3& p2,const math::vec3& p3)
{
	__pm3_assign_v(v1, p1);
	__pm3_assign_v(v2, p2);
	__pm3_assign_v(v3, p3);
	pl = plane(p1,p2,p3);
}

void triangle::set_position(const math::vec3& v) {
	__pm3_add_acc_v(v1, v);
	__pm3_add_acc_v(v2, v);
	__pm3_add_acc_v(v3, v);
	pl.set_position(v1);
}

// GETTERS

const plane& triangle::get_plane() const {
	return pl;
}

bool triangle::is_inside(const math::vec3& p, float tol) const {
	// if the point is not inside the plane,
	// for sure it is not inside the triangle
	if (not pl.is_inside(p, tol)) {
		return false;
	}

	// compute areas of the triangles
	float a1 = triangle_area(p,  v2, v3);
	float a2 = triangle_area(v1,  p, v3);
	float a3 = triangle_area(v1, v2,  p);
	float A  = triangle_area(v1, v2, v3);

	// test inside/outside
	if ((a1 + a2 + a3 - A) <= tol) {
		return true;
	}
	return false;
}

geometry_type triangle::get_geom_type() const {
	return geometry_type::Triangle;
}

bool triangle::intersec_segment(const math::vec3& p1, const math::vec3& p2) const {
	math::vec3 intersection;
	return intersec_segment(p1, p2, intersection);
}

bool triangle::intersec_segment
(const math::vec3& p1, const math::vec3& p2, math::vec3& p_inter) const
{
	// if the segment does not intersect the plane
	// surely it does not intersect the triangle
	if (not pl.intersec_segment(p1,p2, p_inter)) {
		return false;
	}

	// intersection has been computed -> if this point
	// lies inside the triangle then there is intersection
	if (is_inside(p_inter)) {
		return true;
	}
	return false;
}

#define lambdas(p,q,C,r, pq,qc, a,b,c, l0,l1)		\
	__pm3_sub_v_v(pq, p, q);						\
	__pm3_sub_v_v(qc, q, C);						\
	a = __pm3_dot(pq,pq);							\
	b = 2.0f*__pm3_dot(pq, qc);						\
	c = __pm3_dot(qc,qc) - r*r;						\
	l0 = (-b + std::sqrt(b*b - 4.0f*a*c))/(2.0f*a);	\
	l1 = (-b - std::sqrt(b*b - 4.0f*a*c))/(2.0f*a)

bool triangle::intersec_sphere(const math::vec3& C, float R) const {
	/* Update: we should use the method outlined in
	 * https://www.geometrictools.com/Documentation/IntersectionMovingSphereTriangle.pdf
	 */



	// 1. If any of this triangle's points is inside the sphere
	// we have intersection
	if (
		__pm3_dist2(v1, C) <= R*R or
		__pm3_dist2(v2, C) <= R*R or
		__pm3_dist2(v3, C) <= R*R
	)
	{
		return true;
	}

	// 2. If any of the segments of triangle intersects
	// the sphere we have intersection.
	//	Define the segment from P to Q as:
	//			l*P + (1 - l)*Q
	//	Define the sphere as
	//			(X - C)**(X - C) = R*R
	//	There is intersectionbetween the segment and the
	//	sphere iff at least one of l0,l1 is in [0,1] where
	//
	//				 -b + sqrt(b^2 - 4*a*c)
	//			l0 = ----------------------
	//						  2*a
	//				 -b - sqrt(b^2 - 4*a*c)
	//			l1 = ----------------------
	//						  2*a
	//	with
	//			a = (P - Q)**(P - Q)
	//			b = 2*(P - Q)**(Q - C)
	//			c = (Q - C)**(Q - C) - R*R
	float a, b, c, l0, l1;
	math::vec3 pq, qc;

	lambdas(v1,v2,C,R, pq,qc, a,b,c, l0,l1);
	if ((0.0f <= l0 and l0 <= 1.0f) or (0.0f <= l1 and l1 <= 1.0f)) {
		return true;
	}
	lambdas(v2,v3,C,R, pq,qc, a,b,c, l0,l1);
	if ((0.0f <= l0 and l0 <= 1.0f) or (0.0f <= l1 and l1 <= 1.0f)) {
		return true;
	}
	lambdas(v3,v1,C,R, pq,qc, a,b,c, l0,l1);
	if ((0.0f <= l0 and l0 <= 1.0f) or (0.0f <= l1 and l1 <= 1.0f)) {
		return true;
	}

	return false;
}

// OTHERS

void triangle::update_particle
(const math::vec3& pred_pos, const math::vec3& pred_vel, particles::free_particle *p)
const
{
	pl.update_particle(pred_pos, pred_vel, p);
}

void triangle::correct_position(
	const math::vec3& pred_pos, const particles::sized_particle *p,
	math::vec3& correct_position
) const
{

}

void triangle::update_particle
(const math::vec3& pred_pos, const math::vec3& pred_vel, particles::sized_particle *p)
const
{
	// 1. Correct the position of the sized particle
	// as if the triangle was a plane:



	// 2. Update the position of the underlying free particle
}

void triangle::display(std::ostream& os) const {
	os << "I am a triangle" << std::endl;
	os << "    with vertices:" << std::endl;
	os << "        - Point({" << v1.x << "," << v1.y << "," << v1.z << "})" << std::endl;
	os << "        - Point({" << v2.x << "," << v2.y << "," << v2.z << "})" << std::endl;
	os << "        - Point({" << v3.x << "," << v3.y << "," << v3.z << "})" << std::endl;
	os << "    and plane equation:" << std::endl;
	const math::vec3& n = pl.get_normal();
	os << "        " << n.x << "*x + " << n.y << "*y + " << n.z << "*z + "
	   << pl.get_constant() << " = 0" << std::endl;
}

} // -- namespace geom
} // -- namespace sim
