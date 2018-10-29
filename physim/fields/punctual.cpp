#include <physim/fields/punctual.hpp>

// physim includes
#include <physim/math/math.hpp>

namespace physim {
namespace fields {

punctual::punctual() {
	__pm_assign_s(pos, 0.0f);
}

punctual::punctual(const math::vec3& p) : field() {
	__pm_assign_v(pos, p);
}

punctual::punctual(const punctual& f) : field(f) {
	__pm_assign_v(pos, f.pos);
}

punctual::~punctual() {

}

// SETTERS

void punctual::set_position(const math::vec3& p) {
	__pm_assign_v(pos, p);
}

// GETTERS

const math::vec3& punctual::get_position() const {
	return pos;
}

} // -- namespace fields
} // -- namespace physim
