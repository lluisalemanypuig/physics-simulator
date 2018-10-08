#include <simulator/initialiser/rect_fountain.hpp>

namespace physim {
namespace init {

// PROTECTED

void rect_fountain::make_vel_init() {
	vel = [this](particle *p) {
		p->set_previous_velocity(vec3(0.0f,0.0f,0.0f));

		const float r = this->U01(this->E);
		const vec3& pos = p->get_position();

		// how far is pos from the middle (squared)
		const float mx = this->x + w;
		const float mz = this->y + h;
		const float dx = pos.x - mx;
		const float dz = pos.z - mz;
		const float dm2 = dx*dx + dz*dz;

		// the closer to the 'x' edge the slower the x velocity
		const float vx = (w/2.0f)/dx + r*2.0f;
		// the closer to the middle the faster they go 'up'
		const float vy = 10.0f/dm2;
		// the closer to the 'z' edge the slower the z velocity
		const float vz = (h/2.0f)/dz + r*2.0f;

		p->set_velocity(vec3(vx,vy,vz));
	};
}

// PUBLIC

rect_fountain::rect_fountain() : rect_source() { }

rect_fountain::rect_fountain(const rect_fountain& f) : rect_source(f) {

}

rect_fountain::~rect_fountain() { }

} // -- namespace init
} // -- namespace physim
