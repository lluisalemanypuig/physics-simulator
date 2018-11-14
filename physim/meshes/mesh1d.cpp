#include <physim/meshes/mesh1d.hpp>

// C includes
#include <assert.h>
#include <stdlib.h>

// physim includes
#include <physim/math/math_private.hpp>

namespace physim {
namespace meshes {

// this macro requires some local variables to be defined:
//     math::vec3 F1_m1;
//     math::vec3 dir;
//     math::vec3 dvel;
//     float elastic_term, damping_term, dist;
// Other values are class members:
//     Ke, Kd
/*
	// direction vector
	__pm_sub_v_v(dir, ps[i + 1]->cur_pos, ps[i]->cur_pos);
	dist = __pm_norm(dir);
	__pm_normalise(dir, dir);
	// difference of velocities
	__pm_sub_v_v(dvel, ps[i + 1]->cur_vel, ps[i]->cur_vel);
	// compute terms
	elastic_term = Ke*(dist - ds[i]);
	damping_term = Kd*__pm_dot(dvel, dir);
	// compute forces
	__pm_assign_vs(F1_m1, dir, elastic_term + damping_term);
	__pm_add_acc_v(ps[i]->force, F1_m1);
	__pm_invert(F1_m1, F1_m1);
	__pm_add_acc_v(ps[i + 1]->force, F1_m1);
*/
// d is the distance between particles i and j
#define compute_forces(i,j, d)											\
	__pm_sub_v_v(dir, ps[j]->cur_pos, ps[i]->cur_pos);					\
	dist = __pm_norm(dir);												\
	__pm_normalise(dir, dir);											\
	__pm_sub_v_v(dvel, ps[j]->cur_vel, ps[i]->cur_vel);					\
	__pm_assign_vs(F1_m1, dir, Ke*(dist - d) + Kd*__pm_dot(dvel, dir));	\
	__pm_add_acc_v(ps[i]->force, F1_m1);								\
	__pm_invert(F1_m1, F1_m1);											\
	__pm_add_acc_v(ps[j]->force, F1_m1)

// PUBLIC

mesh1d::mesh1d() : mesh() {
	mt = mesh_type::d1;
	ds = nullptr;
	stretch = false;
	bend = false;
}
mesh1d::mesh1d(float ke, float kd) : mesh(ke,kd) {
	mt = mesh_type::d1;
	ds = nullptr;
	stretch = false;
	bend = false;
}

mesh1d::~mesh1d() {
	clear();
}

// MODIFIERS

void mesh1d::make_initial_state() {
	if (ds != nullptr) {
		free(ds);
	}
	ds = (math::vec2 *)malloc((N - 1)*sizeof(math::vec2));

	for (size_t i = 0; i < N - 2; ++i) {
		ds[i].x = __pm_dist(ps[i]->cur_pos, ps[i + 1]->cur_pos);
		ds[i].y = __pm_dist(ps[i]->cur_pos, ps[i + 2]->cur_pos);
	}
	ds[N - 2].x = __pm_dist(ps[N - 2]->cur_pos, ps[N - 1]->cur_pos);
}

void mesh1d::update_forces() {
	assert(ds != nullptr);

	math::vec3 F1_m1;
	math::vec3 dir;
	math::vec3 dvel;
	float dist;

	// compute the bending forces for all particles
	// compute the stretch forces for the N - 1 particles
	for (size_t i = 0; i < N - 2; ++i) {
		if (stretch) {
			compute_forces(i, i + 1, ds[i].x);
		}
		if (bend) {
			compute_forces(i, i + 2, ds[i].y);
		}
	}

	// compute stretch force for the last particle
	if (stretch) {
		compute_forces(N - 2, N - 1, ds[N - 2].x);
	}
}

void mesh1d::clear() {
	mesh::clear();
	if (ds != nullptr) {
		free(ds);
		ds = nullptr;
	}
}

void mesh1d::simulate_stretch(bool s) {
	stretch = s;
}

void mesh1d::simulate_bend(bool s) {
	bend = s;
}

// GETTERS

bool mesh1d::is_simulating_stretch() const {
	return stretch;
}

bool mesh1d::is_simulating_bend() const {
	return bend;
}

} // -- namespace meshes
} // -- namespace physim
