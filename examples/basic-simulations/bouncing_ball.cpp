#include "study_cases.hpp"

namespace study_cases {

	void bouncing_ball() {
		// time step
		const float dt = 0.01;

		// the only particle bouncing up and down
		particle *p = new particle();
		p->set_position(vec3(0.0f, 5.0f, 0.0f));

		simulator S;
		S.add_particle(p);

		vec3 prev_pos, cur_pos;

		bool exit = false;
		while (not exit) {
			prev_pos = p->get_current_position();
			S.apply_time_step(dt);
			cur_pos = p->get_current_position();

			cout << "At time: " << S.get_current_time() << endl;
			cout << "    particle has position ("
				 << cur_pos.x << ", " << cur_pos.y << ", " << cur_pos.z << ")"
				 << endl;

			if (prev_pos.y*prev_pos.y < 0) {
				// there has been a bounce
				cout << "    BOUNCE! Exit? (y/n)";
				char k;
				cin >> k;
				exit = (k == 'y');
			}
		}
	}

} // -- namespace study_cases
