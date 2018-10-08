#include <simulator/initialiser/rect_waterfall.hpp>

#include <iostream>

namespace physim {
namespace init {

// PROTECTED

void rect_waterfall::make_vel_init() {
	vel = [this](particle *p) {
		const float r = this->U01(this->E);
		p->set_previous_velocity(vec3(0.0f,0.0f,0.0f));
		p->set_velocity(vec3(0.0f, -r*10.0f, 0.0f));
	};
}

// PUBLIC

rect_waterfall::rect_waterfall() : rect_source() { }

rect_waterfall::rect_waterfall(const rect_waterfall& w) : rect_source(w) {

}

rect_waterfall::~rect_waterfall() { }

} // -- namespace init
} // -- namespace physim
