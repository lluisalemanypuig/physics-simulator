#include "study_cases.hpp"

namespace study_cases {

	void bouncing_ball() {
		// time step
		const float dt = 0.01;
		const float total_time = 3.0f;

		simulator S(solver_type::EulerSemi);

		// -----------------------------------------
		// -- initialise simulator

		// the only particle bouncing up and down
		particle *p = new particle();
		p->set_position(vec3(0.0f, 5.0f, 0.0f));
		p->set_force( S.get_gravity() );
		S.add_particle(p);


		vec3 D(-2.0f, -1.0f, -2.0f);
		vec3 E( 2.0f,  4.0f,  2.0f);
		vec3 F( 2.0f,  4.0f, -2.0f);
		plane *ramp = new plane(D, E, F);
		S.add_plane(ramp);

		plane *floor = new plane(vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f));
		S.add_plane(floor);
		// -----------------------------------------

		vector<vec3> trajectory;

		while (S.get_current_time() <= total_time) {
			S.apply_time_step(dt);
			vec3 cur_pos = p->get_current_position();
			trajectory.push_back(cur_pos);
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
