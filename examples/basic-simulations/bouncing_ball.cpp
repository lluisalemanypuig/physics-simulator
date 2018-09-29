#include "study_cases.hpp"

namespace study_cases {

	void bouncing_ball(int argc, char *argv[]) {
		float dt = 0.01f;
		float total_time = 2.0f;
		float bounce = 1.0f;
		float lifetime = 2.0f;

		for (int i = 1; i < argc; ++i) {
			if (strcmp(argv[i], "--lifetime") == 0) {
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

		vec3 B(-2.0f, 0.0f,  2.0f);
		vec3 C(-2.0f, 0.0f, -2.0f);
		vec3 D( 2.0f, 4.0f,  0.0f);
		triangle *ramp = new triangle(B, D, C);
		S.add_geometry(ramp);

		vec3 E( 1.0f, 0.0f, -2.0f);
		vec3 F( 1.0f, 0.0f,  2.0f);
		vec3 G(-1.0f, 4.0f,  0.0f);
		triangle *bouncer = new triangle(E, G, F);
		S.add_geometry(bouncer);

		cout << "Ramp:" << endl;
		vec3 ramp_normal = ramp->get_plane().get_normal();
		cout << ramp_normal.x << "*x + " << ramp_normal.y << "*y + " << ramp_normal.z << "*z + "
			 << ramp->get_plane().get_constant() << endl;

		cout << "Bouncer:" << endl;
		vec3 bouncer_normal = bouncer->get_plane().get_normal();
		cout << bouncer_normal.x << "*x + " << bouncer_normal.y << "*y + " << bouncer_normal.z << "*z + "
			 << bouncer->get_plane().get_constant() << endl;

		plane *floor = new plane(vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f));
		S.add_geometry(floor);
		// -----------------------------------------

		vector<vec3> trajectory;

		while (S.get_current_time() <= total_time) {
			S.apply_time_step(dt);
			vec3 cur_pos = p->get_current_position();
			trajectory.push_back(cur_pos);

			cout << "Point({"
				 << cur_pos.x << "," << cur_pos.y << "," << cur_pos.z
				 << "})";
		}

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
