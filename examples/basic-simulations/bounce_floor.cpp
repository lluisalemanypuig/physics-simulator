#include "study_cases.hpp"

namespace study_cases {

	void floor_usage() {
		cout << "floor study case:" << endl;
		cout << endl;
		cout << "This study case is merely a particle bouncing on" << endl;
		cout << "a flat plane, a.k.a. the floor." << endl;
		cout << endl;
		cout << "Options:" << endl;
		cout << endl;
		cout << "    --lifetime t:   the lifetime of the particle" << endl;
		cout << "    --bounce b:     bouncing coefficient of the particle" << endl;
		cout << "    --total-time t: total time of the simulation" << endl;
		cout << "    --step t:       time step of the simulation" << endl;
	}

	void floor(int argc, char *argv[]) {
		float dt = 0.01f;
		float total_time = 2.0f;
		float bounce = 1.0f;
		float lifetime = 2.0f;

		for (int i = 2; i < argc; ++i) {
			if (strcmp(argv[i], "-h") == 0 or strcmp(argv[i], "--help") == 0) {
				floor_usage();
				return;
			}
			else if (strcmp(argv[i], "--lifetime") == 0) {
				lifetime = atof(argv[i + 1]);
				++i;
			}
			else if (strcmp(argv[i], "--bounce") == 0) {
				bounce = atof(argv[i + 1]);
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
			else {
				cerr << "Unknown option '" << string(argv[i]) << "'" << endl;
			}
		}

		simulator S(solver_type::EulerSemi);
		S.set_initialiser(
		[lifetime,bounce](particle *p) {
			p->set_position(vec3(0.0f, 10.0f, 0.0f));
			p->set_lifetime(lifetime);
			p->set_velocity(vec3(0.0f, 0.0f, 0.0f));
			p->set_bouncing(bounce);
		}
		);

		// -----------------------------------------
		// -- initialise simulator

		// the only particle bouncing up and down,
		// initialised using the function.
		const particle *p = S.add_particle();

		plane *floor = new plane(vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f));
		S.add_geometry(floor);
		// -----------------------------------------

		// execute simulation
		timing::time_point begin = timing::now();
		vector<vec3> trajectory;

		while (S.get_current_time() <= total_time) {
			S.apply_time_step(dt);
			vec3 cur_pos = p->get_current_position();
			trajectory.push_back(cur_pos);
		}

		timing::time_point end = timing::now();

		S.clear_geometry();
		S.clear_particles();

		// output result
		cout << "Simulation completed in " << timing::elapsed_seconds(begin, end) << " s" << endl;

		cout.setf(ios::fixed);
		cout.precision(4);
		cout << "{";
		for (uint i = 0; i < trajectory.size(); ++i) {
			const vec3& v = trajectory[i];
			cout << "Point({"
				 << v.x << "," << v.y << "," << v.z
				 << "})";
			if (i < trajectory.size() - 1) {
				cout << ",";
			}
		}
		cout << "}" << endl;
	}

} // -- namespace study_cases
