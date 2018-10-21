#include <physim/geometry/triangle.hpp>

inline
float triangle_area(const vec3& p1, const vec3& p2, const vec3& p3) {
	vec3 vivj = p2 - p1;
	vec3 vivk = p3 - p1;
	return glm::length(glm::cross(vivj, vivk))/2.0f;
}

namespace physim {
namespace geom {

// LOCAL-DEFINED

// PRIVATE

// PUBLIC

triangle::triangle() : geometry() { }

triangle::triangle(const vec3& p1,const vec3& p2,const vec3& p3) : geometry() {
	v1 = p1;
	v2 = p2;
	v3 = p3;
	pl = plane(v1, v2, v3);
}

triangle::triangle(const triangle& t) : geometry(t) {
	v1 = t.v1;
	v2 = t.v2;
	v3 = t.v3;
	pl = t.pl;
}

triangle::~triangle() { }

// SETTERS

void triangle::set_position(const vec3& v) {
	v1 += v;
	v2 += v;
	v3 += v;
	pl.set_position(v1);
}

// GETTERS

const plane& triangle::get_plane() const {
	return pl;
}

bool triangle::is_inside(const vec3& p, float tol) const {
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

geom_type triangle::get_geom_type() const {
	return geom_type::Triangle;
}

bool triangle::intersec_segment(const vec3& p1, const vec3& p2) const {
	vec3 intersection;
	return intersec_segment(p1, p2, intersection);
}

bool triangle::intersec_segment(const vec3& p1, const vec3& p2, vec3& p_inter) const {
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

// OTHERS

void triangle::update_upon_collision(const vec3& pred_pos, const vec3& pred_vel, particle *p) const {
	pl.update_upon_collision(pred_pos, pred_vel, p);
}

void triangle::display(ostream& os) const {
	os << *this;
}

} // -- namespace geom
} // -- namespace sim
