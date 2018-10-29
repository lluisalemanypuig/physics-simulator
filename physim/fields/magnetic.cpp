#include <physim/fields/magnetic.hpp>

// physim includes
#include <physim/math/math.hpp>

namespace physim {
namespace fields {

magnetic::magnetic() : punctual() {
	__pm_assign_s(B, 0.0f);
}

magnetic::magnetic(const math::vec3& p, const math::vec3& b)
	: punctual(p)
{
	__pm_assign_v(B, b);
}

magnetic::magnetic(const magnetic& f)
	: punctual(f)
{
	__pm_assign_v(B, f.B);
}

magnetic::~magnetic() {
}

// SETTERS

void magnetic::set_vector(const math::vec3& v) {
	__pm_assign_v(B, v);
}

// GETTERS

const math::vec3& magnetic::get_vector() const {
	return B;
}

} // -- namespace fields
} // -- namespace physim






