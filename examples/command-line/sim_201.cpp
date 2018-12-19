// C includes
#include <string.h>

// C++ includes
#include <iostream>
#include <fstream>
using namespace std;

// Custom includes
#include "utils.hpp"

// physim includes
#include <physim/initialiser/initialiser.hpp>
#include <physim/particles/free_particle.hpp>
#include <physim/meshes/mesh2d.hpp>
#include <physim/meshes/mesh2d_regular.hpp>
#include <physim/geometry/plane.hpp>
#include <physim/simulator.hpp>
using namespace physim;
using namespace particles;
using namespace math;
using namespace geometric;
using namespace init;
using namespace meshes;

namespace study_cases {

	void sim_201_usage() {
		cout << "2-dimensional mesh:" << endl;
		cout << endl;
		cout << "This study case of a regular 2-dimensinal mesh" << endl;
		cout << endl;
		cout << "Options:" << endl;
		cout << endl;
		cout << "    --total-time t: total time of the simulation.         Default: 2.0" << endl;
		cout << "    --step t:       time step of the simulation.          Default: 0.01" << endl;
		cout << "    --solver s:     numerical solver to use.              Default: 'semi-euler'" << endl;
		cout << "        euler:      Euler integration method.." << endl;
		cout << "        semi-euler: Euler semi-implicit integration method." << endl;
		cout << "        verlet:     Verlet integration method." << endl;
		cout << "    -n k:           number of rows of the regular mesh.   Default: 5" << endl;
		cout << "    -m k:           number of columns of the regular mesh.   Default: 5" << endl;
		cout << "    --bend:         activate bend forces" << endl;
		cout << "    --shear:        activate shear forces" << endl;
		cout << "    --stretch:      activate stretch forces" << endl;
		cout << endl;
	}

	void sim_201(int argc, char *argv[]) {
		float dt = 0.01f;
		float total_time = 5.0f;
		solver_type solv = solver_type::EulerSemi;
		size_t n = 5;
		size_t m = 5;
		bool shear = false;
		bool stretch = false;
		bool bend = false;

		for (int i = 2; i < argc; ++i) {
			if (strcmp(argv[i], "-h") == 0 or strcmp(argv[i], "--help") == 0) {
				sim_201_usage();
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
			else if (strcmp(argv[i], "-n") == 0) {
				n = atoi(argv[i + 1]);
				++i;
			}
			else if (strcmp(argv[i], "-m") == 0) {
				m = atoi(argv[i + 1]);
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
			else if (strcmp(argv[i], "--bend") == 0) {
				bend = true;
			}
			else if (strcmp(argv[i], "--shear") == 0) {
				shear = true;
			}
			else if (strcmp(argv[i], "--stretch") == 0) {
				stretch = true;
			}
			else {
				cerr << "Error: unknown option '" << string(argv[i]) << "'" << endl;
				return;
			}
		}

		float length = 10.0f;
		float height = 10.0f;

		timing::time_point begin_build = timing::now();

		// build regular mesh
		mesh2d_regular *M = new mesh2d_regular();
		M->simulate_bend(bend);
		M->simulate_shear(shear);
		M->simulate_stretch(stretch);

		M->allocate(n*m, 5.0f);
		M->set_dimensions(n, m);

		M->set_elasticity(500.0f);
		M->set_damping(0.5f);

		mesh_particle **mp = M->get_particles();

		// fix some particles
		mp[ M->get_global_index(0,m-1) ]->fixed = true;
		mp[ M->get_global_index(n-1,m - 1) ]->fixed = true;

		// make positions
		for (size_t i = 0; i < n; ++i) {
			for (size_t j = 0; j < m; ++j) {
				mp[ M->get_global_index(i,j) ]->cur_pos =
						vec3((length/n)*i, (height/m)*j, 0.0f);
			}
		}

		timing::time_point end_build = timing::now();

		// -----------------------------------------
		// -- initialise simulator
		simulator S(solv, dt);

		S.add_mesh(M);

		S.set_gravity_acceleration(vec3(0.0f,-9.81f,0.0f));
		// -----------------------------------------

		timing::time_point begin_sim = timing::now();
		float sim_time = 0.0f;

		// execute simulation
		while (sim_time <= total_time) {
			S.apply_time_step();
			sim_time += dt;
		}

		timing::time_point end_sim = timing::now();

		cout << "Mesh characteristics:" << endl;
		cout << "    dimensions: " << n << "x" << m << endl;
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
		cout << "    bending? " << (bend ? "Yes" : "No") << endl;
		cout << "    shear? " << (shear ? "Yes" : "No") << endl;
		cout << "    stretch? " << (stretch ? "Yes" : "No") << endl;

		double t_build = timing::elapsed_seconds(begin_build, end_build);
		double t_sim = timing::elapsed_seconds(begin_sim, end_sim);
		cout << "Time spent in building: " << t_build << " s" << endl;
		cout << "Time spent in simulating: " << t_sim << " s" << endl;
		cout << "Total: " << t_build + t_sim << " s" << endl;
	}

} // -- namespace study_cases

