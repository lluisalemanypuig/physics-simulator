#include <physim/emitter/free_emitters/rect_shower.hpp>

// physim includes
#include <physim/particles/free_particle.hpp>
#include <physim/math/private/math3.hpp>

namespace physim {
using namespace particles;

namespace emitters {
namespace free_emitters {

// PROTECTED

void rect_shower::make_vel_init() {
	vel = [](base_particle& p) {
		__pm3_assign_s(p.cur_vel, 0.0f);
	};
}

// PUBLIC

rect_shower::rect_shower() : rect_source() { }

rect_shower::rect_shower(const rect_shower& w) : rect_source(w) {

}

rect_shower::~rect_shower() { }

// GETTERS

free_emitter *rect_shower::clone() const {
	return new rect_shower(*this);
}

} // -- namespace free_emitters
} // -- namespace emitters
} // -- namespace physim
