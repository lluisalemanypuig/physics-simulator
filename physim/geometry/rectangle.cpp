#include <physim/geometry/rectangle.hpp>

// C includes
#include <assert.h>

// C++ includes
#include <iostream>
using namespace std;

// physim includes
#include <physim/math/private/math3.hpp>

// LOCAL-DEFINED

namespace physim {
using namespace math;
using namespace particles;

namespace geometry {

// PRIVATE

// PUBLIC

rectangle::rectangle() : geometry() { }

rectangle::rectangle
(const vec3& p1,const vec3& p2, const vec3& p3,const vec3& p4)
: geometry(), pl(plane(p1,p2,p3))
{
	__pm3_assign_v(v1, p1);
	__pm3_assign_v(v2, p2);
	__pm3_assign_v(v3, p3);
	__pm3_assign_v(v4, p4);

	// make sure that last vertex is on plane...
	assert(pl.is_inside(v4));

	__pm3_min4(vmin, v1,v2,v3,v4);
	__pm3_max4(vmax, v1,v2,v3,v4);
	__pm3_sub_acc_s(vmin, 0.01f);
	__pm3_add_acc_s(vmax, 0.01f);
}

rectangle::rectangle(const rectangle& r) : geometry(r) {
	__pm3_assign_v(v1, r.v1);
	__pm3_assign_v(v2, r.v2);
	__pm3_assign_v(v3, r.v3);
	__pm3_assign_v(v4, r.v4);
}

rectangle::~rectangle() { }

// SETTERS

void rectangle::set_position(const vec3& v) {
	__pm3_add_acc_v(v1, v);
	__pm3_add_acc_v(v2, v);
	__pm3_add_acc_v(v3, v);
	__pm3_add_acc_v(v4, v);

	__pm3_add_acc_v(vmin, v);
	__pm3_add_acc_v(vmax, v);
	__pm3_sub_acc_s(vmin, 0.01f);
	__pm3_add_acc_s(vmax, 0.01f);
}

// GETTERS

const plane& rectangle::get_plane() const {
	return pl;
}

bool rectangle::is_inside(const vec3& p, float tol) const {
	// check if the point is inside the bounding box
	if (not __pm3_inside_box(p, vmin,vmax)) {
		return false;
	}

	// If the point is inside the bounding box then it
	// might be 'inside' the associated plane.

	if (not pl.is_inside(p, tol)) {
		return false;
	}

	// If the point is on the associated plane
	// and 'inside' the plane then the point
	// is 'inside' the rectangle.
	return true;
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
(const vec3& p1, const vec3& p2, vec3& p_inter) const
{
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

bool rectangle::intersec_sphere(const vec3& c, float R) const {
	cerr << "rectangle::intersec_sphere (" << __LINE__ << ") - Error" << endl;
	cerr << "    Not implemented yet" << endl;
	return false;
}

// OTHERS

void rectangle::update_particle
(const vec3& pred_pos, const vec3& pred_vel, free_particle *p)
const
{
	pl.update_particle(pred_pos, pred_vel, p);
}

void rectangle::correct_position(
	const vec3& pred_pos, const sized_particle *p,
	vec3& correct_position
) const
{

}

void rectangle::update_particle
(const vec3& pred_pos, const vec3& pred_vel, sized_particle *p)
const
{
	cerr << "rectangle::update_particle (" << __LINE__ << ") - Error" << endl;
	cerr << "    Not implemented yet" << endl;
}

void rectangle::display() const {
	cout << "I am a rectangle" << endl;
	cout << "    with vertices:" << endl;
	cout << "        - Point({" << v1.x << "," << v1.y << "," << v1.z << "})" << endl;
	cout << "        - Point({" << v2.x << "," << v2.y << "," << v2.z << "})" << endl;
	cout << "        - Point({" << v3.x << "," << v3.y << "," << v3.z << "})" << endl;
	cout << "        - Point({" << v4.x << "," << v4.y << "," << v4.z << "})" << endl;
	cout << "    and plane equation:" << endl;
	const vec3& n = pl.get_normal();
	cout << "        " << n.x << "*x + " << n.y << "*y + " << n.z << "*z + "
		 << pl.get_constant() << " = 0" << endl;
}

} // -- namespace geom
} // -- namespace sim
