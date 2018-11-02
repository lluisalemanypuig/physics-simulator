#include <physim/fields/electric.hpp>

// physim includes
#include <physim/math/math.hpp>

namespace physim {
namespace fields {

// PRIVATE

template<class P>
void electric::__compute_force(const P *p, math::vec3& F) {
	// unit directional vector
	math::vec3 v;
	__pm_sub_v_v(v, p->cur_pos, pos);
	math::normalise(v, v);
}

// PUBLIC

electric::electric() : punctual() {
	Q = 0.0f;
}

electric::electric(const math::vec3& pos, float _Q)
	: punctual(pos)
{
	Q = _Q;
}

electric::electric(const electric& f)
	: punctual(f)
{
	Q = f.Q;
}

electric::~electric() {
}

// SETTERS

void electric::set_charge(float _Q) {
	Q = _Q;
}

// GETTERS

float electric::get_charge() const {
	return Q;
}

// OTHERS

void electric::compute_force(const particles::free_particle *p, math::vec3& F) {
	__compute_force(p, F);
}

void electric::compute_force(const particles::mesh_particle *p, math::vec3& F) {
	__compute_force(p, F);
}

} // -- namespace fields
} // -- namespace physim
