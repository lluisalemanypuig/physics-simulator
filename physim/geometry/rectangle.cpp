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
(const vec3& _p0,const vec3& _p1, const vec3& _p2,const vec3& _p3)
: geometry(), pl(plane(_p0,_p1,_p2))
{
	__pm3_assign_v(p0, _p0);
	__pm3_assign_v(p1, _p1);
	__pm3_assign_v(p2, _p2);
	__pm3_assign_v(p3, _p3);

	// make sure that last vertex is on plane...
	assert(pl.is_inside(p3));

	__pm3_min4(vmin, p0,p1,p2,p3);
	__pm3_max4(vmax, p0,p1,p2,p3);
	__pm3_sub_acc_s(vmin, 0.01f);
	__pm3_add_acc_s(vmax, 0.01f);
}

rectangle::rectangle(const rectangle& r) : geometry(r) {
	__pm3_assign_v(p0, r.p0);
	__pm3_assign_v(p1, r.p1);
	__pm3_assign_v(p2, r.p2);
	__pm3_assign_v(p3, r.p3);
}

rectangle::~rectangle() { }

// SETTERS

void rectangle::set_position(const vec3& v) {
	__pm3_add_acc_v(p0, v);
	__pm3_add_acc_v(p1, v);
	__pm3_add_acc_v(p2, v);
	__pm3_add_acc_v(p3, v);

	__pm3_add_acc_v(vmin, v);
	__pm3_add_acc_v(vmax, v);
	__pm3_sub_acc_s(vmin, 0.01f);
	__pm3_add_acc_s(vmax, 0.01f);
}

// GETTERS

const plane& rectangle::get_plane() const {
	return pl;
}

void rectangle::get_points
(math::vec3& _p0, math::vec3& _p1, math::vec3& _p2, math::vec3& _p3)
const
{
	__pm3_assign_v(_p0, p0);
	__pm3_assign_v(_p1, p1);
	__pm3_assign_v(_p2, p2);
	__pm3_assign_v(_p3, p3);
}

void rectangle::projection(const math::vec3& p, math::vec3& proj) const {

}

float rectangle::distance(const math::vec3& p) const {
	return 0.0f;
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
(const vec3& _p0, const vec3& _p1, vec3& p_inter) const
{
	// if the segment does not intersect the plane
	// surely it does not intersect the rectangle
	if (not pl.intersec_segment(_p0,_p1, p_inter)) {
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
