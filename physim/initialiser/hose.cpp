#include <physim/initialiser/hose.hpp>

// C includes
#include <assert.h>

// physim includes
#include <physim/particles/free_particle.hpp>
#include <physim/math/math.hpp>

namespace physim {
namespace init {

// PROTECTED

void hose::make_pos_init() {
	pos = [this](free_particle *p) {
		p->set_position(this->source);

		// copy the current position to the previous
		// position so that Verlet's solver works properly.
		p->save_position();
	};
}

void hose::make_vel_init() {
	vel = [this](free_particle *p) {
		const float x = (this->r)*this->U01(this->E);
		const float y = (this->r)*this->U01(this->E);
		const float phi = 2.0f*3.1415926535f*this->U01(this->E);

		math::vec3 base_point;
		__pm_add_vs_vs_v(
			base_point,
			this->v,(x*std::cos(phi)),
			this->w,(y*std::sin(phi)),
			this->cc
		);
		__pm_sub_v_v(p->get_velocity(), base_point, this->source);
	};
}

// PUBLIC

hose::hose() : initialiser() {
	std::random_device r;
	E = std::default_random_engine(r());
	U01 = std::uniform_real_distribution<float>(0.0f, 1.0f);
}

hose::hose(const hose& H) : initialiser(H) {
	E = H.E;
	U01 = H.U01;

	__pm_assign_v(source, H.source);
	__pm_assign_v(cc, H.cc);
	__pm_assign_v(v, H.v);
	__pm_assign_v(w, H.w);

	r = H.r;
	h = H.h;

	// The function that initialises the position in 'rs'
	// has references to its E and U01 objects. If this
	// function was copied then the copied function will
	// keep the references to rs's E and U01 objects, not
	// to *this's. Therefore, remake the position initialser
	// function. Since its random...
	make_pos_init();
	make_vel_init();
}

hose::~hose() { }

// SETTERS

void hose::set_hose_source(const math::vec3& S, const math::vec3& u, float _r,float _h) {
	assert( std::abs(__pm_norm(u) - 1.0f) <= 1e-06f );

	__pm_assign_v(source, S);
	__pm_perp(v, u);
	__pm_normalise(v,v);
	__pm_cross(w, u,v);

	__pm_add_v_vs(cc, source, u, _h);

	r = _r;
	h = _h;

	make_pos_init();
	make_vel_init();
}

// GETTERS

initialiser *hose::clone() const {
	return new hose(*this);
}

} // -- namespace init
} // -- namespace physim
