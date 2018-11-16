#include <physim/initialiser/rect_fountain.hpp>

// physim includes
#include <physim/particles/free_particle.hpp>
#include <physim/math/private/math3.hpp>

namespace physim {
namespace init {

// PROTECTED

void rect_fountain::make_vel_init() {
	vel = [this](particles::free_particle *p) {
		const float d2 = __pm3_dist2(p->cur_pos,this->C);
		const float D2 = (this->h*this->h + this->w*this->w)/4.0f;

		math::vec3 temp;
		__pm3_add_v_v(temp, p->cur_pos, this->n);
		__pm3_sub_v_v(temp, temp, this->C);
		__pm3_normalise(temp, temp);
		__pm3_mul_v_s(p->cur_vel, temp, (D2/d2));
	};
}

// PUBLIC

rect_fountain::rect_fountain() : rect_source() { }

rect_fountain::rect_fountain(const rect_fountain& f) : rect_source(f) {

	// For the same reasons explained in
	// rectangular_source::rectangular_source(const rectangular_source&)
	// make the function that initialises the function again.
	make_vel_init();
}

rect_fountain::~rect_fountain() { }

} // -- namespace init
} // -- namespace physim
