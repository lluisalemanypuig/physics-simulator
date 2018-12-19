#include <physim/geometry/plane.hpp>

// C++ includes
#include <iostream>
using namespace std;

// physim includes
#include <physim/math/private/math3.hpp>

namespace physim {
using namespace math;
using namespace particles;

namespace geometric {

// PRIVATE

// PUBLIC

plane::plane() : geometry() {
	__pm3_assign_s(normal, 0.0f);
	dconst = 0.0f;
}

plane::plane(const vec3& n, const vec3& p) : geometry() {
	normalise(n,normal);
	dconst = -__pm3_dot(p, normal);
}

plane::plane(const vec3& n, float d) : geometry() {
	__pm3_normalise(normal,n);
	dconst = d;
}

plane::plane(const vec3& p0, const vec3& p1, const vec3& p2) : geometry() {
	__pm3_cross_diff(normal, p0,p1,p2);
	__pm3_normalise(normal,normal);
	dconst = -__pm3_dot(p0, normal);
}

plane::plane(const plane& p) : geometry(p) {
	__pm3_assign_v(normal, p.normal);
	dconst = p.dconst;
}

plane::~plane() { }

// SETTERS

void plane::set_position(const vec3& p) {
	dconst = -__pm3_dot(p, normal);
}

geometry_type plane::get_geom_type() const {
	return geometry_type::Plane;
}

float plane::dist_point_plane(const vec3& p) const {
	return __pm3_dot(p, normal) + dconst;
}

void plane::closest_point_plane(const vec3& p, vec3& closest) const {
	__pm3_add_v_vs(closest, p, normal,-dconst - __pm3_dot(p, normal));
}

const vec3& plane::get_normal() const {
	return normal;
}

float plane::get_constant() const {
	return dconst;
}

// GETTERS

bool plane::is_inside(const vec3& p, float tol) const {
	if ((__pm3_dot(p, normal) + dconst) <= tol) {
		return true;
	}
	return false;
}

bool plane::intersec_segment(const vec3& p1, const vec3& p2) const {
	float d1 = dist_point_plane(p1);
	float d2 = dist_point_plane(p2);
	return d1*d2 <= 0.0f;
}

bool plane::intersec_sphere(const vec3& c, float R) const {
	// find point at the surface of the sphere either in
	// the opposite direction of the sphere, or in the same
	// direction depending on the position of c with respect
	// to the plane
	float side = __pm3_dot(c,normal) + dconst;
	vec3 surf;
	if (side < 0.0f) {
		__pm3_add_v_vs(surf, c, normal,R);
	}
	else {
		__pm3_sub_v_vs(surf, c, normal,R);
	}
	// the plane intersects the sphere if the segment joining
	// the center and the point at the surface intersects the
	// plane.
	return intersec_segment(c, surf);
}

bool plane::intersec_segment(const vec3& p1, const vec3& p2, vec3& p_inter) const {
	if (not intersec_segment(p1,p2)) {
		return false;
	}

	// Use point of intersection temporarily.
	// It will be overwritten later.
	__pm3_sub_v_v(p_inter, p2, p1);
	float r = (-dconst - __pm3_dot(p1, normal))/__pm3_dot(p_inter, normal);
	__pm3_add_vs_vs(p_inter, p1,(1.0f - r), p2,r);
	return true;
}

// OTHERS

void plane::update_particle
(const vec3& pred_pos, const vec3& pred_vel, free_particle *p)
const
{
	p->save_position();

	// Wn is a vector normal to the plane with
	// direction towards the intersection point
	// (this point is not needed to calculate Wn)

	vec3 Wn;
	__pm3_mul_v_s(Wn, normal, __pm3_dot(pred_pos,normal) + dconst);

	// --- update position --- (with bouncing coefficient)

	float bounce = p->bouncing;
	__pm3_sub_v_vs(p->cur_pos, pred_pos, Wn, 1.0f + bounce);

	// --- update velocity (1) --- (with bouncing coefficient)

	// We need the velocity at time T, not the previous velocity (time T - dt)
	// for the 'friction operation'. A constant reference is not used because
	// we need to keep this value after update.
	vec3 vt;
	__pm3_assign_v(vt, p->cur_vel);

	// first update of the velociy (with bouncing)
	__pm3_sub_v_vs(p->cur_vel, pred_vel, normal,(1.0f + bounce)*__pm3_dot(normal, pred_vel));

	// --- update velocity (2) --- (with friction coefficient)

	// Use 'vt', the velocity at time t
	vec3 vT;
	__pm3_sub_v_vs(vT, vt, normal,__pm3_dot(normal,vt));
	__pm3_sub_acc_vs(p->cur_vel, vT, p->friction);
}

void plane::update_particle
(const vec3& pred_pos, const vec3& pred_vel, sized_particle *p)
const
{
	p->save_position();

	/*	1. We know that the predicted position is at a certain SIGNED
	 *		distance D from the plane.
	 *	2. Define a position P = pred_pos + sign(D)*normal*d, where
	 *		d = p->R - |D|.
	 *	   Notice P is at a distance R from the plane.
	 *	3. Define a plane 'Q' with normal 'normal' that goes through P
	 *	4. The last correct position of the sized particle 'p' is the
	 *		intersection between the line through pred_pos and F
	 *		and the plane 'Q'. Let 'I' be such position.
	 *		F is a point on the line through P with director vector
	 *		'pred_pos'. In particular, this point is 'behind' P in this
	 *		line.
	 *		4.0. Let v = pred_vel
	 *		4.1. Let u = v/||v||
	 *		4.2. Define the parametric line  l(k) : P + k*u
	 *		4.3. Let F = l(-1) (there will always be intersection as
	 *			 long as the time steps are small).
	 *	5. We only have to update the free particle associated to 'p'
	 *		with plane 'Q' considering the predicted position to be 'I'.
	 */

	float D = dist_point_plane(pred_pos);
	float d = (signbit(D) ? -1.0f : 1.0f)*(p->R - std::abs(D));

	vec3 P;
	__pm3_add_v_vs(P, pred_pos, normal,d);

	vec3 u_pred_vel;
	normalise(pred_vel, u_pred_vel);

	vec3 F;
	__pm3_add_v_vs(F, P, u_pred_vel,-1.0f);

	plane Q(normal, P);
	vec3 I;
	Q.intersec_segment(F, pred_pos, I);
	Q.update_particle(I, pred_vel, static_cast<free_particle *>(p));
}

void plane::display() const {
	cout << "I am a plane" << endl;
	cout << "    with plane equation:" << endl;
	cout << "        " << normal.x << "*x + "
					   << normal.y << "*y + "
					   << normal.z << "*z + "
					   << dconst << " = 0" << endl;
}

} // -- namespace geom
} // -- namespace sim
