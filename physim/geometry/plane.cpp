#include <physim/geometry/plane.hpp>

// physim includes
#include <physim/math/math.hpp>

namespace physim {
namespace geom {

// PRIVATE

// PUBLIC

plane::plane() : geometry() {
	__pm_assign_c(normal, 0.0f,0.0f,0.0f);
	dconst = 0.0f;
}

plane::plane(const math::vec3& n, const math::vec3& p) : geometry() {
	__pm_normalise(normal,n);
	dconst = -__pm_dot(p, normal);
}

plane::plane(const math::vec3& n, float d) : geometry() {
	__pm_normalise(normal,n);
	dconst = d;
}

plane::plane(const math::vec3& p0, const math::vec3& p1, const math::vec3& p2) : geometry() {
	math::vec3 v1, v2;
	__pm_sub_v_v(v1, p1, p0);
	__pm_sub_v_v(v2, p2, p0);
	__pm_cross(normal, v1,v2);
	__pm_normalise(normal,normal);
	dconst = -__pm_dot(p0, normal);
}

plane::plane(const plane& p) : geometry(p) {
	__pm_assign_v(normal, p.normal);
	dconst = p.dconst;
}

plane::~plane() { }

// SETTERS

void plane::set_position(const math::vec3& p) {
	dconst = -__pm_dot(p, normal);
}

geom_type plane::get_geom_type() const {
	return geom_type::Plane;
}

float plane::dist_point_plane(const math::vec3& p) const {
	return __pm_dot(p, normal) + dconst;
}

void plane::closest_point_plane(const math::vec3& p, math::vec3& closest) const {
	__pm_add_v_vs(closest, p, normal,-dconst - __pm_dot(p, normal));
}

const math::vec3& plane::get_normal() const {
	return normal;
}

float plane::get_constant() const {
	return dconst;
}

// GETTERS

bool plane::is_inside(const math::vec3& p, float tol) const {
	if ((__pm_dot(p, normal) + dconst) <= tol) {
		return true;
	}
	return false;
}

bool plane::intersec_segment(const math::vec3& p1, const math::vec3& p2) const {
	float d1 = dist_point_plane(p1);
	float d2 = dist_point_plane(p2);
	return d1*d2 <= 0.0f;
}

bool plane::intersec_segment(const math::vec3& p1, const math::vec3& p2, math::vec3& p_inter) const {
	if (not intersec_segment(p1,p2)) {
		return false;
	}

	// Use point of intersection temporarily.
	// It will be overwritten later.
	__pm_sub_v_v(p_inter, p2, p1);
	float r = (-dconst - __pm_dot(p1, normal))/__pm_dot(p_inter, normal);
	__pm_add_vs_vs(p_inter, p1,(1.0f - r), p2,r);
	return true;
}

// OTHERS

void plane::update_particle
(const math::vec3& pred_pos, const math::vec3& pred_vel, particle *p)
const
{
	p->save_position();

	// Wn is a vector normal to the plane with
	// direction towards the intersection point
	// (this point is not needed to calculate Wn)

	math::vec3 Wn;
	__pm_mul_v_s(Wn, normal, __pm_dot(pred_pos,normal) + dconst);

	// --- update position --- (with bouncing coefficient)

	float bounce = p->get_bouncing();
	__pm_sub_v_vs(p->get_position(), pred_pos, Wn, 1.0f + bounce);

	// --- update velocity (1) --- (with bouncing coefficient)

	// We need the velocity at time T, not the previous
	// velocity (time T - dt). A constant reference is
	// not used because we need to keep this value after update.
	math::vec3 vt;
	__pm_assign_v(vt, p->get_velocity());

	// first update of the velociy (with bouncing)
	__pm_sub_v_vs(p->get_velocity(), pred_vel, normal,(1.0f + bounce)*__pm_dot(normal, pred_vel));

	// --- update velocity (2) --- (with friction coefficient)

	// Use 'vt', the velocity at time t
	math::vec3 vT;
	__pm_sub_v_vs(vT, vt, normal,__pm_dot(normal,vt));
	__pm_sub_acc_vs(p->get_velocity(), vT, p->get_friction());
}

void plane::display(std::ostream& os) const {
	os << "I am a plane" << std::endl;
	os << "    with plane equation:" << std::endl;
	os << "        " << normal.x << "*x + "
					 << normal.y << "*y + "
					 << normal.z << "*z + "
					 << dconst << " = 0" << std::endl;
}

} // -- namespace geom
} // -- namespace sim
