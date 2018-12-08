#include <physim/geometry/sphere.hpp>

// C includes
#include <assert.h>

// physim includes
#include <physim/geometry/plane.hpp>
#include <physim/math/private/math3.hpp>

namespace physim {
namespace geometry {

// PRIVATE

// PUBLIC

sphere::sphere() : geometry() { }

sphere::sphere(const math::vec3& c, float r) : geometry() {
	__pm3_assign_v(C, c);
	R = r;
}

sphere::sphere(const sphere& s) : geometry(s) {
	__pm3_assign_v(C, s.C);
	R = s.R;
}

sphere::~sphere() { }

// SETTERS

void sphere::set_position(const math::vec3& p) {
	__pm3_assign_v(C, p);
}

// GETTERS

void sphere::set_radius(float r) {
	R = r;
}

// GETTERS

const math::vec3& sphere::get_centre() const {
	return C;
}

float sphere::get_radius() const {
	return R;
}

bool sphere::is_inside(const math::vec3& p, float tol) const {
	// compute squared distance from centre to p
	return ((__pm3_dist2(C,p)) - R*R) <= tol;
}

geometry_type sphere::get_geom_type() const {
	return geometry_type::Sphere;
}

bool sphere::intersec_segment(const math::vec3& p1, const math::vec3& p2) const {
	bool p1_in = is_inside(p1);
	bool p2_in = is_inside(p2);
	return (p1_in and not p2_in) or (not p1_in and p2_in);
}

bool sphere::intersec_segment(const math::vec3& p, const math::vec3& q, math::vec3& p_inter) const {
	if (not intersec_segment(p,q)) {
		return false;
	}

	/*
	 * The intersection point between a segment and
	 * a sphere is the point of the segment
	 *          I = (1-L)*p + L*q, for L in [0,1]
	 * such that
	 *     (I - C)**(I - C) - r = 0,
	 * where ** represents the dot product
	 *
	 * Upon arithmetic manipulations we obtain that
	 * we have to solve the following quadratic equation
	 * in L:
	 *   a*L^2 + b*L + c = 0
	 *
	 * where
	 *	a = v**v
	 *	b = 2( v**(p - C) )
	 *	c = C**C + p**p - 2(p**C) - r*r
	 */

	// coefficients of quadratic equation
	math::vec3 p_minus_C;
	__pm3_sub_v_v(p_minus_C, p, C);

	math::vec3 v;
	__pm3_sub_v_v(v, q, p);

	float a = __pm3_dot(v,v);
	float b = 2.0f*__pm3_dot(p_minus_C,v);
	float c = __pm3_dot(C,C) + __pm3_dot(p,p) - 2.0f*__pm3_dot(p,C) - R*R;

	// discriminant of the quadratic equation
	float discr = b*b - 4.0f*a*c;

	// allow negative values for the discriminant
	// as long as they are close enough to 0
	if (discr < 0.0f and std::abs(discr) <= 0.01f) {
		discr = 0.0f;
		#if defined (DEBUG)
		std::cerr << "sphere::intersec_segment - corrected discriminant" << std::endl;
		#endif
	}

	#if defined (DEBUG)
	if (discr < 0.0f) {
		std::cerr << "sphere::intersec_segment - Error:" << std::endl;
		std::cerr << "    Negative value for the discriminant: " << discr << std::endl;
		std::cerr << "    The application will crash" << std::endl;
	}
	#endif

	// make sure the discriminant is positive so as
	// to be safe when computing the square root
	assert(discr >= 0.0f);

	// compute both solutions of the equation and
	// take the one that is closest to the interval [0,1]
	float Lp = (-b + std::sqrt(discr))/(2.0f*a);
	float Lm = (-b - std::sqrt(discr))/(2.0f*a);

	float L, dp, dm;
	L = dp = dm = 0.0f;
	if (0.0f <= Lp and Lp <= 1.0f) {
		L = Lp;
	}
	else if (0.0f <= Lm and Lm <= 1.0f) {
		L = Lm;
	}
	else {
		// bit tricks? if Lp is smaller than 0 .., else if Lp is greater than 1 ...
		dp = (Lp < 0.0f)*std::abs(Lp) + (Lp > 1.0f)*(Lp - 1.0f);
		// bit tricks? if Lm is smaller than 0 .., else if Lm is greater than 1 ...
		dm = (Lm < 0.0f)*std::abs(Lm) + (Lm > 1.0f)*(Lm - 1.0f);
		// take value closest to the interval
		L = (dp < dm ? Lp : Lm);
		// if L is larger than 1 ... else if smaller than 0 ... else keep L
		L = (L > 1.0f)*1.0f + (L < 0.0f)*(0.0f) + (0.0f <= L and L <= 1.0f)*L;
	}

	// compute intersection point
	__pm3_add_vs_vs(p_inter, p,(1.0f - L), q,L);
	return true;
}

bool sphere::intersec_sphere(const math::vec3& c, float r) const {
	return __pm3_dist2(C, c) - (R + r)*(R + r) <= 0.0f;
}

// OTHERS

void sphere::update_particle
(const math::vec3& pred_pos, const math::vec3& pred_vel, particles::free_particle *p)
const
{
	// define a plane tangent to the sphere
	// that goes through the intersection point

	// compute intersection point

	math::vec3 I;
	intersec_segment(p->cur_pos, pred_pos, I);

	// define the plane
	math::vec3 normal;
	__pm3_sub_v_v(normal, C, I);
	plane tan_plane(normal,I);

	// make the plane update the particle
	tan_plane.update_particle(pred_pos, pred_vel, p);

	// If the distance between the predicted position
	// and the center is less than the radius then
	// it means that the particle has pierced through
	// the sphere's surface. If this happens, move the
	// position to the surface (and a bit farther).
	if (__pm3_dist2(p->cur_pos, C) < R*R) {
		__pm3_sub_v_v(normal, p->cur_pos, C);
		__pm3_normalise(normal, normal);
		__pm3_add_v_vs(p->cur_pos, C, normal,R + 0.1f);
	}
}

void sphere::correct_position(
	const math::vec3& pred_pos, const particles::sized_particle *p,
	math::vec3& correct_position
) const
{
	/* Let r be the particle's radius.
	 *
	 * The correct position of the particle I is at a distance R + r
	 * of the sphere's center. This position is the intersection
	 * between the segment from the particle's current position and
	 * its predicted position, and the sphere of radius R + r (the
	 * center does not change). Let S be this sphere.
	 *
	 * This intersection is guaranteed to exist because the current
	 * position of the particle is at a distance greater than R + r
	 * of the center of the sphere.
	 */

	float new_R = R + p->R;
	sphere S(C, new_R);
	S.intersec_segment(pred_pos, p->cur_pos, correct_position);
}

void sphere::update_particle
(const math::vec3& pred_pos, const math::vec3& pred_vel, particles::sized_particle *p)
const
{
	/* Let r be the particle's radius.
	 *
	 * The correct position of the particle I is at a distance R + r
	 * of the sphere's center. This position is the intersection
	 * between the segment from the particle's current position and
	 * its predicted position, and the sphere of radius R + r (the
	 * center does not change). Let S be this sphere.
	 *
	 * This intersection is guaranteed to exist because the current
	 * position of the particle is at a distance greater than R + r
	 * of the center of the sphere.
	 *
	 * The updated particle is obtained by updating a free particle
	 * at a predicted position I using a plane with normal a unit
	 * vector from the center to I.
	 */

	math::vec3 I;

	float new_R = R + p->R;
	sphere S(C, new_R);
	S.intersec_segment(pred_pos, p->cur_pos, I);

	math::vec3 normal;
	__pm3_sub_v_v(normal, C, I);
	plane tan_plane(normal,I);

	// make the plane update the particle
	tan_plane.update_particle
	(pred_pos, pred_vel, static_cast<particles::free_particle *>(p));
}

void sphere::display(std::ostream& os) const {
	os << "I am a sphere" << std::endl;
	os << "    with centre: (" << C.x << "," << C.y << "," << C.z << ")" << std::endl;
	os << "    and radius: " << R << std::endl;
}

} // -- namespace geom
} // -- namespace sim
