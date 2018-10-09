#include <simulator/initialiser/rect_fountain.hpp>

namespace physim {
namespace init {

// PROTECTED

void rect_fountain::make_vel_init() {
	vel = [this](particle *p) {
		const vec3& pos = p->get_position();

		float d2 = glm::dot(pos - this->C,pos - this->C);
		const float D2 = (this->h*this->h + this->w*this->w)/4.0f;

		vec3 V = pos + this->n;
		vec3 CV = (D2/d2)*glm::normalize(V - this->C);

		p->set_velocity( CV );
		p->set_previous_velocity(vec3(0.0f,0.0f,0.0f));
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
