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

	void sim_302_usage() {
		cout << "Simulation of a simple fluid" << endl;
		cout << endl;
		cout << "Used for measuring efficiency of fluid simulation." << endl;
		cout << endl;
		cout << "Options:" << endl;
		cout << endl;
		cout << "    --total-time t: total time of the simulation.  Default: 2.0" << endl;
		cout << "    --step t:       time step of the simulation.   Default: 0.0001" << endl;
		cout << "    --solver s:     numerical solver to use.       Default: 'semi-euler'" << endl;
		cout << "        euler:      Euler integration method.." << endl;
		cout << "        semi-euler: Euler semi-implicit integration method." << endl;
		cout << "        verlet:     Verlet integration method." << endl;
		cout << "    --R r:          Size of the neighbourhood.     Default: 0.015" << endl;
		cout << "    --vol v:        Volume of the fluid.           Default: 0.01" << endl;
		cout << "    --vis v:        Viscosity of the fluid.        Default: 0.001" << endl;
		cout << "    --dens d:       Density at rest of the fluid.  Default: 1000.0" << endl;
		cout << "    --c v:          Speed of sound.                Default: 1500.0" << endl;
		cout << "    --p n:          Number of particles per side.  Default: 16" << endl;
		cout << "    --threads n:    Number of threads.             Default: 1" << endl;
		cout << "    --lenx x:       Length in x axis.              Default: 0.5" << endl;
		cout << "    --leny y:       Length in y axis.              Default: 0.5" << endl;
		cout << "    --lenz z:       Length in z axis.              Default: 0.5" << endl;
		cout << endl;
	}

	void sim_302(int argc, char *argv[]) {
		float dt = 0.01f;
		float total_time = 5.0f;
		solver_type solv = solver_type::EulerSemi;
		float lenx = 0.5f;
		float leny = 0.5f;
		float lenz = 0.5f;
		size_t side__ = 16;		// number of particles per side
		float h = 0.015f;		// neighbourhood size
		float vol = 0.01f;		// volume
		float vis = 0.001f;		// viscosity
		float dens = 1000.0f;	// density
		float cs = 1500.0f;		// speed of sound
		size_t nt = 1;			// number of threads

		for (int i = 2; i < argc; ++i) {
			if (strcmp(argv[i], "-h") == 0 or strcmp(argv[i], "--help") == 0) {
				sim_302_usage();
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
			else if (strcmp(argv[i], "--lenx") == 0) {
				lenx = atof(argv[i + 1]);
				++i;
			}
			else if (strcmp(argv[i], "--leny") == 0) {
				leny = atof(argv[i + 1]);
				++i;
			}
			else if (strcmp(argv[i], "--lenz") == 0) {
				lenz = atof(argv[i + 1]);
				++i;
			}
			else {
				cerr << "Error: unknown option '" << string(argv[i])
					 << "'" << endl;
				return;
			}
		}

		size_t N = side__*side__*side__;
		timing::time_point begin, end;

		cout << "Allocating..." << endl;
		begin = timing::now();
		fluid *F = new fluid();
		F->allocate(N, vol, dens, vis, h, cs);
		end = timing::now();
		cout << "    in " << timing::elapsed_seconds(begin, end)
			 << " seconds" << endl;

		cout << "Fluid characteristics:" << endl;
		cout << "    Number of particles: " << N << endl;
		cout << "    Volume: " << vol << endl;
		cout << "    Viscosity: " << vis << endl;
		cout << "    Density: " << dens << endl;
		cout << "    Neighbourhood size: " << h << endl;
		cout << "    Initial shape:" << endl;
		cout << "        " << lenx << " x " << leny << " x " << lenz << endl;
		cout << "    Mass per particle: " << F->get_particles()[0].mass << endl;
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

		kernel_scalar_function W;
		kernel_functions::density_poly6(h, W);
		kernel_vectorial_function gW;
		kernel_functions::pressure_poly6(h, gW);
		kernel_scalar_function g2W;
		kernel_functions::viscosity_poly6(h, g2W);
		F->set_kernel(W, gW, g2W);

		// assign initial positions
		cout << "Assigning initial positions..." << endl;
		begin = timing::now();
		for (size_t i = 0; i < side__; ++i) {
			for (size_t j = 0; j < side__; ++j) {
				for (size_t k = 0; k < side__; ++k) {
					vec3 pos(i*(lenx/side__), j*(leny/side__), k*(lenz/side__));
					size_t idx = i*side__*side__ + j*side__ + k;
					F->get_particles()[idx].cur_pos = pos;
				}
			}
		}
		end = timing::now();
		cout << "    in " << timing::elapsed_seconds(begin, end)
			 << " seconds" << endl;

		simulator S(solv, dt);
		S.add_fluid(F);
		S.set_gravity_acceleration(vec3(0.0f, -9.81f, 0.0f));
		plane *base = new plane(vec3(0,1,0), vec3(0,-0.25,0));
		plane *w1 = new plane(vec3(1,0,0), vec3(-0.25,0,0));
		plane *w2 = new plane(vec3(-1,0,0), vec3(0.75,0,0));
		plane *w3 = new plane(vec3(0,0,1), vec3(0,0,-0.25));
		plane *w4 = new plane(vec3(0,0,-1), vec3(0,0,0.75));
		S.add_geometry(base);
		S.add_geometry(w1);
		S.add_geometry(w2);
		S.add_geometry(w3);
		S.add_geometry(w4);

		float sim_time = 0.0f;
		double total_exe_secs = 0.0;

		cout << "Simulating..." << endl;
		while (sim_time <= total_time) {
			begin = timing::now();
			S.simulate_fluids(nt);
			end = timing::now();
			double elapsed_exe_secs = timing::elapsed_seconds(begin, end);
			total_exe_secs += elapsed_exe_secs;

			sim_time += dt;
			cout << "Simulation time: " << sim_time << endl;
		}

		cout << "Total execution time: " << total_exe_secs << endl;
		cout << "Simulation time: " << total_time << " seconds" << endl;
		cout << "Average execution time per update: "
			 << total_exe_secs/(sim_time/dt) << " seconds" << endl;
	}

} // -- namespace study_cases
