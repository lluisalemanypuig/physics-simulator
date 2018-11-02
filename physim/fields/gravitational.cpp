#include <physim/fields/gravitational.hpp>

// physim includes
#include <physim/math/math.hpp>

#include <iostream>
using namespace std;

namespace physim {
namespace fields {

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
	// unit directional vector
	math::vec3 v;
	__pm_sub_v_v(v, pos, p->cur_pos);
	math::normalise(v, v);

	__pm_mul_v_s(F, v, p->mass*M);
}

} // -- namespace fields
} // -- namespace physim
