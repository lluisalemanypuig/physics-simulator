#include <physim/initialiser/rect_shower.hpp>

namespace physim {
namespace init {

// PROTECTED

void rect_shower::make_vel_init() {
	vel = [](particle *p) {
		__pm_assign_c(p->get_velocity(), 0.0f,0.0f,0.0f);
	};
}

// PUBLIC

rect_shower::rect_shower() : rect_source() { }

rect_shower::rect_shower(const rect_shower& w) : rect_source(w) {

}

rect_shower::~rect_shower() { }

// GETTERS

initialiser *rect_shower::clone() const {
	return new rect_shower(*this);
}

} // -- namespace init
} // -- namespace physim
