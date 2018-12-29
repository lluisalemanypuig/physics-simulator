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

namespace study_cases {

	void sim_300_usage() {
		cout << "Simulation of a simple fluid" << endl;
		cout << endl;
		cout << "Options:" << endl;
		cout << endl;
		cout << "    --total-time t: total time of the simulation.  Default: 2.0" << endl;
		cout << "    --step t:       time step of the simulation.   Default: 0.0001" << endl;
		cout << "    --solver s:     numerical solver to use.       Default: 'semi-euler'" << endl;
		cout << "        euler:      Euler integration method.." << endl;
		cout << "        semi-euler: Euler semi-implicit integration method." << endl;
		cout << "        verlet:     Verlet integration method." << endl;
		cout << "    --R r:			 Size of the neighbourhood.     Default: 0.06" << endl;
		cout << "    --vol v:		 Volume of the fluid.           Default: 10.0" << endl;
		cout << "    --vis v:		 Viscosity of the fluid.        Default: 0.001" << endl;
		cout << "    --dens d:       Density at rest of the fluid.  Default: 1000.0" << endl;
		cout << "    --c v:			 Speed of sound.                Default: 1500.0" << endl;
		cout << "    --p n:			 Number of particles per side.  Default: 16" << endl;
		cout << "    --threads n:    Number of threads.             Default: 1" << endl;
		cout << endl;
	}

	void sim_300(int argc, char *argv[]) {
		float dt = 0.0001f;
		float total_time = 5.0f;
		solver_type solv = solver_type::EulerSemi;
		// number of particles per side
		size_t side__ = 16;
		// neighbourhood size
		float R = 0.06f;
		// volume
		float vol = 10.0f;
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

		size_t slice__ = side__*side__;
		size_t total__ = side__*side__*side__;
		size_t N = total__;

		timing::time_point begin, end;

		cout << "Fluid characteristics:" << endl;
		cout << "    Number of particles: " << N << endl;
		cout << "    Volume: " << vol << endl;
		cout << "    Viscosity: " << vis << endl;
		cout << "    Density: " << dens << endl;
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
		F.allocate(N, vol, dens, vis, R, cs);
		end = timing::now();
		cout << "    in " << timing::elapsed_seconds(begin, end) << " seconds" << endl;

		cout << "Mass per particle: " << F.get_particles()[0].mass << endl;

		kernel_scalar_function W =
		[R](float r2) -> float
		{
			float k = R*R - r2;
			float PI = static_cast<float>(M_PI);
			return (315.0f/(64.0f*PI*std::pow(R, 9.0f)))*k*k*k;
		};
		kernel_vectorial_function gW =
		[R](const vec3& r, float r2, vec3& res) -> void
		{
			static const float PI = static_cast<float>(M_PI);

			float k = R*R - r2;
			cout << "        k= " << k << endl;
			float s = (-945.0f/(32.0f*PI*std::pow(R, 9.0f)))*k*k;
			cout << "        s= " << s << endl;
			res = r*s;
		};
		kernel_scalar_function g2W =
		[R](float r2) -> float
		{
			float k = R*R - r2;
			float PI = static_cast<float>(M_PI);
			return (945.0f/(8.0f*PI*std::pow(R, 9.0f)))*k*(r2 - 0.75f*k);
		};
		F.set_kernel(W, gW, g2W);

		// assign initial positions
		cout << "Assigning initial positions..." << endl;
		begin = timing::now();
		for (size_t i = 0; i < N; ++i) {
			size_t slice = i/slice__;
			size_t __i = i%slice__;
			size_t row = __i/side__;
			size_t col = __i%side__;

			F.get_particles()[i].cur_pos = vec3(
				row*(0.5f/side__), col*(0.5f/side__), slice*(0.5f/side__)
			);
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
