#include <simulator/geometry/plane.hpp>

namespace sim {
namespace geom {

// PRIVATE

// PUBLIC

plane::plane() {
	normal = vec3(0.0f,0.0f,0.0f);
	dconst = 0.0f;
}

plane::plane(const vec3& n, const vec3& p) {
	normal = glm::normalize(n);
	dconst = -glm::dot(p, normal);
}

plane::plane(const vec3& p0, const vec3& p1, const vec3& p2) {
	vec3 v1 = p1 - p0;
	vec3 v2 = p2 - p0;
	normal = glm::normalize(glm::cross(v1, v2));
	dconst = -glm::dot(p0, normal);
}

plane::~plane() { }

// SETTERS

void plane::set_position(const vec3& p) {
	dconst = -glm::dot(p, normal);
}

geom_type plane::get_geom_type() const {
	return geom_type::Plane;
}

float plane::dist_point_plane(const vec3& p) const {
	return glm::dot(p, normal) + dconst;
}

vec3 plane::closest_point_plane(const vec3& p) const {
	float r = (-dconst - glm::dot(p, normal));
	return p + r*normal;
}

const vec3& plane::get_normal() const {
	return normal;
}

float plane::get_constant() const {
	return dconst;
}

// GETTERS

bool plane::is_inside(const vec3& p, float tol) const {
	float dist = glm::dot(p, normal) + dconst;
	if (dist <= tol) {
		return true;
	}
	return false;
}

bool plane::intersec_segment(const vec3& p1, const vec3& p2) const {
	float d1 = dist_point_plane(p1);
	float d2 = dist_point_plane(p2);
	return d1*d2 <= 0.0f;
}

bool plane::intersec_segment(const vec3& p1, const vec3& p2, vec3& p_inter) const {
	if (not intersec_segment(p1,p2)) {
		return false;
	}

	float r = (-dconst - glm::dot(p1, normal))/glm::dot((p2 - p1), normal);
	p_inter = (1 - r)*p1 + r*p2;
	return true;
}

// OTHERS

void plane::update_upon_collision(particle *p) const {
	// predicted position and velocity
	const vec3& pred_pos = p->get_current_position();
	const vec3& pred_vel = p->get_velocity();

	// Wn is a vector normal to the plane with
	// direction towards the intersection point
	// (this point is not needed to calculate Wn)
	vec3 Wn = (glm::dot(pred_pos, normal) + dconst)*normal;

	// update position (with bouncing coefficient)
	float bounce = p->get_bouncing();
	p->set_position( pred_pos - (1 + bounce)*Wn );

	// update velocity
	float nv_dot = glm::dot(normal, pred_vel);
	p->set_velocity( pred_vel - (1 + bounce)*(nv_dot*normal) );

	// update velocity (2 -> with friction coefficient)
	vec3 vt = pred_vel - glm::dot(normal,pred_vel)*normal;
	p->set_velocity( p->get_velocity() - p->get_friction()*vt );
}

void plane::display(ostream& os) const {
	os << "I am a plane" << endl;
	os << "    with plane equation:" << endl;
	const vec3& n = get_normal();
	os << "        " << n.x << "*x + " << n.y << "*y + " << n.z << "*z + "
	   << get_constant() << " = 0" << endl;
}

} // -- namespace geom
} // -- namespace sim
