#include <physim/fields/gravitational_planet.hpp>

// physim includes
#include <physim/math/private/math3.hpp>

namespace physim {
namespace fields {

// PRIVATE

template<class P>
void gravitational_planet::__compute_force(const P *p, math::vec3& F) {
	__pm3_mul_v_s(F, pos, p->mass);
}

// PUBLIC

gravitational_planet::gravitational_planet() : punctual() {
}

gravitational_planet::gravitational_planet(const math::vec3& pos)
	: punctual(pos)
{
}

gravitational_planet::gravitational_planet(const gravitational_planet& f)
	: punctual(f)
{
}

gravitational_planet::~gravitational_planet() {
}

// OTHERS

void gravitational_planet::compute_force(const particles::free_particle *p, math::vec3& F) {
	__compute_force(p, F);
}

void gravitational_planet::compute_force(const particles::mesh_particle *p, math::vec3& F) {
	__compute_force(p, F);
}

} // -- namespace fields
} // -- namespace physim
