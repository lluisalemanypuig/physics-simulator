#include <physim/fields/punctual_field.hpp>

// physim includes
#include <physim/math/math.hpp>

namespace physim {
namespace fields {

punctual_field::punctual_field() {
	__pm_assign_s(pos, 0.0f);
}

punctual_field::punctual_field(const math::vec3& p) : field() {
	__pm_assign_v(pos, p);
}

punctual_field::punctual_field(const punctual_field& f) : field(f) {
	__pm_assign_v(pos, f.pos);
}

punctual_field::~punctual_field() {

}

// SETTERS

void punctual_field::set_position(const math::vec3& p) {
	__pm_assign_v(pos, p);
}

// GETTERS

const math::vec3& punctual_field::get_position() const {
	return pos;
}

} // -- namespace fields
} // -- namespace physim
