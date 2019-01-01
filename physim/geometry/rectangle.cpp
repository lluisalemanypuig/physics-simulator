#include <physim/geometry/rectangle.hpp>

// C includes
#include <assert.h>

// C++ includes
#include <iostream>
using namespace std;

// physim includes
#include <physim/math/private/math3.hpp>

// LOCAL-DEFINED

typedef physim::math::vec3 pmvec3;
typedef physim::math::vec2 pmvec2;

static inline
float triangle_area(const pmvec3& v0,const pmvec3& v1,const pmvec3& v2) {
	pmvec3 C;
	__pm3_cross_diff(C, v0,v1,v2);
	return __pm3_norm(C)/2.0f;
}

enum rectangle_region {
	r0123,
	r0, r1, r2, r3,
	r01, r12, r23, r30
};

// locate point Y in the triangle (q0,q1,q2)
static inline
rectangle_region locate
(
	const pmvec2& q0, const pmvec2& q1,
	const pmvec2& q2, const pmvec2& q3,
	const pmvec2& q0n0, const pmvec2& q0n3,
	const pmvec2& q1n0, const pmvec2& q1n1,
	const pmvec2& q2n1, const pmvec2& q2n2,
	const pmvec2& q3n2, const pmvec2& q3n3,
	const pmvec2 Y
)
{
	// inside rectangle
	if (__pm2_left_aligned(q0,q1, Y) and
		__pm2_left_aligned(q1,q2, Y) and
		__pm2_left_aligned(q2,q3, Y) and
		__pm2_left_aligned(q3,q0, Y)
	)
	{
		return rectangle_region::r0123;
	}

	/* regions Rij */
	if (__pm2_left(q0,q0n0, Y) and
		__pm2_right(q0,q1, Y) and
		__pm2_right(q1,q1n0, Y)
	)
	{
		return rectangle_region::r01;
	}
	if (__pm2_left(q1,q1n1, Y) and
		__pm2_right(q1,q2, Y) and
		__pm2_right(q2,q2n1, Y)
	)
	{
		return rectangle_region::r12;
	}
	if (__pm2_left(q2,q2n2, Y) and
		__pm2_right(q2,q3, Y) and
		__pm2_right(q3,q3n2, Y)
	)
	{
		return rectangle_region::r23;
	}
	if (__pm2_left(q3,q3n3, Y) and
		__pm2_right(q3,q0, Y) and
		__pm2_right(q0,q0n3, Y)
	)
	{
		return rectangle_region::r30;
	}

	/* regions Ri */

	// in case of ties with Rij regions
	// (R2 with R12 or R2 with R20) choose Ri
	if ((__pm2_left(q1,q1n0, Y) and __pm2_right(q1, q1n1, Y)) or
		__pm2_aligned(q1,q1n0, Y) or __pm2_aligned(q1,q1n1, Y)
	)
	{
		return rectangle_region::r1;
	}
	if ((__pm2_left(q2,q2n1, Y) and __pm2_right(q2,q2n2, Y)) or
		__pm2_aligned(q2,q2n1, Y) or __pm2_aligned(q2,q2n2, Y)
	)
	{
		return rectangle_region::r2;
	}
	if ((__pm2_left(q3,q3n2, Y) and __pm2_right(q3,q3n3, Y)) or
		__pm2_aligned(q3,q3n2, Y) or __pm2_aligned(q3,q3n3, Y)
	)
	{
		return rectangle_region::r3;
	}

	return rectangle_region::r0;
}

namespace physim {
using namespace math;
using namespace particles;

namespace geometric {

// PRIVATE

// PUBLIC

rectangle::rectangle() : geometry() { }

rectangle::rectangle
(const vec3& _p0,const vec3& _p1, const vec3& _p2,const vec3& _p3)
: geometry()
{
	set_points(_p0, _p1, _p2, _p3);
}

rectangle::rectangle(const rectangle& r) : geometry(r) {
	__pm3_assign_v(p0, r.p0);
	__pm3_assign_v(p1, r.p1);
	__pm3_assign_v(p2, r.p2);
	__pm3_assign_v(p3, r.p3);

	__pm3_assign_v(u0, r.u0);
	__pm3_assign_v(u1, r.u1);
	__pm3_assign_v(u2, r.u2);

	__pm2_assign_v(q0, r.q0);
	__pm2_assign_v(q1, r.q1);
	__pm2_assign_v(q2, r.q2);
	__pm2_assign_v(q3, r.q3);

	__pm2_assign_v(e0, r.e0);
	__pm2_assign_v(e1, r.e1);
	__pm2_assign_v(e2, r.e2);
	__pm2_assign_v(e3, r.e3);

	__pm2_assign_v(n0, r.n0);
	__pm2_assign_v(n1, r.n1);
	__pm2_assign_v(n2, r.n2);
	__pm2_assign_v(n3, r.n3);

	__pm2_assign_v(q0n0, r.q0n0);
	__pm2_assign_v(q0n3, r.q0n3);
	__pm2_assign_v(q1n0, r.q1n0);
	__pm2_assign_v(q1n1, r.q1n1);
	__pm2_assign_v(q2n1, r.q2n1);
	__pm2_assign_v(q2n2, r.q2n2);
	__pm2_assign_v(q3n2, r.q3n2);
	__pm2_assign_v(q3n3, r.q3n3);
}

rectangle::~rectangle() { }

// SETTERS

void rectangle::set_points
(const math::vec3& _p0,const math::vec3& _p1,
 const math::vec3& _p2,const math::vec3& _p3)
{
	/* copy vertices */
	__pm3_assign_v(p0, _p0);
	__pm3_assign_v(p1, _p1);
	__pm3_assign_v(p2, _p2);
	__pm3_assign_v(p3, _p3);

	/* make box */
	__pm3_min3(vmin, p0,p1,p2);
	__pm3_max3(vmax, p0,p1,p2);
	__pm3_sub_acc_s(vmin, 0.01f);
	__pm3_add_acc_s(vmax, 0.01f);

	/* make vectors u0,u1,u2 */
	vec3 p0p1, p0p2, p0p3;
	__pm3_sub_v_v(p0p1, p1, p0);
	__pm3_sub_v_v(p0p2, p2, p0);
	__pm3_sub_v_v(p0p3, p3, p0);

	__pm3_cross(u2, p0p1, p0p2);
	normalise(u2, u2);
	__pm3_assign_v(u0, p0p1);
	normalise(u0, u0);
	__pm3_cross(u1, u2, u0);

	/* make local reference system */
	float l = __pm3_norm(p0p1);
	float alpha = __pm3_dot(u0, p0p2);
	float beta = __pm3_dot(u1, p0p2);
	float gamma = __pm3_dot(u0, p0p3);
	float delta = __pm3_dot(u1, p0p3);
	// points
	__pm2_assign_s(q0, 0.0f);
	__pm2_assign_c(q1, l, 0.0f);
	__pm2_assign_c(q2, alpha, beta);
	__pm2_assign_c(q3, gamma, delta);
	// edge vectors
	__pm2_assign_c(e0, l, 0.0f);
	__pm2_assign_c(e1, alpha - l, beta);
	__pm2_assign_c(e2, gamma - alpha, delta - beta);
	__pm2_assign_c(e3, -gamma, -delta);
	// normal edge vectors
	__pm2_assign_c(n0, 0.0f, -1.0f);
	__pm2_assign_c(n1, beta, l - alpha);
	normalise(n1, n1);
	__pm2_assign_c(n2, delta - beta, alpha - gamma);
	normalise(n2, n2);
	__pm2_assign_c(n3, -delta, gamma);
	normalise(n3, n3);
	// normal edge points
	__pm2_add_v_v(q0n0, q0, n0);
	__pm2_add_v_v(q0n3, q0, n3);
	__pm2_add_v_v(q1n0, q1, n0);
	__pm2_add_v_v(q1n1, q1, n1);
	__pm2_add_v_v(q2n1, q2, n1);
	__pm2_add_v_v(q2n2, q2, n2);
	__pm2_add_v_v(q3n2, q3, n2);
	__pm2_add_v_v(q3n3, q3, n3);

	// make plane
	pl = plane(u2, p1);
}

void rectangle::set_position(const vec3& v) {
	__pm3_add_acc_v(p0, v);
	__pm3_add_acc_v(p1, v);
	__pm3_add_acc_v(p2, v);
	__pm3_add_acc_v(p3, v);

	__pm3_add_acc_v(vmin, v);
	__pm3_add_acc_v(vmax, v);
	__pm3_sub_acc_s(vmin, 0.01f);
	__pm3_add_acc_s(vmax, 0.01f);

	// points q0,q1,q2 and vectors u0,u1,u2,n0,n1,n2
	// do not change since translations preserve distances.
}

// GETTERS

const plane& rectangle::get_plane() const {
	return pl;
}

void rectangle::get_points
(vec3& _p0, vec3& _p1, vec3& _p2, vec3& _p3)
const
{
	__pm3_assign_v(_p0, p0);
	__pm3_assign_v(_p1, p1);
	__pm3_assign_v(_p2, p2);
	__pm3_assign_v(_p3, p3);
}

void rectangle::projection(const vec3& X, vec3& proj) const {
	vec3 p0X;
	__pm3_sub_v_v(p0X, X, p0);

	// projection of point onto the plane
	// (in the local reference system)
	vec2 Y;
	__pm2_assign_c(Y, __pm3_dot(u0,p0X), __pm3_dot(u1,p0X));

	// easy regions: r012 (inside rectangle), r0,r1,r2
	rectangle_region R = locate(q0,q1,q2,q3, q0n0,q0n3,q1n0,q1n1,q2n1,q2n2,q3n2,q3n3, Y);
	switch (R) {
	case r0123:
		__pm3_add_vs_vs_v(proj, u0,__pm3_dot(u0,p0X), u1,__pm3_dot(u1,p0X), p0);
		break;
	case r0: __pm3_assign_v(proj, p0); return;
	case r1: __pm3_assign_v(proj, p1); return;
	case r2: __pm3_assign_v(proj, p2); return;
	case r3: __pm3_assign_v(proj, p3); return;
	default:
		;
	}

	// difficult regions (deimited by edges)
	// r01, r12, r23, r30
	float s;
	vec2 qY;
	switch (R) {
	case r01:
		__pm2_sub_v_v(qY, Y, q0);
		s = __pm2_dot(e0,qY)/__pm2_dot(e0,e0);
		__pm3_sub_v_v(proj, p1, p0);
		__pm3_add_v_vs(proj, p0, proj,s);
		break;
	case r12:
		__pm2_sub_v_v(qY, Y, q1);
		s = __pm2_dot(e1,qY)/__pm2_dot(e1,e1);
		__pm3_sub_v_v(proj, p2, p1);
		__pm3_add_v_vs(proj, p1, proj,s);
		break;
	case r23:
		__pm2_sub_v_v(qY, Y, q2);
		s = __pm2_dot(e2,qY)/__pm2_dot(e2,e2);
		__pm3_sub_v_v(proj, p3, p2);
		__pm3_add_v_vs(proj, p2, proj,s);
		break;
	case r30:
		__pm2_sub_v_v(qY, Y, q3);
		s = __pm2_dot(e3,qY)/__pm2_dot(e3,e3);
		__pm3_sub_v_v(proj, p0, p3);
		__pm3_add_v_vs(proj, p3, proj,s);
		break;
	default:
		;
	}
}

float rectangle::distance(const vec3& p) const {
	vec3 proj;
	projection(p, proj);
	return __pm3_dist(p, proj);
}

bool rectangle::is_inside(const vec3& x, float tol) const {
	// check if the point is inside the bounding box
	if (not __pm3_inside_box(x, vmin,vmax)) {
		return false;
	}

	// If the point is inside the bounding box then it
	// might be 'inside' the associated plane.

	if (not pl.is_inside(x, tol)) {
		return false;
	}

	// If the point is on the associated plane
	// and 'inside' any of the two triangles
	// then the point is in the rectangle

	float a0, a1, a2, A;

	// compute areas of the triangles (1)
	a0 = triangle_area(x,  p1, p2);
	a1 = triangle_area(p0,  x, p2);
	a2 = triangle_area(p0, p1,  x);
	if (a0 <= tol or a1 <= tol or a2 <= tol) {
		// collinearities
		return true;
	}
	A = triangle_area(p0, p1, p2);

	// test inside/outside
	if ((a0 + a1 + a2 - A) <= tol) {
		return true;
	}

	// compute areas of the triangles (2)
	a0 = triangle_area(x,  p2, p3);
	a1 = triangle_area(p0,  x, p3);
	a2 = triangle_area(p0, p2,  x);
	if (a0 <= tol or a1 <= tol or a2 <= tol) {
		// collinearities
		return true;
	}
	A = triangle_area(p0, p2, p3);

	// test inside/outside
	if ((a0 + a1 + a2 - A) <= tol) {
		return true;
	}
	return false;
}

geometry_type rectangle::get_geom_type() const {
	return geometry_type::Rectangle;
}

bool rectangle::intersec_segment
(const vec3& p1, const vec3& p2) const
{
	vec3 intersection;
	return intersec_segment(p1, p2, intersection);
}

bool rectangle::intersec_segment
(const vec3& _p0, const vec3& _p1, vec3& p_inter) const
{
	// if the segment does not intersect the plane
	// surely it does not intersect the rectangle
	bool inter = pl.intersec_segment(_p0,_p1, p_inter);
	if (not inter) {
		return false;
	}

	// intersection has been computed -> if this point
	// lies inside the rectangle then there is intersection
	if (is_inside(p_inter)) {
		return true;
	}
	return false;
}

bool rectangle::intersec_sphere(const vec3& c, float R) const {
	// if the distance between the projection of
	// the center onto the triangle and the center
	// is smaller than the radius we have intersection
	vec3 proj;
	projection(c, proj);
	return __pm3_dist2(c, proj) <= R*R;
}

// OTHERS

void rectangle::update_particle
(const vec3& pred_pos, const vec3& pred_vel, free_particle& p)
const
{
	// Since a precondition is that the
	// segment intersects the triangle...
	// well, we can just use the plane
	pl.update_particle(pred_pos, pred_vel, p);
}

void rectangle::update_particle
(const vec3& pred_pos, const vec3& pred_vel, sized_particle& p)
const
{
	// no need to save the position because
	// the plane will do it for us

	// 0. Compute projection of predicted position
	// onto the triangle.
	vec3 proj;
	projection(pred_pos, proj);

	// 1. Correct the position of the sized particle
	//    (code copied to avoid recomputation of vectors)

	float D = __pm3_dist(proj, pred_pos);

	vec3 vel_normal;
	normalise(pred_vel, vel_normal);
	vec3 cor_pos;
	__pm3_sub_v_vs(cor_pos, pred_pos, vel_normal, p.R - D);

	// 2. Update the position of the underlying free particle
	// 2.1. Compute normal of tangent plane
	vec3 dir;
	__pm3_sub_v_v(dir, pred_pos, proj);
	normalise(dir,dir);

	plane T(dir, cor_pos);
	T.update_particle(pred_pos, pred_vel, static_cast<free_particle&>(p));
}

void rectangle::display() const {
	cout << "I am a rectangle" << endl;
	cout << "    with vertices:" << endl;
	cout << "        - Point({" << p0.x << "," << p0.y << "," << p0.z << "})" << endl;
	cout << "        - Point({" << p1.x << "," << p1.y << "," << p1.z << "})" << endl;
	cout << "        - Point({" << p2.x << "," << p2.y << "," << p2.z << "})" << endl;
	cout << "        - Point({" << p3.x << "," << p3.y << "," << p3.z << "})" << endl;
	cout << "    and plane equation:" << endl;
	const vec3& n = pl.get_normal();
	cout << "        " << n.x << "*x + " << n.y << "*y + " << n.z << "*z + "
		 << pl.get_constant() << " = 0" << endl;
}

} // -- namespace geom
} // -- namespace sim
