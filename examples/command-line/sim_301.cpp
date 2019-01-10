// C includes
#include <string.h>

// C++ includes
#include <iostream>
#include <fstream>
using namespace std;

// physim includes
#include <physim/simulator.hpp>
#include <physim/particles/fluid_particle.hpp>
#include <physim/fluids/newtonian.hpp>
using namespace physim;
using namespace particles;
using namespace math;
using namespace geometric;
using namespace fluids;

// custom includes
#include "utils.hpp"
#include "kernel_functions_fluids.hpp"

namespace study_cases {

void sim_301_usage() {
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
	cout << "    --R r:			 Size of the neighbourhood.     Default: 0.015" << endl;
	cout << "    --vol v:		 Volume of the fluid.           Default: 0.01" << endl;
	cout << "    --vis v:		 Viscosity of the fluid.        Default: 0.001" << endl;
	cout << "    --dens d:       Density at rest of the fluid.  Default: 1000.0" << endl;
	cout << "    --c v:			 Speed of sound.                Default: 1500.0" << endl;
	cout << "    --threads n:    Number of threads.             Default: 1" << endl;
	cout << "    --lenx x        Length in each dimension.      Default: 0.5" << endl;
	cout << "    --lenx y                                       Default: 0.5" << endl;
	cout << "    --lenx z                                       Default: 0.5" << endl;
	cout << "    --sidex x       Sizes in each dimension.       Default: 8" << endl;
	cout << "    --sidey y                                      Default: 8" << endl;
	cout << "    --sidez z                                      Default: 8" << endl;
	cout << endl;
}

void sim_301(int argc, char *argv[]) {
	float dt = 0.0001f;
	float total_time = 5.0f;
	solver_type solv = solver_type::EulerSemi;
	// neighbourhood size
	float h = 0.015f;
	// sizes and lengths
	float lenx, leny, lenz;
	lenx = leny = lenz = 0.5f;
	size_t sidex, sidey, sidez;
	sidex = sidey = sidez = 8;
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
			sim_301_usage();
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
		else if (strcmp(argv[i], "--threads") == 0) {
			nt = atoi(argv[i + 1]);
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
		else if (strcmp(argv[i], "--sidex") == 0) {
			sidex = atoi(argv[i + 1]);
			++i;
		}
		else if (strcmp(argv[i], "--sidey") == 0) {
			sidey = atoi(argv[i + 1]);
			++i;
		}
		else if (strcmp(argv[i], "--sidez") == 0) {
			sidez = atoi(argv[i + 1]);
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
			cerr << "Error: unknown option '" << string(argv[i])
				 << "'" << endl;
			return;
		}
	}

	// volume
	float vol = lenx*leny*lenz;
	// number of particles
	size_t N = sidex*sidey*sidez;

	timing::time_point begin, end;

	cout << "Fluid characteristics:" << endl;
	cout << "    Number of particles: "
		 << sidex << " x "
		 << sidey << " x "
		 << sidez << endl;
	cout << "    Volume: " << vol << endl;
	cout << "    Viscosity: " << vis << endl;
	cout << "    Density: " << dens << endl;
	cout << "    Neighbourhood size: " << h << endl;
	cout << "    Initial shape:" << endl;
	cout << "        " << lenx << " x " << leny << " x " << lenz << endl;
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
	fluid *F = new newtonian();
	F->allocate(N, vol, dens, vis, h, cs);
	end = timing::now();
	cout << "    in " << timing::elapsed_seconds(begin, end)
		 << " seconds" << endl;
	cout << "Mass per particle: " << F->get_particles()[0].mass << endl;

	kernel_scalar_function W;
	kernel_functions::density_poly6(h, W);
	F->set_kernel_density(W);

	kernel_vectorial_function gW;
	kernel_functions::pressure_poly6(h, gW);
	F->set_kernel_pressure(gW);

	kernel_scalar_function g2W;
	kernel_functions::viscosity_poly6(h, g2W);
	F->set_kernel_viscosity(g2W);

	// assign initial positions
	cout << "Assigning initial positions..." << endl;
	begin = timing::now();
	for (size_t i = 0; i < sidex; ++i) {
		for (size_t j = 0; j < sidey; ++j) {
			for (size_t k = 0; k < sidez; ++k) {
				vec3 pos(i*(lenx/sidex), j*(leny/sidey), k*(lenz/sidez));
				size_t idx = j*sidex*sidez + k*sidex + i;
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
	}

	cout << "Total execution time: " << total_exe_secs << endl;
	cout << "Simulation time: " << total_time << " seconds" << endl;
	cout << "Average execution time per update: "
		 << total_exe_secs/(sim_time/dt) << " seconds" << endl;

#define vec_out(v) v.x << "," << v.y << "," << v.z

	const fluid_particle *ps = F->get_particles();

	for (size_t i = 0; i < N; ++i) {
		const fluid_particle& p = ps[i];
		cout << i << " : " << endl;
		cout << "    " << "prev_pos: " << vec_out(p.prev_pos) << endl;
		cout << "    " << " cur_pos: " << vec_out(p.cur_pos) << endl;
		cout << "    " << " cur_vel: " << vec_out(p.cur_vel) << endl;
		cout << "    " << "   force: " << vec_out(p.force) << endl;
		cout << "    " << "    mass: " << p.mass << endl;
		cout << "    " << " density: " << p.density << endl;
		cout << "    " << "pressure: " << p.pressure << endl;
	}
}

} // -- namespace study_cases
