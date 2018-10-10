#include <simulator/geometry/sphere.hpp>

namespace physim {
namespace geom {

// PRIVATE

// PUBLIC

sphere::sphere() : geometry() { }

sphere::sphere(const vec3& c, float r) : geometry() {
	C = c;
	R = r;
}

sphere::sphere(const sphere& s) : geometry(s) {
	C = s.C;
	R = s.R;
}

sphere::~sphere() { }

// SETTERS

void sphere::set_position(const vec3& p) {
	C = p;
}

// GETTERS

void sphere::set_radius(float r) {
	R = r;
}

// GETTERS

const vec3& sphere::get_centre() const {
	return C;
}

float sphere::get_radius() const {
	return R;
}

bool sphere::is_inside(const vec3& p, float tol) const {
	// compute squared distance from centre to p
	float dx = (C.x - p.x)*(C.x - p.x);
	float dy = (C.y - p.y)*(C.y - p.y);
	float dz = (C.z - p.z)*(C.z - p.z);
	return (dx + dy + dz - R*R <= tol);
}

geom_type sphere::get_geom_type() const {
	return geom_type::Sphere;
}

bool sphere::intersec_segment(const vec3& p1, const vec3& p2) const {
	bool p1_in = is_inside(p1);
	bool p2_in = is_inside(p2);
	return (p1_in and not p2_in) or (not p1_in and p2_in);
}

bool sphere::intersec_segment(const vec3& p, const vec3& q, vec3& p_inter) const {
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

	// coefficients of quadratic equation
	const vec3 v = q - p;
	const float a = glm::dot(v,v);
	const float b = 2.0f*glm::dot(p - C,v);
	const float c = glm::dot(C,C) + glm::dot(p,p) - 2.0f*glm::dot(p,C) - R*R;

	// discriminant of the quadratic equation
	const float discr = b*b - 4.0f*a*c;

	// make sure the discriminant is positive
	// to be safe that we can compute the square root
	assert(discr >= 0.0f);

	float L = (-b + sqrt(discr))/(2.0f*a);

	if (L < 0.0f or 1.0f < L) {
		L = (-b - sqrt(discr))/(2.0f*a);
	}

	// make sure that L has the right value
	assert(0.0f <= L and L <= 1.0f);

	// compute intersection point
	p_inter = (1 - L)*p + L*q;
	return true;
}

// OTHERS

void sphere::update_upon_collision(const vec3& pred_pos, const vec3& pred_vel, particle *p) const {
	// define a plane tangent to the sphere
	// that goes through the intersection point

	// compute intersection point
	const vec3& pos = p->get_position();

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
}

void sphere::display(ostream& os) const {
	os << *this;
}

} // -- namespace geom
} // -- namespace sim
