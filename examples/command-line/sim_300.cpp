// C includes
#include <string.h>

// C++ includes
#include <iostream>
#include <fstream>
using namespace std;

// Custom includes
#include "utils.hpp"

// physim includes
#include <physim/simulator.hpp>
#include <physim/particles/fluid_particle.hpp>
#include <physim/fluids/fluid.hpp>
using namespace physim;
using namespace particles;
using namespace math;
using namespace geometric;
using namespace fluids;

#define side__ 64
#define slice__ (side__*side__)
#define total__ (side__*side__*side__)

namespace study_cases {

	void sim_300_usage() {
		cout << "Simulation of a simple fluid" << endl;
		cout << endl;
		cout << "Options:" << endl;
		cout << endl;
		cout << "    --total-time t: total time of the simulation.         Default: 2.0" << endl;
		cout << "    --step t:       time step of the simulation.          Default: 0.01" << endl;
		cout << "    --solver s:     numerical solver to use.              Default: 'semi-euler'" << endl;
		cout << "        euler:      Euler integration method.." << endl;
		cout << "        semi-euler: Euler semi-implicit integration method." << endl;
		cout << "        verlet:     Verlet integration method." << endl;
		cout << "    --R r:			 Size of the neighbourhood.            Default: 0.01" << endl;
		cout << "    --vol v:		 Volume of the fluid.                  Default: 1.0" << endl;
		cout << "    --vis v:		 Viscosity of the fluid.               Default: 1.0" << endl;
		cout << "    --D d:			 Density at rest of the fluid.         Default: 1.0" << endl;
		cout << "    --threads n:    Number of threads.                    Default: 1" << endl;
	}

	void sim_300(int argc, char *argv[]) {
		float dt = 0.01f;
		float total_time = 5.0f;
		solver_type solv = solver_type::EulerSemi;
		// number of particles
		size_t N = total__;
		// neighbourhood size
		float R = 0.01f;
		// volume
		float vol = 1.0f;
		// viscosity
		float vis = 1.0f;
		// density
		float D = 1.0f;
		// number of threads
		size_t nt = 1;

		for (int i = 2; i < argc; ++i) {
			if (strcmp(argv[i], "-h") == 0 or strcmp(argv[i], "--help") == 0) {
				sim_300_usage();
				return;
			}
			else if (strcmp(argv[i], "--total-time") == 0) {
				total_time = atof(argv[i + 1]);
				++i;
			}
			else if (strcmp(argv[i], "--step") == 0) {
				dt = atof(argv[i + 1]);
				++i;
			}
			else if (strcmp(argv[i], "--R") == 0) {
				R = atof(argv[i + 1]);
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
			else if (strcmp(argv[i], "--D") == 0) {
				D = atof(argv[i + 1]);
				++i;
			}
			else if (strcmp(argv[i], "--threads") == 0) {
				nt = atoi(argv[i + 1]);
				++i;
			}
			else if (strcmp(argv[i], "--solver") == 0) {
				string solv_name = string(argv[i + 1]);
				if (solv_name == "euler") {
					solv = solver_type::EulerOrig;
				}
				else if (solv_name == "semi-euler") {
					solv = solver_type::EulerSemi;
				}
				else if (solv_name == "verlet") {
					solv = solver_type::Verlet;
				}
				else {
					cerr << "Error: invalid value for solver." << endl;
					return;
				}
				++i;
			}
			else {
				cerr << "Error: unknown option '" << string(argv[i]) << "'" << endl;
				return;
			}
		}

		timing::time_point begin, end;

		cout << "Fluid characteristics:" << endl;
		cout << "    Number of particles: " << N << endl;
		cout << "    Volume: " << vol << endl;
		cout << "    Viscosity: " << vis << endl;
		cout << "    Density: " << D << endl;
		cout << "    Neighbourhood size: " << R << endl;
		cout << "Simulation:" << endl;
		cout << "    total time: " << total_time << endl;
		cout << "    step time: " << dt << endl,
		cout << "    solver: ";
		if (solv == solver_type::EulerOrig) {
			cout << " Euler" << endl;
		}
		else if (solv == solver_type::EulerSemi) {
			cout << " Semi-Euler" << endl;
		}
		else if (solv == solver_type::Verlet) {
			cout << " Verlet" << endl;
		}

		cout << "Allocating..." << endl;
		begin = timing::now();
		fluid F;
		F.allocate(N, vol, D, vis, R);
		end = timing::now();
		cout << "    in " << timing::elapsed_seconds(begin, end) << " seconds" << endl;

		// assign initial positions
		cout << "Assigning initial positions..." << endl;
		begin = timing::now();
		for (size_t i = 0; i < N; ++i) {
			size_t slice = i/slice__;
			size_t __i = i%slice__;
			size_t row = __i/side__;
			size_t col = __i%side__;

			F.get_particles()[i].cur_pos = vec3(
				row*0.05f, col*0.05f, slice*0.05f
			);
		}
		end = timing::now();
		cout << "    in " << timing::elapsed_seconds(begin, end) << " seconds" << endl;

		cout << "Updating forces..." << endl;
		begin = timing::now();
		F.update_forces(nt);
		end = timing::now();
		cout << "    in " << timing::elapsed_seconds(begin, end) << " seconds" << endl;

		// output forces
		const fluid_particle *ps = F.get_particles();
		for (size_t i = 0; i < N; ++i) {
			cout << i << ": "
				 << ps[i].force.x << " " << ps[i].force.y << " " << ps[i].force.z
				 << endl;
		}
	}

} // -- namespace study_cases
