#include <physim/fields/gravitational.hpp>

// physim includes
#include <physim/math/math_private.hpp>

namespace physim {
namespace fields {

// PRIVATE

template<class P>
void gravitational::__compute_force(const P *p, math::vec3& F) {
	// unit directional vector
	math::vec3 v;
	__pm_sub_v_v(v, pos, p->cur_pos);
	math::normalise(v, v);

	__pm_mul_v_s(F, v, p->mass*M);
}

// PUBLIC

gravitational::gravitational() : punctual() {
	M = 0.0f;
}

gravitational::gravitational(const math::vec3& pos, float _M)
	: punctual(pos)
{
	M = _M*6.674e-11;
}

gravitational::gravitational(const gravitational& f)
	: punctual(f)
{
	M = f.M;
}

gravitational::~gravitational() {
}

// SETTERS

void gravitational::set_mass(float _M) {
	M = _M*6.674e-11;
}

// GETTERS

float gravitational::get_mass() const {
	return M;
}

// OTHERS

void gravitational::compute_force(const particles::free_particle *p, math::vec3& F) {
	__compute_force(p, F);
}

void gravitational::compute_force(const particles::mesh_particle *p, math::vec3& F) {
	__compute_force(p, F);
}

} // -- namespace fields
} // -- namespace physim
