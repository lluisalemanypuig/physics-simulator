#include <physim/fields/magnetic_B.hpp>

// physim includes
#include <physim/math/math.hpp>

namespace physim {
namespace fields {

magnetic_B::magnetic_B() : magnetic() {

}

magnetic_B::magnetic_B(const math::vec3& pos, const math::vec3& b)
	: magnetic(pos, b)
{

}

magnetic_B::magnetic_B(const magnetic_B& f)
	: magnetic(f)
{

}

magnetic_B::~magnetic_B() {

}

// OTHERS

void magnetic_B::compute_force(const particle* p, math::vec3& F) {
	math::vec3 temp;
	__pm_mul_v_s(temp, p->get_velocity(), p->get_charge());
	__pm_cross(F, temp, B);
}

} // -- namespace fields
} // -- namespace physim
