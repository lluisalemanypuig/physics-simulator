#include <physim/fields/magnetic_b_field.hpp>

// physim includes
#include <physim/math/math.hpp>

namespace physim {
namespace fields {

magnetic_B_field::magnetic_B_field() : magnetic_field() {

}

magnetic_B_field::magnetic_B_field(const math::vec3& pos, const math::vec3& b)
	: magnetic_field(pos, b)
{

}

magnetic_B_field::magnetic_B_field(const magnetic_B_field& f)
	: magnetic_field(f)
{

}

magnetic_B_field::~magnetic_B_field() {

}

// OTHERS

void magnetic_B_field::compute_force(const particle* p, math::vec3& F) {
	math::vec3 temp;
	__pm_mul_v_s(temp, p->get_velocity(), p->get_charge());
	__pm_cross(F, temp, B);
}

} // -- namespace fields
} // -- namespace physim
