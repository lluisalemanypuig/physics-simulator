#include <physim/geometry/sphere.hpp>

namespace physim {
namespace geom {

// PRIVATE

// PUBLIC

sphere::sphere() : geometry() { }

sphere::sphere(const math::vec3& c, float r) : geometry() {
	//C = c;

	__pm_assign_v(C, c);
	R = r;
}

sphere::sphere(const sphere& s) : geometry(s) {
	//C = s.C;

	__pm_assign_v(C, s.C);
	R = s.R;
}

sphere::~sphere() { }

// SETTERS

void sphere::set_position(const math::vec3& p) {
	//C = p;
	__pm_assign_v(C, p);
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
	/*float dx = (C.x - p.x)*(C.x - p.x);
	float dy = (C.y - p.y)*(C.y - p.y);
	float dz = (C.z - p.z)*(C.z - p.z);
	return (dx + dy + dz - R*R <= tol);*/
	return ((__pm_dist2(C,p)) - R*R) <= tol;
}

geom_type sphere::get_geom_type() const {
	return geom_type::Sphere;
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
	 *          I = (1-L)*p + L*q, L in [0,1]
	 * such that
	 *     (I - C)**(I - C) - r = 0,
	 * where ** represents the dot product
	 *
	 * Upon arithmetic manipulations we obtain that
	 * we have to solve the following quadratic equation
	 * for L:
	 *   a*L^2 + b*L + c = 0
	 *
	 * where
	 *	a = v**v
	 *	b = 2( v**(p - C) )
	 *	c = C**C + p**p - 2(p**C) - r*r
	 *
	 */

	/*
	// coefficients of quadratic equation
	vec3 v = q - p;
	float a = glm::dot(v,v);
	float b = 2.0f*glm::dot(p - C,v);
	float c = glm::dot(C,C) + glm::dot(p,p) - 2.0f*glm::dot(p,C) - R*R;

	// discriminant of the quadratic equation
	float discr = b*b - 4.0f*a*c;

	// make sure the discriminant is positive
	// to be safe that we can compute the square root
	assert(discr >= 0.0f);

	// compute both solutions of the equation and
	// take the one that is closest to the interval [0,1]
	float Lp = (-b + sqrt(discr))/(2.0f*a);
	float Lm = (-b - sqrt(discr))/(2.0f*a);

	float dp = 0.0f;
	if (Lp < 0.0f) {
		dp = abs(Lp);
	}
	else if (Lp > 1.0f) {
		dp = Lp - 1.0f;
	}
	float dm = 0.0f;
	if (Lm < 0.0f) {
		dm = abs(Lm);
	}
	else if (Lm > 1.0f) {
		dm = Lm - 1.0f;
	}
	float L = (dp < dm ? Lp : Lm);

	#if defined (DEBUG)
	// make sure that L has the right value (with some tolerance)
	if (not (-0.00009f <= L and L <= 1.00009f)) {
		cerr << "sphere::intersec_segment: Error:" << endl;
		cerr << "    Value of L to determine intersection point is not valid." << endl;
		cerr << "    L= " << L << endl;
		cerr << "    Between the two solutions:" << endl;
		cerr << "        Lp= " << Lp << ", dp= " << dp << endl;
		cerr << "        Lm= " << Lm << ", dm= " << dm << endl;
		assert( false );
	}
	#endif

	// compute intersection point
	p_inter = (1 - L)*p + L*q;
	return true;
	*/

	// coefficients of quadratic equation
	math::vec3 p_minus_C;
	__pm_sub_v_v(p_minus_C, p, C);

	math::vec3 v;
	__pm_sub_v_v(v, q, p);

	float a = (__pm_dot(v,v));
	float b = 2.0f*(__pm_dot(p_minus_C,v));
	float c = (__pm_dot(C,C)) + (__pm_dot(p,p)) - 2.0f*(__pm_dot(p,C)) - R*R;

	// discriminant of the quadratic equation
	float discr = b*b - 4.0f*a*c;

	// make sure the discriminant is positive
	// to be safe that we can compute the square root
	assert(discr >= 0.0f);

	// compute both solutions of the equation and
	// take the one that is closest to the interval [0,1]
	float Lp = (-b + std::sqrt(discr))/(2.0f*a);
	float Lm = (-b - std::sqrt(discr))/(2.0f*a);

	float dp = 0.0f;
	if (Lp < 0.0f) {
		dp = abs(Lp);
	}
	else if (Lp > 1.0f) {
		dp = Lp - 1.0f;
	}
	float dm = 0.0f;
	if (Lm < 0.0f) {
		dm = abs(Lm);
	}
	else if (Lm > 1.0f) {
		dm = Lm - 1.0f;
	}
	float L = (dp < dm ? Lp : Lm);

	#if defined (DEBUG)
	// make sure that L has the right value (with some tolerance)
	if (not (-0.00009f <= L and L <= 1.00009f)) {
		cerr << "sphere::intersec_segment: Error:" << endl;
		cerr << "    Value of L to determine intersection point is not valid." << endl;
		cerr << "    L= " << L << endl;
		cerr << "    Between the two solutions:" << endl;
		cerr << "        Lp= " << Lp << ", dp= " << dp << endl;
		cerr << "        Lm= " << Lm << ", dm= " << dm << endl;
		assert( false );
	}
	#endif

	// compute intersection point
	__pm_add_vs_vs(p_inter, p,(1.0f - L), q,L);

	return true;
}

// OTHERS

void sphere::update_upon_collision(const math::vec3& pred_pos, const math::vec3& pred_vel, particle *p) const {
	// define a plane tangent to the sphere
	// that goes through the intersection point

	/*
	// compute intersection point
	const math::vec3& pos = p->get_position();

	vec3 I;
	bool r = intersec_segment(pos, pred_pos, I);
	if (not r) {
		return;
	}

	// define the plane
	vec3 normal = C - I;
	plane tan_plane(normal,I);

	// tell the plane to update the particle
	tan_plane.update_upon_collision(pred_pos, pred_vel, p);
	*/

	// compute intersection point

	math::vec3 I;
	bool r = intersec_segment(p->get_position(), pred_pos, I);
	if (not r) {
		return;
	}

	// define the plane
	math::vec3 normal;
	__pm_sub_v_v(normal, C, I);
	plane tan_plane(normal,I);

	// tell the plane to update the particle
	tan_plane.update_upon_collision(pred_pos, pred_vel, p);
}

void sphere::display(ostream& os) const {
	os << "I am a sphere" << endl;
	os << "    with centre: (" << C.x() << "," << C.y() << "," << C.z() << ")" << endl;
	os << "    and radius: " << R << endl;
}

} // -- namespace geom
} // -- namespace sim
