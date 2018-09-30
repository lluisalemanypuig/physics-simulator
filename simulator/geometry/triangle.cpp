#include <simulator/geometry/triangle.hpp>

#include <iostream>
using namespace std;

namespace sim {
namespace geom {

// LOCAL-DEFINED

inline
float triangle_area(const vec3& p1, const vec3& p2, const vec3& p3) {
	vec3 vivj = p2 - p1;
	vec3 vivk = p3 - p1;
	return glm::length(glm::cross(vivj, vivk))/2.0f;
}

// PRIVATE

// PUBLIC

triangle::triangle() { }

triangle::triangle(const vec3& p1,const vec3& p2,const vec3& p3) {
	v1 = p1;
	v2 = p2;
	v3 = p3;
	pl = plane(v1, v2, v3);
}

triangle::~triangle() { }

void triangle::display() const {
	cout << "I am a triangle" << endl;
	cout << "    with vertices:" << endl;
	cout << "        - Point({" << v1.x << "," << v1.y << "," << v1.z << "})" << endl;
	cout << "        - Point({" << v2.x << "," << v2.y << "," << v2.z << "})" << endl;
	cout << "        - Point({" << v3.x << "," << v3.y << "," << v3.z << "})" << endl;
	cout << "    and plane equation:" << endl;
	const vec3& n = pl.get_normal();
	cout << "        " << n.x << "*x + " << n.y << "*y + " << n.z << "*z + "
		 << pl.get_constant() << " = 0" << endl;
}

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
	cout << "Is point inside triangle?" << endl;
	cout << "Point({" << p.x << "," << p.y << "," << p.z << "})" << endl;

	float a1 = triangle_area(p,  v2, v3);
	float a2 = triangle_area(v1,  p, v3);
	float a3 = triangle_area(v1, v2,  p);
	float A  = triangle_area(v1, v2, v3);

	cout << "    a1= " << a1 << endl;
	cout << "    a2= " << a2 << endl;
	cout << "    a3= " << a3 << endl;
	cout << "    A=  " << A << endl;
	cout << "    a1 + a2 + a3 = " << a1 + a2 + a3 << endl;
	cout << "    a1 + a2 + a3 - A = " << a1 + a2 + a3 - A << endl;

	if ((a1 + a2 + a3 - A) <= tol) {
		return true;
	}
	return false;
}

geom_type triangle::get_geom_type() const {
	return geom_type::Triangle;
}

bool triangle::intersec_segment(const vec3& p1, const vec3& p2) const {
	cout << "Testing intersection with segment and triangle" << endl;

	vec3 intersection;
	return intersec_segment(p1, p2, intersection);
}

bool triangle::intersec_segment(const vec3& p1, const vec3& p2, vec3& p_inter) const {
	cout << "Testing intersection with segment and triangle (while computing the intersection point)!" << endl;

	// if the segment does not intersect the plane
	// surely it does not intersect the triangle
	if (not pl.intersec_segment(p1,p2, p_inter)) {
		cout << "The segment does not intersect the plane!" << endl;
		return false;
	}

	cout << "Intersection point:" << endl;
	cout << "Point({" << p_inter.x << "," << p_inter.y << "," << p_inter.z << "})" << endl;

	// intersection has been computed -> if this point
	// lies inside the triangle then there is intersection
	if (is_inside(p_inter)) {
		return true;
	}
	cout << "The intersection point is not inside the triangle!" << endl;
	return false;
}

// OTHERS

void triangle::update_upon_collision(particle *p) const {
	pl.update_upon_collision(p);
}

} // -- namespace geom
} // -- namespace sim
