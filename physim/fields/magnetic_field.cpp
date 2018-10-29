#include <physim/fields/magnetic_field.hpp>

// physim includes
#include <physim/math/math.hpp>

namespace physim {
namespace fields {

magnetic_field::magnetic_field() : punctual_field() {
	__pm_assign_s(B, 0.0f);
}

magnetic_field::magnetic_field(const math::vec3& p, const math::vec3& b)
	: punctual_field(p)
{
	__pm_assign_v(B, b);
}

magnetic_field::magnetic_field(const magnetic_field& f)
	: punctual_field(f)
{
	__pm_assign_v(B, f.B);
}

magnetic_field::~magnetic_field() {
}

// SETTERS

void magnetic_field::set_vector(const math::vec3& v) {
	__pm_assign_v(B, v);
}

// GETTERS

const math::vec3& magnetic_field::get_vector() const {
	return B;
}

} // -- namespace fields
} // -- namespace physim






