#include "study_cases.hpp"

#include <physim/meshes/mesh2d.hpp>
#include <physim/meshes/mesh2d_regular.hpp>
using namespace meshes;

namespace study_cases {

	void mesh2d_regular_usage() {
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
		cout << endl;
	}

	void mesh2d_regular_case(int argc, char *argv[]) {
		float dt = 0.01f;
		float total_time = 2.0f;
		solver_type solv = solver_type::EulerSemi;
		size_t n = 5;
		size_t m = 5;

		for (int i = 2; i < argc; ++i) {
			if (strcmp(argv[i], "-h") == 0 or strcmp(argv[i], "--help") == 0) {
				mesh2d_regular_usage();
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
			else {
				cerr << "Error: unknown option '" << string(argv[i]) << "'" << endl;
				return;
			}
		}

		float length = 10.0f;
		float width = 10.f;

		timing::time_point begin_build = timing::now();

		// build regular mesh
		mesh2d_regular *M = new mesh2d_regular();
		M->allocate(n*m, 2.0f);
		M->set_dimensions(n, m);

		M->set_elasticity(500.0f);
		M->set_damping(0.5f);

		mesh_particle **mp = M->get_particles();

		// fix some particles
		mp[ M->get_global_index(0,0) ]->fixed = true;
		mp[ M->get_global_index(0, m - 1) ]->fixed = true;

		// make positions
		for (size_t i = 0; i < n; ++i) {
			for (size_t j = 0; j < m; ++j) {
				mp[ M->get_global_index(i,j) ]->cur_pos =
						vec3((length/n)*i, (width/m)*j, 0.0f);
			}
		}

		M->make_initial_state();

		timing::time_point end_build = timing::now();

		// -----------------------------------------
		// -- initialise simulator
		simulator S(solv, dt);

		S.add_mesh(M);

		S.add_gravity_acceleration(vec3(0.0f,-9.81f,0.0f));
		// -----------------------------------------

		timing::time_point begin_sim = timing::now();

		// execute simulation
		while (S.get_current_time() <= total_time) {
			S.apply_time_step();
		}

		timing::time_point end_sim = timing::now();

		cout << "Time spent in building: "
			 << timing::elapsed_seconds(begin_build, end_build) << " s" << endl;
		cout << "Time spent in simulating: "
			 << timing::elapsed_seconds(begin_sim, end_sim) << " s" << endl;
	}

} // -- namespace study_cases

