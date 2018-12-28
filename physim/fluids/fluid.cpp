#include <physim/fluids/fluid.hpp>

// C includes
#include <stdlib.h>

// C++ includes
#include <vector>
using namespace std;

// physim includes
#include <physim/math/private/math3.hpp>

#define sq(x) (x*x)

#define speed_sound 343.0f
#define speed_sound2 sq(speed_sound)

#define Pij(i,j) -ps[j].mass*(					\
	ps[i].pressure*(1.0f/sq(ps[i].density)) +	\
	ps[j].pressure*(1.0f/sq(ps[j].density)))

#define Vij(i,j) viscosity*ps[j].mass*(			\
	(ps[i].cur_vel - ps[j].cur_vel)*			\
	(1.0f/(ps[i].density*ps[j].density)))

namespace physim {
using namespace particles;
using namespace structures;

namespace fluids {

// PUBLIC

fluid::fluid() {
	N = 0;
	volume = 0.0f;
	density = 0.0f;
	viscosity = 0.0f;
	ps = nullptr;

	kernel_function empty =
	[](const fluid_particle&, const fluid_particle&, float) -> float
	{ return 1.0f; };

	// use empty kernels, to avoid segmentation
	// faults and things like these
	kernel = empty;
	kernel_pressure = empty;
	kernel = empty;
}

fluid::~fluid() {
	clear();
}

// OPERATORS

particles::fluid_particle *fluid::operator[] (size_t i) {
	return &ps[i];
}

const particles::fluid_particle *fluid::operator[] (size_t i) const {
	return &ps[i];
}

// MODIFIERS

void fluid::allocate(size_t n, float vol, float dens, float visc, float r) {
	if (tree == nullptr) {
		tree = new octree();
	}
	else {
		tree->clear();
	}

	R = r;
	volume = vol;
	density = dens;
	viscosity = visc;

	float mass_per_particle = (density*volume)/n;

	if (ps != nullptr) {
		// clear and allocate only if necessary
		if (n != N) {
			clear();
			N = n;
			ps = static_cast<fluid_particle *>(malloc(N*sizeof(fluid_particle)));
		}
	}
	else {
		N = n;
		ps = static_cast<fluid_particle *>(malloc(N*sizeof(fluid_particle)));
	}

	for (size_t i = 0; i < N; ++i) {
		ps[i].init();
		ps[i].index = i;
		ps[i].mass = mass_per_particle;
	}
}

void fluid::clear() {
	if (ps != nullptr) {
		free(ps);
		ps = nullptr;
	}
}

void fluid::update_forces() {
	make_partition();

	const float R2 = R*R;

	// Compute neighbour lists, and squared distances
	// between a particle and its neighbours.
	// We need a dynamic list for the neighbours, but a
	// fixed-size list of these lists will suffice.
	vector<size_t> neighs[N];
	vector<float> d2s[N];

	for (size_t i = 0; i < N; ++i) {
		tree->get_indices(ps[i].cur_pos, R, neighs[i]);
		d2s[i] = vector<float>(neighs[i].size());
	}

	// compute density and pressure of each particle,
	// store squared distance
	for (size_t i = 0; i < N; ++i) {
		ps[i].density = 0.0f;

		// iterate over the list of neighbours.
		// initialise density, pressure, and store squared distance
		for (size_t j_it = 0; j_it < neighs[i].size(); ++j_it) {
			size_t j = neighs[i][j_it];

			d2s[i][j_it] = __pm3_dist2(ps[i].cur_pos, ps[j].cur_pos);
			if (d2s[i][j_it] <= R2) {
				ps[i].density += ps[j].mass*kernel(ps[i], ps[j], d2s[i][j_it]);
			}
		}

		ps[i].pressure = speed_sound2*(ps[i].density - density);
	}

	// compute forces of the fluid (due to pressure and viscosity)
	for (size_t i = 0; i < N; ++i) {

		float aP = 0.0f;	// acceleration from pressure
		float aV = 0.0f;	// acceleration from viscosity

		for (size_t i = 0; i < N; ++i) {
			ps[i].density = 0.0f;

			for (size_t j_it = 0; j_it < neighs[i].size(); ++j_it) {
				size_t j = neighs[i][j_it];

				if (d2s[i][j_it] <= R2) {
					aP += Pij(i,j)*kernel_pressure(ps[i], ps[j], d2s[i][j_it]);
					aV += Pij(i,j)*kernel_viscosity(ps[i], ps[j], d2s[i][j_it]);
				}
			}

			ps[i].force += (aP + aV)*ps[i].mass;
		}
	}
}

void fluid::make_partition() {
	tree->clear();

	// compute offset of position w.r.t. to the
	// beginning of its struct
	size_t base = reinterpret_cast<size_t>( &ps[0] );
	size_t member_pos = reinterpret_cast<size_t>( &ps[0].cur_pos );
	size_t offset = member_pos - base;

	tree->init(&ps[0], N, 1, sizeof(fluid_particle), offset);
}

// SETTERS

void fluid::set_kernel
(const kernel_function& W, const kernel_function& W_p, const kernel_function& W_v)
{
	kernel = W;
	kernel_pressure = W_p;
	kernel_viscosity = W_v;
}

// GETTERS

size_t fluid::size() const {
	return N;
}

float fluid::get_volume() const {
	return volume;
}

float fluid::get_density() const {
	return density;
}

float fluid::get_viscosity() const {
	return viscosity;
}

fluid_particle **fluid::get_particles() {
	return &ps;
}
fluid_particle *const *fluid::get_particles() const {
	return &ps;
}

} // -- namespace fluids
} // -- namespace physim

