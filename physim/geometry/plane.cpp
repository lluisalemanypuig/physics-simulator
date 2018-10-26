#include <physim/geometry/plane.hpp>

namespace physim {
namespace geom {

// PRIVATE

// PUBLIC

plane::plane() : geometry() {
	__pm_assign_c(normal, 0.0f,0.0f,0.0f);
	dconst = 0.0f;
}

plane::plane(const math::vec3& n, const math::vec3& p) : geometry() {
	/*normal = glm::normalize(n);
	dconst = -glm::dot(p, normal);*/

	__pm_normalise(normal,n);
	dconst = -__pm_dot(p, normal);
}

plane::plane(const math::vec3& n, float d) : geometry() {
	/*normal = glm::normalize(n);
	dconst = d;*/

	__pm_normalise(normal,n);
	dconst = d;
}

plane::plane(const math::vec3& p0, const math::vec3& p1, const math::vec3& p2) : geometry() {
	/*vec3 v1 = p1 - p0;
	vec3 v2 = p2 - p0;
	normal = glm::normalize(glm::cross(v1, v2));
	dconst = -glm::dot(p0, normal);*/

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
	// dconst = -glm::dot(p, normal);

	dconst = -__pm_dot(p, normal);
}

geom_type plane::get_geom_type() const {
	return geom_type::Plane;
}

float plane::dist_point_plane(const math::vec3& p) const {
	//return glm::dot(p, normal) + dconst;
	return __pm_dot(p, normal) + dconst;
}

void plane::closest_point_plane(const math::vec3& p, math::vec3& closest) const {
	/*float r = (-dconst - glm::dot(p, normal));
	return p + r*normal;*/

	float r = -dconst - __pm_dot(p, normal);
	__pm_mul_v_s(closest, normal, r);
	__pm_add_v_v(closest, closest, p);
}

const math::vec3& plane::get_normal() const {
	return normal;
}

float plane::get_constant() const {
	return dconst;
}

// GETTERS

bool plane::is_inside(const math::vec3& p, float tol) const {
	//float dist = glm::dot(p, normal) + dconst;
	float dist = __pm_dot(p, normal) + dconst;

	if (dist <= tol) {
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

	/*float r = (-dconst - glm::dot(p1, normal))/glm::dot((p2 - p1), normal);
	p_inter = (1 - r)*p1 + r*p2;
	return true;*/

	// Use point of intersection temporarily.
	// It will be overwritten later.
	__pm_sub_v_v(p_inter, p2, p1);
	float r = (-dconst - __pm_dot(p1, normal))/__pm_dot(p_inter, normal);
	__pm_add_vs_vs(p_inter, p1,(1.0f - r), p2,r);
	return true;
}

// OTHERS

void plane::update_upon_collision
(const math::vec3& pred_pos, const math::vec3& pred_vel, particle *p)
const
{
	/*p->save_position();

	// Wn is a vector normal to the plane with
	// direction towards the intersection point
	// (this point is not needed to calculate Wn)
	vec3 Wn = (glm::dot(pred_pos, normal) + dconst)*normal;

	// --- update position --- (with bouncing coefficient)

	float bounce = p->get_bouncing();
	p->set_position( pred_pos - (1.0f + bounce)*Wn );

	// --- update velocity (1) --- (with bouncing coefficient)

	// We need the velocity at time T, not the previous
	// velocity (time T - dt). A constant reference is
	// not used because we need to keep this value after update.
	vec3 vt = p->get_velocity();

	// first update of the velociy (with bouncing)
	float nv_dot = glm::dot(normal, pred_vel);
	p->set_velocity( pred_vel - (1.0f + bounce)*(nv_dot*normal) );

	// --- update velocity (2) --- (with friction coefficient)

	// Use 'vt', the velocity at time t
	vec3 vT = vt - glm::dot(normal,vt)*normal;
	p->set_velocity( p->get_velocity() - p->get_friction()*vT );*/



	p->save_position();
	math::vec3 temp;

	// Wn is a vector normal to the plane with
	// direction towards the intersection point
	// (this point is not needed to calculate Wn)

	math::vec3 Wn;
	__pm_mul_v_s(Wn, normal, (__pm_dot(pred_pos,normal) + dconst));

	// --- update position --- (with bouncing coefficient)

	float bounce = p->get_bouncing();
	__pm_mul_v_s(temp, Wn, (1.0f + bounce));
	__pm_sub_v_v(p->get_position(), pred_pos, temp);

	// --- update velocity (1) --- (with bouncing coefficient)

	// We need the velocity at time T, not the previous
	// velocity (time T - dt). A constant reference is
	// not used because we need to keep this value after update.
	math::vec3 vt;
	__pm_assign_v(vt, p->get_velocity());

	// first update of the velociy (with bouncing)
	float nv_dot = __pm_dot(normal, pred_vel);
	__pm_mul_v_s(temp, normal, ((1.0f + bounce)*nv_dot));
	__pm_sub_v_v(p->get_velocity(), pred_vel, temp);

	// --- update velocity (2) --- (with friction coefficient)

	// Use 'vt', the velocity at time t
	math::vec3 vT;
	__pm_mul_v_s(vT, normal, __pm_dot(normal,vt));
	__pm_sub_v_v(vT, vt, vT);
	__pm_mul_v_s(vT, vT, p->get_friction());
	__pm_sub_v_v(p->get_velocity(), p->get_velocity(), vT);
}

void plane::display(ostream& os) const {
	os << "I am a plane" << endl;
	os << "    with plane equation:" << endl;
	os << "        " << normal.x() << "*x + "
					 << normal.y() << "*y + "
					 << normal.z() << "*z + "
					 << dconst << " = 0" << endl;
}

} // -- namespace geom
} // -- namespace sim
