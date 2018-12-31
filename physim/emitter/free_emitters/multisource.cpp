#include <physim/emitter/free_emitters/multisource.hpp>

namespace physim {
namespace emitters {
namespace free_emitters {

// PUBLIC

template<class T>
multisource<T>::multisource() : free_emitter() {
	P = 0;
	use_functions = 0;
}

template<class T>
multisource<T>::~multisource() {}

template<class T>
void multisource<T>::allocate(size_t p, size_t n) {
	P = p;
	sources.resize(n);
}

// MODIFIERS

template<class T>
void multisource<T>:: use_position_init(bool u) {
	if (u)	{ use_functions = use_functions | 0x0001; }
	else	{ use_functions = use_functions & (~0x0001); }
}

template<class T>
void multisource<T>:: use_velocity_init(bool u) {
	if (u)	{ use_functions = use_functions | 0x0002; }
	else	{ use_functions = use_functions & (~0x0002); }
}

template<class T>
void multisource<T>:: use_mass_init(bool u) {
	if (u)	{ use_functions = use_functions | 0x0004; }
	else	{ use_functions = use_functions & (~0x0004); }
}

template<class T>
void multisource<T>:: use_charge_init(bool u) {
	if (u)	{ use_functions = use_functions | 0x0008; }
	else	{ use_functions = use_functions & (~0x0008); }
}

template<class T>
void multisource<T>:: use_bounce_init(bool u) {
	if (u)	{ use_functions = use_functions | 0x0010; }
	else	{ use_functions = use_functions & (~0x0010); }
}

template<class T>
void multisource<T>:: use_friction_init(bool u) {
	if (u)	{ use_functions = use_functions | 0x0020; }
	else	{ use_functions = use_functions & (~0x0020); }
}

template<class T>
void multisource<T>:: use_lifetime_init(bool u) {
	if (u)	{ use_functions = use_functions | 0x0040; }
	else	{ use_functions = use_functions & (~0x0040); }
}

template<class T>
void multisource<T>:: use_starttime_init(bool u) {
	if (u)	{ use_functions = use_functions | 0x0080; }
	else	{ use_functions = use_functions & (~0x0080); }
}

template<class T>
void multisource<T>:: use_fixed_init(bool u) {
	if (u)	{ use_functions = use_functions | 0x0100; }
	else	{ use_functions = use_functions & (~0x0100); }
}

template<class T>
void multisource<T>::make_position_init() {
	pos = [this](particles::base_particle& p) {
		size_t c = P/sources.size();
		size_t k = p.index/c;
		if (k >= sources.size()) {
			k = sources.size() - 1;
		}
		sources[k].get_pos_initialiser()(p);
	};
}

template<class T>
void multisource<T>::make_velocity_init() {
	vel = [this](particles::base_particle& p) {
		size_t c = P/sources.size();
		size_t k = p.index/c;
		if (k >= sources.size()) {
			k = sources.size() - 1;
		}
		sources[k].get_vel_initialiser()(p);
	};
}

template<class T>
void multisource<T>::make_mass_init() {
	mass = [this](particles::base_particle& p) {
		size_t c = P/sources.size();
		size_t k = p.index/c;
		if (k >= this->sources.size()) {
			k = this->sources.size() - 1;
		}
		this->sources[k].get_mass_initialiser()(p);
	};
}

template<class T>
void multisource<T>::make_charge_init() {
	charge = [this](particles::free_particle& p) {
		size_t c = P/sources.size();
		size_t k = p.index/c;
		if (k >= sources.size()) {
			k = sources.size() - 1;
		}
		sources[k].get_charge_initialiser()(p);
	};
}

template<class T>
void multisource<T>::make_friction_init() {
	friction = [this](particles::free_particle& p) {
		size_t c = P/sources.size();
		size_t k = p.index/c;
		if (k >= sources.size()) {
			k = sources.size() - 1;
		}
		sources[k].get_friction_initialiser()(p);
	};
}

template<class T>
void multisource<T>::make_bounce_init() {
	bounce = [this](particles::free_particle& p) {
		size_t c = P/sources.size();
		size_t k = p.index/c;
		if (k >= sources.size()) {
			k = sources.size() - 1;
		}
		sources[k].get_bounce_initialiser()(p);
	};
}

template<class T>
void multisource<T>::make_lifetime_init() {
	lifetime = [this](particles::free_particle& p) {
		size_t c = P/sources.size();
		size_t k = p.index/c;
		if (k >= sources.size()) {
			k = sources.size() - 1;
		}
		sources[k].get_lifetime_initialiser()(p);
	};
}

template<class T>
void multisource<T>::make_starttime_init() {
	starttime = [this](particles::free_particle& p) {
		size_t c = P/sources.size();
		size_t k = p.index/c;
		if (k >= sources.size()) {
			k = sources.size() - 1;
		}
		sources[k].get_starttime_initialiser()(p);
	};
}

template<class T>
void multisource<T>::make_fixed_init() {
	fixed = [this](particles::free_particle& p) {
		size_t c = P/sources.size();
		size_t k = p.index/c;
		if (k >= sources.size()) {
			k = sources.size() - 1;
		}
		sources[k].get_fixed_initialiser()(p);
	};
}

template<class T>
void multisource<T>::make_all_init() {
	make_position_init();
	make_velocity_init();
	make_mass_init();
	make_charge_init();
	make_friction_init();
	make_bounce_init();
	make_lifetime_init();
	make_starttime_init();
	make_fixed_init();
}

// GETTERS

template<class T>
size_t multisource<T>::size() const {
	return sources.size();
}

template<class T>
std::vector<T>& multisource<T>::get_sources() {
	return sources;
}

template<class T>
const std::vector<T>& multisource<T>::get_sources() const {
	return sources;
}

// OTHERS

template<class T>
free_emitter *multisource<T>::clone() const {
	multisource<T> *ms= new multisource();

	ms->P = P;
	ms->use_functions = use_functions;
	ms->sources.resize(size());

	for (size_t i = 0; i < ms->sources.size(); ++i) {
		ms->sources[i] = sources[i];
	}

	if ((ms->use_functions & 0x0001) != 0) { ms->make_position_init(); }
	else { ms->pos = pos; }

	if ((ms->use_functions & 0x0002) != 0) { ms->make_velocity_init(); }
	else { ms->vel = vel; }

	if ((ms->use_functions & 0x0004) != 0) { ms->make_mass_init(); }
	else { ms->mass = mass; }

	if ((ms->use_functions & 0x0008) != 0) { ms->make_charge_init(); }
	else { ms->charge = charge; }

	if ((ms->use_functions & 0x0010) != 0) { ms->make_bounce_init(); }
	else { ms->bounce = bounce; }

	if ((ms->use_functions & 0x0020) != 0) { ms->make_friction_init(); }
	else { ms->friction = friction; }

	if ((ms->use_functions & 0x0040) != 0) { ms->make_lifetime_init(); }
	else { ms->lifetime = lifetime; }

	if ((ms->use_functions & 0x0080) != 0) { ms->make_starttime_init(); }
	else { ms->starttime = starttime; }

	if ((ms->use_functions & 0x0100) != 0) { ms->make_fixed_init(); }
	else { ms->fixed = fixed; }

	return ms;
}

} // -- namespace free_emitters
} // -- namespace emitters
} // -- namespace physim
