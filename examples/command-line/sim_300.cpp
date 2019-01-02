// C includes
#include <string.h>

// C++ includes
#include <iostream>
#include <fstream>
using namespace std;

// physim includes
#include <physim/simulator.hpp>
#include <physim/particles/fluid_particle.hpp>
#include <physim/fluids/fluid.hpp>
using namespace physim;
using namespace particles;
using namespace math;
using namespace geometric;
using namespace fluids;

// custom includes
#include "utils.hpp"
#include "kernel_functions_fluids.hpp"

namespace study_cases {

	void sim_300_usage() {
		cout << "Simulation of a simple fluid" << endl;
		cout << endl;
		cout << "Options:" << endl;
		cout << endl;
		cout << "    --R r:			 Size of the neighbourhood.     Default: 0.015" << endl;
		cout << "    --vol v:		 Volume of the fluid.           Default: 0.01" << endl;
		cout << "    --vis v:		 Viscosity of the fluid.        Default: 0.001" << endl;
		cout << "    --dens d:       Density at rest of the fluid.  Default: 1000.0" << endl;
		cout << "    --c v:			 Speed of sound.                Default: 1500.0" << endl;
		cout << "    --p n:			 Number of particles per side.  Default: 16" << endl;
		cout << "    --threads n:    Number of threads.             Default: 1" << endl;
		cout << endl;
	}

	void sim_300(int argc, char *argv[]) {
		float dt = 0.0001f;
		//solver_type solv = solver_type::EulerSemi;
		// number of particles per side
		size_t side__ = 16;
		// neighbourhood size
		float h = 0.015f;
		// volume
		float vol = 0.01f;
		// viscosity
		float vis = 0.001f;
		// density
		float dens = 1000.0f;
		// speed of sound
		float cs = 1500.0f;
		// number of threads
		size_t nt = 1;

		for (int i = 2; i < argc; ++i) {
			if (strcmp(argv[i], "-h") == 0 or strcmp(argv[i], "--help") == 0) {
				sim_300_usage();
				return;
			}
			else if (strcmp(argv[i], "--R") == 0) {
				h = atof(argv[i + 1]);
				++i;
			}
			else if (strcmp(argv[i], "--vol") == 0) {
				vol = atof(argv[i + 1]);
				++i;
			}
			else if (strcmp(argv[i], "--vis") == 0) {
				vis = atof(argv[i + 1]);
				++i;
			}
			else if (strcmp(argv[i], "--dens") == 0) {
				dens = atof(argv[i + 1]);
				++i;
			}
			else if (strcmp(argv[i], "--c") == 0) {
				cs = atof(argv[i + 1]);
				++i;
			}
			else if (strcmp(argv[i], "--p") == 0) {
				side__ = atoi(argv[i + 1]);
				++i;
			}
			else if (strcmp(argv[i], "--threads") == 0) {
				nt = atoi(argv[i + 1]);
				++i;
			}
			else if (strcmp(argv[i], "--particles") == 0) {
				side__ = atoi(argv[i + 1]);
				++i;
			}
			else {
				cerr << "Error: unknown option '" << string(argv[i]) << "'" << endl;
				return;
			}
		}

		size_t N = side__*side__*side__;

		timing::time_point begin, end;

		cout << "Fluid characteristics:" << endl;
		cout << "    Number of particles: " << N << endl;
		cout << "    Volume: " << vol << endl;
		cout << "    Viscosity: " << vis << endl;
		cout << "    Density: " << dens << endl;
		cout << "    Neighbourhood size: " << h << endl;
		cout << "    Initial shape:" << endl;
		cout << "        0.2 x 0.2 x 0.1" << endl;
		cout << "Simulation:" << endl;
		cout << "    step time: " << dt << endl;

		cout << "Allocating..." << endl;
		begin = timing::now();
		fluid F;
		F.allocate(N, vol, dens, vis, h, cs);
		end = timing::now();
		cout << "    in " << timing::elapsed_seconds(begin, end) << " seconds" << endl;

		cout << "Mass per particle: " << F.get_particles()[0].mass << endl;

		kernel_scalar_function W;
		kernel_functions::density_poly6(h, W);
		kernel_vectorial_function gW;
		kernel_functions::pressure_poly6(h, gW);
		kernel_scalar_function g2W;
		kernel_functions::viscosity_poly6(h, g2W);
		F.set_kernel(W, gW, g2W);

		// assign initial positions
		cout << "Assigning initial positions..." << endl;
		begin = timing::now();
		for (size_t i = 0; i < side__; ++i) {
			for (size_t j = 0; j < side__; ++j) {
				for (size_t k = 0; k < side__; ++k) {
					vec3 pos(i*(0.2f/side__), j*(0.2f/side__), k*(0.1f/side__));
					size_t idx = i*side__*side__ + j*side__ + k;
					F.get_particles()[idx].cur_pos = pos;
				}
			}
		}

		end = timing::now();
		cout << "    in " << timing::elapsed_seconds(begin, end) << " seconds" << endl;

		cout << "Updating forces..." << endl;
		begin = timing::now();
		F.update_forces(nt);
		end = timing::now();
		cout << "    in " << timing::elapsed_seconds(begin, end) << " seconds" << endl;

		const fluid_particle *ps = F.get_particles();

		// output forces
		cout << "FORCES" << endl;
		for (size_t i = 0; i < N; ++i) {
			cout << i << ": "
				 << ps[i].force.x << " " << ps[i].force.y << " " << ps[i].force.z
				 << endl;
		}

		// compute velocities
		cout << "VELOCITIES" << endl;
		for (size_t i = 0; i < N; ++i) {
			vec3 vel = ps[i].force*dt*(1.0f/ps[i].mass);
			cout << i << ": " << vel.x << " " << vel.y << " " << vel.z << endl;
		}
	}

} // -- namespace study_cases
