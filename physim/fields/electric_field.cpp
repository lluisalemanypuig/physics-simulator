#include <physim/fields/electric_field.hpp>

// physim includes
#include <physim/math/math.hpp>

namespace physim {
namespace fields {

electric_field::electric_field() : punctual_field() {
	Q = 0.0f;
}

electric_field::electric_field(const math::vec3& pos, float _Q)
	: punctual_field(pos)
{
	Q = _Q;
}

electric_field::electric_field(const electric_field& f)
	: punctual_field(f)
{
	Q = f.Q;
}

electric_field::~electric_field() {
}

// SETTERS

void electric_field::set_charge(float _Q) {
	Q = _Q;
}

// GETTERS

float electric_field::get_charge() const {
	return Q;
}

// OTHERS

void electric_field::compute_force(const particle *p, math::vec3& F) {
	// unit directional vector
	math::vec3 v;
	__pm_sub_v_v(v, p->get_position(), pos);
	math::normalise(v, v);


}

} // -- namespace fields
} // -- namespace physim
