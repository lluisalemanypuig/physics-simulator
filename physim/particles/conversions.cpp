#include <physim/particles/conversions.hpp>

// physim includes
#include <physim/math/private/math3.hpp>

namespace physim {
namespace particles {

void from_mesh_to_free(const mesh_particle& m, free_particle& f) {
	__pm3_assign_v(f.prev_pos, m.prev_pos);
	__pm3_assign_v(f.cur_pos, m.cur_pos);
	__pm3_assign_v(f.cur_vel, m.cur_vel);
	__pm3_assign_v(f.force, m.force);

	f.mass = m.mass;
	f.charge = m.charge;
	f.fixed = m.fixed;
	f.index = m.index;
}

void from_free_to_mesh(const free_particle& f, mesh_particle& m) {
	__pm3_assign_v(m.prev_pos, f.prev_pos);
	__pm3_assign_v(m.cur_pos, f.cur_pos);
	__pm3_assign_v(m.cur_vel, f.cur_vel);
	__pm3_assign_v(m.force, f.force);

	m.mass = f.mass;
	m.charge = f.charge;
	m.fixed = f.fixed;
	m.index = f.index;
}

void from_fluid_to_free(const fluid_particle& fl, free_particle& fr) {
	__pm3_assign_v(fr.prev_pos, fl.prev_pos);
	__pm3_assign_v(fr.cur_pos, fl.cur_pos);
	__pm3_assign_v(fr.cur_vel, fl.cur_vel);
	__pm3_assign_v(fr.force, fl.force);

	fr.mass = fl.mass;
	fr.index = fl.index;
}

void from_free_to_fluid(const free_particle& fr, fluid_particle& fl) {
	__pm3_assign_v(fl.prev_pos, fr.prev_pos);
	__pm3_assign_v(fl.cur_pos, fr.cur_pos);
	__pm3_assign_v(fl.cur_vel, fr.cur_vel);
	__pm3_assign_v(fl.force, fr.force);

	fl.mass = fr.mass;
	fl.index = fr.index;
}

} // -- namespace particles
} // -- namespace physim
