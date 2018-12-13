#include <physim/geometry/triangle.hpp>

// C++ includes
#include <iostream>
using namespace std;

// physim includes
#include <physim/math/private/math2.hpp>
#include <physim/math/private/math3.hpp>
#include <physim/geometry/sphere.hpp>

// LOCAL-DEFINED

static inline
float triangle_area(
	const physim::math::vec3& v0,
	const physim::math::vec3& v1,
	const physim::math::vec3& v2
)
{
	physim::math::vec3 C;
	__pm3_cross_diff(C, v0,v1,v2);
	return __pm3_norm(C)/2.0f;
}

enum region {
	r012,
	r0, r1, r2,
	r01, r12, r20
};

// locate point Y in the triangle (q0,q1,q2)
static inline
region locate
(
	const physim::math::vec2& q0, const physim::math::vec2& q1,
	const physim::math::vec2& q2,
	const physim::math::vec2& q0n0, const physim::math::vec2& q0n2,
	const physim::math::vec2& q1n0, const physim::math::vec2& q1n1,
	const physim::math::vec2& q2n1, const physim::math::vec2& q2n2,
	const physim::math::vec2 Y
)
{
	// inside triangle
	if (__pm2_left_aligned(q0,q1, Y) and
		__pm2_left_aligned(q1,q2, Y) and
		__pm2_left_aligned(q2,q0, Y)
	)
	{
		return r012;
	}

	/* regions Rij */
	if (__pm2_left(q0,q0n0, Y) and
		__pm2_right(q0,q1, Y) and
		__pm2_right(q1,q1n0, Y)
	)
	{
		return r01;
	}
	if (__pm2_left(q1,q1n1, Y) and
		__pm2_right(q1,q2, Y) and
		__pm2_right(q2,q2n1, Y)
	)
	{
		return r12;
	}
	if (__pm2_left(q2,q2n2, Y) and
		__pm2_right(q2,q0, Y) and
		__pm2_right(q0,q0n2, Y)
	)
	{
		return r20;
	}

	/* regions Ri */

	// in case of ties with Rij regions
	// (R2 with R12 or R2 with R20) choose Ri
	if ((__pm2_left(q1,q1n0, Y) and __pm2_right(q1, q1n1, Y)) or
		__pm2_aligned(q1,q1n0, Y) or __pm2_aligned(q1,q1n1, Y)
	)
	{
		return r1;
	}
	if ((__pm2_left(q2,q2n1, Y) and __pm2_right(q2,q2n2, Y)) or
		__pm2_aligned(q2,q2n1, Y) or __pm2_aligned(q2,q2n2, Y)
	)
	{
		return r2;
	}

	return r0;
}

namespace physim {
using namespace math;
using namespace particles;

namespace geometry {

// PRIVATE

// PUBLIC

triangle::triangle() : geometry() { }

triangle::triangle
(const vec3& _p0,const vec3& _p1,const vec3& _p2)
	: geometry()
{
	set_points(_p0,_p1,_p2);
}

triangle::triangle(const triangle& t)
	: geometry(t), pl(t.pl)
{
	__pm3_assign_v(p0, t.p0);
	__pm3_assign_v(p1, t.p1);
	__pm3_assign_v(p2, t.p2);

	__pm3_assign_v(u0, t.u0);
	__pm3_assign_v(u1, t.u1);
	__pm3_assign_v(u2, t.u2);

	__pm2_assign_v(e0, t.e0);
	__pm2_assign_v(e1, t.e1);
	__pm2_assign_v(e2, t.e2);

	__pm2_assign_v(q0, t.q0);
	__pm2_assign_v(q1, t.q1);
	__pm2_assign_v(q2, t.q2);

	__pm2_assign_v(n0, t.n0);
	__pm2_assign_v(n1, t.n1);
	__pm2_assign_v(n2, t.n2);

	__pm2_assign_v(q0n0, t.q0n0);
	__pm2_assign_v(q0n2, t.q0n2);
	__pm2_assign_v(q1n0, t.q1n0);
	__pm2_assign_v(q1n1, t.q1n1);
	__pm2_assign_v(q2n1, t.q2n1);
	__pm2_assign_v(q2n2, t.q2n2);
}

triangle::~triangle() { }

// SETTERS

void triangle::set_points
(const vec3& _p0,const vec3& _p1,const vec3& _p2)
{
	/* copy vertices */
	__pm3_assign_v(p0, _p0);
	__pm3_assign_v(p1, _p1);
	__pm3_assign_v(p2, _p2);

	/* make box */
	__pm3_min3(vmin, p0,p1,p2);
	__pm3_max3(vmax, p0,p1,p2);
	__pm3_sub_acc_s(vmin, 0.01f);
	__pm3_add_acc_s(vmax, 0.01f);

	/* make vectors u0,u1,u2 */
	vec3 p0p1, p0p2;
	__pm3_sub_v_v(p0p1, p1, p0);
	__pm3_sub_v_v(p0p2, p2, p0);

	__pm3_cross(u2, p0p1, p0p2);
	normalise(u2, u2);
	__pm3_assign_v(u0, p0p1);
	normalise(u0, u0);
	__pm3_cross(u1, u2, u0);

	/* make local reference system */
	float l = __pm3_norm(p0p1);
	float alpha = __pm3_dot(u0, p0p2);
	float beta = __pm3_dot(u1, p0p2);
	// points
	__pm2_assign_s(q0, 0.0f);
	__pm2_assign_c(q1, l, 0.0f);
	__pm2_assign_c(q2, alpha, beta);
	// edge vectors
	__pm2_assign_c(e0, l, 0.0f);
	__pm2_assign_c(e1, alpha - l, beta);
	__pm2_assign_c(e2, -alpha, -beta);
	// normal edge vectors
	__pm2_assign_c(n0, 0.0f, -1.0f);
	__pm2_assign_c(n1, beta, l - alpha);
	normalise(n1, n1);
	__pm2_assign_c(n2, -beta, alpha);
	normalise(n2, n2);
	// normal edge points
	__pm2_add_v_v(q0n0, q0, n0);
	__pm2_add_v_v(q1n0, q1, n0);
	__pm2_add_v_v(q1n1, q1, n1);
	__pm2_add_v_v(q2n1, q2, n1);
	__pm2_add_v_v(q2n2, q2, n2);
	__pm2_add_v_v(q0n2, q0, n2);

	// make plane
	pl = plane(u2, p1);
}

void triangle::set_position(const vec3& v) {
	__pm3_add_acc_v(p0, v);
	__pm3_add_acc_v(p1, v);
	__pm3_add_acc_v(p2, v);
	pl.set_position(p0);

	__pm3_add_acc_v(vmin, v);
	__pm3_add_acc_v(vmax, v);
	__pm3_sub_acc_s(vmin, 0.01f);
	__pm3_add_acc_s(vmax, 0.01f);

	// points q0,q1,q2 and vectors u0,u1,u2,n0,n1,n2
	// do not change since translations preserve distances.
}

// GETTERS

const plane& triangle::get_plane() const {
	return pl;
}

void triangle::get_points(vec3& _p0, vec3& _p1, vec3& _p2) const {
	__pm3_assign_v(_p0, p0);
	__pm3_assign_v(_p1, p1);
	__pm3_assign_v(_p2, p2);
}

void triangle::projection(const vec3& X, vec3& proj) const {
	vec3 p0X;
	__pm3_sub_v_v(p0X, X, p0);

	// projection of point onto the plane
	// (in the local reference system)
	vec2 Y;
	__pm2_assign_c(Y, __pm3_dot(u0,p0X), __pm3_dot(u1,p0X));

	// easy regions: r012 (inside triangle), r0,r1,r2
	region R = locate(q0,q1,q2, q0n0,q0n2,q1n0,q1n1,q2n1,q2n2, Y);
	switch (R) {
	case r012:
		__pm3_add_vs_vs_v(proj, u0,__pm3_dot(u0,p0X), u1,__pm3_dot(u1,p0X), p0);
		break;
	case r0: __pm3_assign_v(proj, p0); return;
	case r1: __pm3_assign_v(proj, p1); return;
	case r2: __pm3_assign_v(proj, p2); return;
	default:
		;
	}

	// difficult regions (deimited by edges)
	// r01, r12, r20
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
	case r20:
		__pm2_sub_v_v(qY, Y, q2);
		s = __pm2_dot(e2,qY)/__pm2_dot(e2,e2);
		__pm3_sub_v_v(proj, p0, p2);
		__pm3_add_v_vs(proj, p2, proj,s);
		break;
	default:
		;
	}
}

float triangle::distance(const vec3& X) const {
	vec3 p0X;
	__pm3_sub_v_v(p0X, X, p0);

	// projection of point onto the plane
	// (in the local reference system)
	vec2 Y;
	__pm2_assign_c(Y, __pm3_dot(u0,p0X), __pm3_dot(u1,p0X));

	// easy regions: r012 (inside triangle), r0,r1,r2
	region R = locate(q0,q1,q2, q0n0,q0n2,q1n0,q1n1,q2n1,q2n2, Y);
	switch (R) {
	case r012: return std::abs(__pm3_dot(u2,p0X));
	case r0: return __pm3_dist(p0, X);
	case r1: return __pm3_dist(p1, X);
	case r2: return __pm3_dist(p2, X);
	default:
		;
	}

	// difficult regions (deimited by edges)
	// r01, r12, r20
	float s;
	vec2 qY;
	vec3 proj;
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
	case r20:
		__pm2_sub_v_v(qY, Y, q2);
		s = __pm2_dot(e2,qY)/__pm2_dot(e2,e2);
		__pm3_sub_v_v(proj, p0, p2);
		__pm3_add_v_vs(proj, p2, proj,s);
		break;
	default:
		;
	}

	return __pm3_dist(X, proj);
}

bool triangle::is_inside(const vec3& p, float tol) const {
	// check if point is inside the box
	if (not __pm3_inside_box(p, vmin,vmax)) {
		return false;
	}

	// if the point is not inside the plane,
	// for sure it is not inside the triangle
	if (not pl.is_inside(p, tol)) {
		return false;
	}

	// compute areas of the triangles
	float a1 = triangle_area(p,  p1, p2);
	float a2 = triangle_area(p0,  p, p2);
	float a3 = triangle_area(p0, p1,  p);
	float A  = triangle_area(p0, p1, p2);

	// test inside/outside
	if ((a1 + a2 + a3 - A) <= tol) {
		return true;
	}
	return false;
}

geometry_type triangle::get_geom_type() const {
	return geometry_type::Triangle;
}

bool triangle::intersec_segment(const vec3& _p0, const vec3& _p1) const {
	vec3 intersection;
	return intersec_segment(_p0, _p1, intersection);
}

bool triangle::intersec_segment
(const vec3& _p0, const vec3& _p1, vec3& p_inter) const
{
	// if the segment does not intersect the plane
	// surely it does not intersect the triangle
	if (not pl.intersec_segment(_p0,_p1, p_inter)) {
		return false;
	}

	// intersection has been computed -> if this point
	// lies inside the triangle then there is intersection
	if (is_inside(p_inter)) {
		return true;
	}
	return false;
}

bool triangle::intersec_sphere(const vec3& C, float R) const {
	// if the distance between the center and the
	// triangle is smaller than the radius we have
	// intersection
	float D = distance(C);
	return D <= R;
}

// OTHERS

void triangle::update_particle
(const vec3& pred_pos, const vec3& pred_vel, free_particle *p)
const
{
	pl.update_particle(pred_pos, pred_vel, p);
}

void triangle::correct_position(
	const vec3& pred_pos, const sized_particle *p,
	vec3& correct_position
) const
{
	float D = distance(pred_pos);
	vec3 vel_normal;
	normalise(p->cur_vel, vel_normal);
	__pm3_sub_v_vs(correct_position, pred_pos, vel_normal, D);
}

void triangle::update_particle
(const vec3& pred_pos, const vec3& pred_vel, sized_particle *p)
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
	__pm3_sub_v_vs(cor_pos, pred_pos, vel_normal, p->R - D);

	// 2. Update the position of the underlying free particle
	// 2.1. Compute normal of tangent plane
	vec3 dir;
	__pm3_sub_v_v(dir, pred_pos, proj);
	normalise(dir,dir);

	plane T(dir, cor_pos);
	T.update_particle(pred_pos, pred_vel, static_cast<free_particle *>(p));
}

void triangle::display() const {
	cout << "I am a triangle" << std::endl;
	cout << "    with vertices:" << std::endl;
	cout << "        - Point({" << p0.x << "," << p0.y << "," << p0.z << "})" << std::endl;
	cout << "        - Point({" << p1.x << "," << p1.y << "," << p1.z << "})" << std::endl;
	cout << "        - Point({" << p2.x << "," << p2.y << "," << p2.z << "})" << std::endl;
	cout << "    and plane equation:" << std::endl;
	const vec3& n = pl.get_normal();
	cout << "        " << n.x << "*x + " << n.y << "*y + " << n.z << "*z + "
		 << pl.get_constant() << " = 0" << std::endl;
}

} // -- namespace geom
} // -- namespace sim
