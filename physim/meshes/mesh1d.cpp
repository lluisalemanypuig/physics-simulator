#include <physim/meshes/mesh1d.hpp>

// C includes
#include <assert.h>
#include <stdlib.h>

// physim includes
#include <physim/math/math_private.hpp>

namespace physim {
using namespace particles;

namespace meshes {

// PUBLIC

mesh1d::mesh1d() : mesh() {
	mt = mesh_type::d1;
}
mesh1d::mesh1d(float ke, float kd) : mesh(ke,kd) {
	mt = mesh_type::d1;
}

mesh1d::~mesh1d() {
	this->clear();
}

// MODIFIERS

void mesh1d::make_initial_state() {
	ds = std::vector<float>(N - 1, 0.0f);
	for (size_t i = 0; i < N - 1; ++i) {
		ds[i] = __pm_dist(ps[i]->cur_pos, ps[i+1]->cur_pos);
	}
}

void mesh1d::update_forces() {
	assert(ds.size() == N - 1);

	math::vec3 F1_m1;
	math::vec3 dir;
	math::vec3 dvel;
	float elastic_term, damping_term;

	for (size_t i = 0; i < N - 1; ++i) {
		// direction vector
		__pm_sub_v_v(dir, ps[i+1]->cur_pos, ps[i]->cur_pos);
		float dist = __pm_norm(dir);
		__pm_normalise(dir, dir);

		// difference of velocities
		__pm_sub_v_v(dvel, ps[i+1]->cur_vel, ps[i]->cur_vel);

		// compute terms
		elastic_term = Ke*(dist - ds[i]);
		damping_term = Kd*__pm_dot(dvel, dir);

		// compute forces
		__pm_assign_vs(F1_m1, dir, elastic_term + damping_term);

		__pm_add_acc_v(ps[i]->force, F1_m1);
		__pm_invert(F1_m1, F1_m1);
		__pm_add_acc_v(ps[i + 1]->force, F1_m1);
	}
}

void mesh1d::allocate(size_t n) {
	if (ps != nullptr) {
		clear();
	}

	N = n;
	ps = (mesh_particle **)malloc(N*sizeof(mesh_particle *));

	for (size_t i = 0; i < N; ++i) {
		ps[i] = new mesh_particle();
		ps[i]->index = i;
	}
}

void mesh1d::clear() {
	mesh::clear();
	ds.clear();
}

} // -- namespace meshes
} // -- namespace physim
