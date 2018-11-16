#include <physim/fields/magnetic_B.hpp>

// physim includes
#include <physim/math/private/math3.hpp>

namespace physim {
namespace fields {

// PRIVATE

template<class P>
void magnetic_B::__compute_force(const P *p, math::vec3& F) {
	math::vec3 temp;
	__pm3_mul_v_s(temp, p->cur_vel, p->charge);
	__pm3_cross(F, temp, B);
}

// PUBLIC

magnetic_B::magnetic_B() : magnetic() { }

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

void magnetic_B::compute_force(const particles::free_particle *p, math::vec3& F) {
	__compute_force(p, F);
}

void magnetic_B::compute_force(const particles::mesh_particle *p, math::vec3& F) {
	__compute_force(p, F);
}

} // -- namespace fields
} // -- namespace physim
