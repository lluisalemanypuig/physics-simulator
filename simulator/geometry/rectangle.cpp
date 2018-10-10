#include <simulator/geometry/rectangle.hpp>

// LOCAL-DEFINED

/* The 'glm::min/2' and 'glm::max/2' functions take the
 * minimum and maximum coordinates of the two vectors
 * respectively:
 *
 * vec3 v = glm::min( vec3(0,10,10), vec3(15,9,8) )
 * v --> vec3(0,9,8)
 *
 * vec3 v = glm::max( vec3(0,10,10), vec3(15,9,8) )
 * v --> vec3(15,10,10)
 */

template<typename T>
inline T min4(const T& a, const T& b, const T& c, const T& d) {
	return glm::min(glm::min(glm::min(a,b),c),d);
}

template<typename T>
inline T max4(const T& a, const T& b, const T& c, const T& d) {
	return glm::max(glm::max(glm::max(a,b),c),d);
}

namespace physim {
namespace geom {

// PRIVATE

// PUBLIC

rectangle::rectangle() : geometry() { }

rectangle::rectangle(const vec3& p1,const vec3& p2,const vec3& p3,const vec3& p4) : geometry() {
	v1 = p1;
	v2 = p2;
	v3 = p3;
	v4 = p4;
	pl = plane(v1, v2, v3);

	// make sure that last vertex is on plane...
	assert(pl.is_inside(v4));

	min = min4(v1,v2,v3,v4);
	max = max4(v1,v2,v3,v4);
}

rectangle::rectangle(const rectangle& r) : geometry(r) {
	v1 = r.v1;
	v2 = r.v2;
	v3 = r.v3;
	v4 = r.v4;
	pl = r.pl;

	min = r.min;
	max = r.max;
}

rectangle::~rectangle() { }

// SETTERS

void rectangle::set_position(const vec3& v) {
	v1 += v;
	v2 += v;
	v3 += v;
	v4 += v;
	pl.set_position(v1);

	min += v;
	max += v;
}

// GETTERS

const plane& rectangle::get_plane() const {
	return pl;
}

bool rectangle::is_inside(const vec3& p, float tol) const {
	// if the point is not inside the plane,
	// for sure it is not inside the rectangle
	if (not pl.is_inside(p, tol)) {
		return false;
	}

	// If the point is on the associated plane
	// then it only remains to check that is is
	// inside the bounding box of the rectangle.
	// If so then the point is 'inside' the rectangle.

	if (not (min.x <= p.x and p.x <= max.x)) {
		// outside x
		return false;
	}
	if (not (min.y <= p.y and p.y <= max.y)) {
		// outside y
		return false;
	}
	if (not (min.z <= p.z and p.z <= max.z)) {
		// outside z
		return false;
	}

	// inside box
	return true;
}

geom_type rectangle::get_geom_type() const {
	return geom_type::Rectangle;
}

bool rectangle::intersec_segment(const vec3& p1, const vec3& p2) const {
	vec3 intersection;
	return intersec_segment(p1, p2, intersection);
}

bool rectangle::intersec_segment(const vec3& p1, const vec3& p2, vec3& p_inter) const {
	// if the segment does not intersect the plane
	// surely it does not intersect the rectangle
	if (not pl.intersec_segment(p1,p2, p_inter)) {
		return false;
	}

	// intersection has been computed -> if this point
	// lies inside the rectangle then there is intersection
	if (is_inside(p_inter)) {
		return true;
	}
	return false;
}

// OTHERS

void rectangle::update_upon_collision(particle *p) const {
	pl.update_upon_collision(p);
}

void rectangle::display(ostream& os) const {
	os << *this;
}

} // -- namespace geom
} // -- namespace sim
