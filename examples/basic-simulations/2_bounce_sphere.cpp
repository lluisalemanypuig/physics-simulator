#include "study_cases.hpp"

namespace study_cases {

	void bounce_sphere_usage() {
		cout << "bounce on sphere study case:" << endl;
		cout << endl;
		cout << "This study case is merely a particle that falls on" << endl;
		cout << "a sphere, then bounces away from it." << endl;
		cout << endl;
		cout << "Options:" << endl;
		cout << endl;
		cout << "    --lifetime t:   the lifetime of the particle.         Default: 2.0" << endl;
		cout << "    --total-time t: total time of the simulation.         Default: 2.0" << endl;
		cout << "    --step t:       time step of the simulation.          Default: 0.01" << endl;
		cout << "    --bounce b:     bouncing coefficient of the particle. Default: 1.0" << endl;
		cout << "    --friction f:   friction coefficient of the particle. Default: 0.0" << endl;
		cout << "    --Iial-x x:  the Iial value of the x position of the particle." << endl;
		cout << "                                                          Default: 0.0" << endl;
		cout << endl;
		cout << "    [-o|--output]:  store the particle's trajectory in the specified file." << endl;
	}

	void bounce_on_sphere(int argc, char *argv[]) {
		string output = "none";

		float dt = 0.01f;
		float total_time = 2.0f;
		float lifetime = 2.0f;
		float bounce = 1.0f;
		float friction = 0.0f;
		float ix = 0.0f;

		for (int i = 2; i < argc; ++i) {
			if (strcmp(argv[i], "-h") == 0 or strcmp(argv[i], "--help") == 0) {
				bounce_sphere_usage();
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
			else if (strcmp(argv[i], "--Iial-x") == 0) {
				ix = atof(argv[i + 1]);
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
			[&](particle *p) {
				p->set_position(math::vec3(ix,5.0f,0.0f));
			}
		);
		I.set_vel_initialiser(
			[](particle *p) {
				p->set_velocity(math::vec3(0.0f,0.0f,0.0f));
			}
		);
		I.set_lifetime_initialiser(
			[&](particle *p) { p->set_lifetime(lifetime); }
		);
		I.set_bounce_initialiser(
			[&](particle *p) { p->set_bouncing(bounce); }
		);
		I.set_friction_initialiser(
			[&](particle *p) { p->set_friction(friction); }
		);

		simulator S(solver_type::EulerSemi, dt);

		// -----------------------------------------
		// -- Iialise simulator

		S.set_initialiser(&I);

		// the only particle bouncing up and down,
		// Iialised using the function.
		const particle *p = S.add_particle();

		sphere *ball = new sphere(math::vec3(0.0f,2.0f,0.0f), 1.0f);
		S.add_geometry(ball);

		plane *floor = new plane(math::vec3(0.0f,1.0f,0.0f), math::vec3(0.0f,0.0f,0.0f));
		S.add_geometry(floor);
		// -----------------------------------------

		// execute simulation
		timing::time_point begin = timing::now();
		vector<math::vec3> trajectory;

		while (S.get_current_time() <= total_time) {
			math::vec3 cur_pos = p->get_position();
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

			// only in GeoGebra format
			cout << "{";
			for (size_t i = 0; i < trajectory.size(); ++i) {
				const math::vec3& v = trajectory[i];
				cout << "Point({"
					 << v.x << "," << v.y << "," << v.z
					 << "})";
				if (i < trajectory.size() - 1) {
					cout << ",";
				}
			}
			cout << "}" << endl;
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
				fout << "Iial-x: " << ix << endl;

				// first in Geogebra format
				fout << "{";
				for (size_t i = 0; i < trajectory.size(); ++i) {
					const math::vec3& v = trajectory[i];
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
					const math::vec3& v = trajectory[i];
					fout << v.x << "," << v.y << "," << v.z << endl;
				}
				fout.close();
			}
		}
	}

} // -- namespace study_cases
