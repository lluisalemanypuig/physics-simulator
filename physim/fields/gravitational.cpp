#include <physim/fields/gravitational.hpp>

// physim includes
#include <physim/math/private/math3.hpp>

namespace physim {
using namespace particles;
using namespace math;

namespace fields {

// PRIVATE

template<class P>
void gravitational::__compute_force(const P& p, vec3& F) {
	// unit directional vector
	vec3 v;
	__pm3_sub_v_v(v, pos, p.cur_pos);
	normalise(v, v);

	__pm3_mul_v_s(F, v, p.mass*M);
}

// PUBLIC

gravitational::gravitational() : punctual() {
	M = 0.0f;
}

gravitational::gravitational(const vec3& pos, float _M)
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

void gravitational::compute_force(const free_particle& p, vec3& F) {
	__compute_force(p, F);
}

void gravitational::compute_force(const mesh_particle& p, vec3& F) {
	__compute_force(p, F);
}

void gravitational::compute_force(const fluid_particle& p, vec3& F) {
	__compute_force(p, F);
}

} // -- namespace fields
} // -- namespace physim
