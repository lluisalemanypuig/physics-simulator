#include <physim/initialiser/rect_fountain.hpp>

// physim includes
#include <physim/particles/free_particle.hpp>
#include <physim/math/math.hpp>

namespace physim {
namespace init {

// PROTECTED

void rect_fountain::make_vel_init() {
	vel = [this](particles::free_particle *p) {
		const float d2 = __pm_dist2(p->get_position(),this->C);
		const float D2 = (this->h*this->h + this->w*this->w)/4.0f;

		math::vec3 temp;
		__pm_add_v_v(temp, p->get_position(), this->n);
		__pm_sub_v_v(temp, temp, this->C);
		__pm_normalise(temp, temp);
		__pm_mul_v_s(p->get_velocity(), temp, (D2/d2));
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
