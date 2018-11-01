#include <physim/fields/gravitational_planet.hpp>

// physim includes
#include <physim/math/math.hpp>

namespace physim {
namespace fields {

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
	__pm_mul_v_s(F, pos, p->get_mass());
}

} // -- namespace fields
} // -- namespace physim
