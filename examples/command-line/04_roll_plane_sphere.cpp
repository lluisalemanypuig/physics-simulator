#include "study_cases.hpp"

// physim includes
#include <physim/initialiser/initialiser.hpp>
#include <physim/particles/free_particle.hpp>
#include <physim/geometry/plane.hpp>
#include <physim/geometry/sphere.hpp>
#include <physim/simulator.hpp>
using namespace physim;
using namespace particles;
using namespace math;
using namespace geom;
using namespace init;

namespace study_cases {

	void roll_plane_sphere_usage() {
		cout << "roll on floor, collide with sphere study case:" << endl;
		cout << endl;
		cout << "This study case consists of a particle rolling down a plane" << endl;
		cout << "and then colliding with a sphere at its feet." << endl;
		cout << endl;
		cout << "Options:" << endl;
		cout << endl;
		cout << "    --lifetime t:   the lifetime of the particle.         Default: 2.0" << endl;
		cout << "    --total-time t: total time of the simulation.         Default: 2.0" << endl;
		cout << "    --step t:       time step of the simulation.          Default: 0.01" << endl;
		cout << "    --bounce b:     bouncing coefficient of the particle. Default: 1.0" << endl;
		cout << "    --friction f:   friction coefficient of the particle. Default: 0.0" << endl;
		cout << "    --initial-z z:  the initial value of the z position of the particle." << endl;
		cout << "                                                          Default: 0.0" << endl;
		cout << endl;
		cout << "    [-o|--output]:  store the particle's trajectory in the specified file." << endl;
	}

	void roll_plane_sphere(int argc, char *argv[]) {
		string output = "none";

		float dt = 0.01f;
		float total_time = 2.0f;
		float lifetime = 2.0f;
		float bounce = 0.8f;
		float friction = 0.2f;
		float iz = 0.0f;

		for (int i = 2; i < argc; ++i) {
			if (strcmp(argv[i], "-h") == 0 or strcmp(argv[i], "--help") == 0) {
				roll_plane_sphere_usage();
				return;
			}
			else if (strcmp(argv[i], "--lifetime") == 0) {
				lifetime = atof(argv[i + 1]);
				++i;
			}
			else if (strcmp(argv[i], "--total-time") == 0) {
				total_time = atof(argv[i + 1]);
				++i;
			}
			else if (strcmp(argv[i], "--step") == 0) {
				dt = atof(argv[i + 1]);
				++i;
			}
			else if (strcmp(argv[i], "--bounce") == 0) {
				bounce = atof(argv[i + 1]);
				++i;
			}
			else if (strcmp(argv[i], "--friction") == 0) {
				friction = atof(argv[i + 1]);
				++i;
			}
			else if (strcmp(argv[i], "--initial-z") == 0) {
				iz = atof(argv[i + 1]);
				++i;
			}
			else if (strcmp(argv[i], "-o") == 0 or strcmp(argv[i], "--output") == 0) {
				output = string(argv[i + 1]);
				++i;
			}
			else {
				cerr << "Unknown option '" << string(argv[i]) << "'" << endl;
			}
		}

		initialiser I;
		I.set_pos_initialiser(
			[&](free_particle *p) {
				p->cur_pos = vec3(-2.0f,4.5f,iz);
			}
		);
		I.set_vel_initialiser(
			[](free_particle *p) {
				p->cur_vel = vec3(0.0f,0.0f,0.0f);
			}
		);
		I.set_lifetime_initialiser(
			[&](free_particle *p) { p->lifetime = lifetime; }
		);
		I.set_bounce_initialiser(
			[&](free_particle *p) { p->bouncing = bounce; }
		);
		I.set_friction_initialiser(
			[&](free_particle *p) { p->friction = friction; }
		);

		simulator S(solver_type::EulerSemi, dt);

		// -----------------------------------------
		// -- initialise simulator
		S.set_initialiser(&I);

		// the only particle bouncing up and down,
		// initialised using the function.
		const free_particle *p = S.add_particle();

		sphere *ball = new sphere(vec3(0.0f,2.0f,0.0f), 1.0f);
		S.add_geometry(ball);

		plane *ramp = new plane(vec3(1.0f,1.0f,0.0f), vec3(0.0f,2.0f,-1.0f));
		S.add_geometry(ramp);

		ramp->display();

		plane *floor = new plane(vec3(0.0f,1.0f,0.0f), vec3(0.0f,0.0f,0.0f));
		S.add_geometry(floor);

		S.add_gravity_acceleration(vec3(0.0f,-9.81f,0.0f));
		// -----------------------------------------

		// execute simulation
		timing::time_point begin = timing::now();
		vector<vec3> trajectory;

		while (S.get_current_time() <= total_time) {
			vec3 cur_pos = p->cur_pos;
			trajectory.push_back(cur_pos);
			S.apply_time_step();
		}

		timing::time_point end = timing::now();

		S.clear_geometry();
		S.clear_particles();

		// output result
		cerr << "Simulation completed in " << timing::elapsed_seconds(begin, end) << " s" << endl;

		if (output == "none") {
			cout.setf(ios::fixed);
			cout.precision(4);

			// only in plain text
			for (size_t i = 0; i < trajectory.size(); ++i) {
				const vec3& v = trajectory[i];
				cout << v.x << "," << v.y << "," << v.z << endl;
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
				fout << "lifetime: " << lifetime << endl;
				fout << "bounce: " << bounce << endl;
				fout << "friction: " << friction << endl;
				fout << "initial-z: " << iz << endl;

				// first in Geogebra format
				fout << "{";
				for (size_t i = 0; i < trajectory.size(); ++i) {
					const vec3& v = trajectory[i];
					fout << "Point({"
						 << v.x << "," << v.y << "," << v.z
						 << "})";
					if (i < trajectory.size() - 1) {
						fout << ",";
					}
				}
				fout << "}" << endl;

				// then in plain text
				for (size_t i = 0; i < trajectory.size(); ++i) {
					const vec3& v = trajectory[i];
					fout << v.x << "," << v.y << "," << v.z << endl;
				}
				fout.close();
			}
		}
	}

} // -- namespace study_cases

