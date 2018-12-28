// C includes
#include <string.h>

// C++ includes
#include <iostream>
#include <fstream>
using namespace std;

// Custom includes
#include "utils.hpp"

// physim includes
#include <physim/meshes/mesh1d.hpp>
#include <physim/geometry/plane.hpp>
#include <physim/simulator.hpp>
using namespace physim;
using namespace particles;
using namespace math;
using namespace geometric;
using namespace meshes;

namespace study_cases {

	void sim_200_print_geogebra_trajectory(const vector<vec3>& t, ofstream& fout) {
		fout << "{";
		for (size_t i = 0; i < t.size(); ++i) {
			const vec3& v = t[i];
			fout << "Point({"
				 << v.x << "," << v.y
				 << "})";
			if (i < t.size() - 1) {
				fout << ",";
			}
		}
		fout << "}" << endl;
	}

	void sim_200_usage() {
		cout << "1-dimensional mesh:" << endl;
		cout << endl;
		cout << "This study case is merely a spring of a few particles" << endl;
		cout << endl;
		cout << "Options:" << endl;
		cout << endl;
		cout << "    --total-time t: total time of the simulation.         Default: 2.0" << endl;
		cout << "    --friction f:   friction coefficient of the particle. Default: 0.0" << endl;
		cout << "    --solver s:     numerical solver to use.              Default: 'semi-euler'" << endl;
		cout << "        euler:      Euler integration method. Numerically unstable." << endl;
		cout << "        semi-euler: Euler semi-implicit integration method. Numerically stable." << endl;
		cout << "        verlet:     Verlet integration method. Numerically even more stable." << endl;
		cout << "    --print:        Print trajectory of particles.        Default: do not print" << endl;
		cout << endl;
		cout << "    [-o|--output]:  store the particle's trajectory in the specified file." << endl;
	}

	void sim_200(int argc, char *argv[]) {
		string output = "none";

		float dt = 0.01f;
		float total_time = 2.0f;
		bool print = false;
		solver_type solv = solver_type::EulerSemi;

		for (int i = 2; i < argc; ++i) {
			if (strcmp(argv[i], "-h") == 0 or strcmp(argv[i], "--help") == 0) {
				sim_200_usage();
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
			else if (strcmp(argv[i], "--print") == 0) {
				print = true;
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
			else if (strcmp(argv[i], "-o") == 0 or strcmp(argv[i], "--output") == 0) {
				output = string(argv[i + 1]);
				++i;
			}
			else {
				cerr << "Error: unknown option '" << string(argv[i]) << "'" << endl;
				return;
			}
		}

		mesh1d *m = new mesh1d();
		// just for the sake of debugging
		m->allocate(5);
		m->clear();
		m->allocate(5);

		m->set_elasticity(500.0f);
		m->set_damping(0.5f);

		mesh_particle **mp = m->get_particles();

		mp[0]->fixed = true;

		mp[0]->cur_pos = vec3(0.0f, 10.0f, 0.0f);
		mp[1]->cur_pos = vec3(1.0f, 10.0f, 0.0f);
		mp[2]->cur_pos = vec3(2.0f, 10.0f, 0.0f);
		mp[3]->cur_pos = vec3(3.0f, 10.0f, 0.0f);
		mp[4]->cur_pos = vec3(4.0f, 10.0f, 0.0f);

		simulator S(solv, dt);

		// -----------------------------------------
		// -- initialise simulator
		S.add_mesh(m);

		S.set_gravity_acceleration(vec3(0.0f,-9.81f,0.0f));
		// -----------------------------------------

		// execute simulation
		vector<vec3> t0, t1, t2, t3, t4;
		float sim_time = 0.0f;
		while (sim_time <= total_time) {
			t0.push_back(mp[0]->cur_pos);
			t1.push_back(mp[1]->cur_pos);
			t2.push_back(mp[2]->cur_pos);
			t3.push_back(mp[3]->cur_pos);
			t4.push_back(mp[4]->cur_pos);
			S.apply_time_step();
			sim_time += dt;
		}

		if (output == "none") {
			if (print) {
				cout.setf(ios::fixed);
				cout.precision(4);

				// only in plain text
				cout << "Particle 0" << endl;
				for (size_t i = 0; i < t0.size(); ++i) {
					const vec3& v = t0[i];
					cout << v.x << "," << v.y << "," << v.z << endl;
				}
				cout << "Particle 1" << endl;
				for (size_t i = 0; i < t1.size(); ++i) {
					const vec3& v = t1[i];
					cout << v.x << "," << v.y << "," << v.z << endl;
				}
				cout << "Particle 2" << endl;
				for (size_t i = 0; i < t2.size(); ++i) {
					const vec3& v = t2[i];
					cout << v.x << "," << v.y << "," << v.z << endl;
				}
			}
		}
		else {
			// store trajectory in output file
			ofstream fout;
			fout.open(output.c_str());
			if (not fout.is_open()) {
				cerr << "Could not open output file '" << output << "'" << endl;
			}
			else {
				fout.setf(ios::fixed);
				fout.precision(4);

				// configuration
				fout << "step-time: " << dt << endl;
				fout << "total-time: " << total_time << endl;

				// first in Geogebra format
				sim_200_print_geogebra_trajectory(t0, fout);
				sim_200_print_geogebra_trajectory(t1, fout);
				sim_200_print_geogebra_trajectory(t2, fout);
				sim_200_print_geogebra_trajectory(t3, fout);
				sim_200_print_geogebra_trajectory(t4, fout);
			}
		}
	}

} // -- namespace study_cases

