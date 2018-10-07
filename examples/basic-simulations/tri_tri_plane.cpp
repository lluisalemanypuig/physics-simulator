#include "study_cases.hpp"

namespace study_cases {

	void tri2f_usage() {
		cout << "tri2f study case:" << endl;
		cout << endl;
		cout << "This study case consists of two triangles almost perfectly" << endl;
		cout << "facing each other, and a floor. The particle falls onto the" << endl;
		cout << "first triangle so that it bounces towards the second." << endl;
		cout << endl;
		cout << "Although they are called 'triangles' the parameters allow" << endl;
		cout << "using planes, in order to debug the particle-plane and" << endl;
		cout << "particle-triangle collisions." << endl;
		cout << endl;
		cout << "Either triangle or plane, both geometrical objects have a name," << endl;
		cout << "the first being the 'ramp' and the second the 'bouncer'." << endl;
		cout << endl;
		cout << "Options:" << endl;
		cout << endl;
		cout << "    --lifetime t:   the lifetime of the particle.         Default: 2.0" << endl;
		cout << "    --total-time t: total time of the simulation.         Default: 2.0" << endl;
		cout << "    --step t:       time step of the simulation.          Default: 0.01" << endl;
		cout << "    --bounce b:     bouncing coefficient of the particle. Default: 1.0" << endl;
		cout << "    --friction f:   friction coefficient of the particle. Default: 0.0" << endl;
		cout << endl;
		cout << "    --ramp-plane      : make the simulation use the 'ramp' as a plane.       Default: true" << endl;
		cout << "    --bouncer-plane   : make the simulation use the 'bouncer' as a plane.    Default: true" << endl;
		cout << "    --ramp-triangle   : make the simulation use the 'ramp' as a triangle.    Default: false" << endl;
		cout << "    --bouncer-triangle: make the simulation use the 'bouncer' as a triangle. Default: false" << endl;
		cout << endl;
		cout << "    [-o|--output]:  store the particle's trajectory in the specified file." << endl;
	}

	void make_ramp_plane(simulator& S) {
		vec3 B(-2.0f, 0.0f,  2.0f);
		vec3 C(-2.0f, 0.0f, -2.0f);
		vec3 D( 2.0f, 4.0f,  0.0f);
		plane *ramp = new plane(B, D, C);
		S.add_geometry(ramp);
	}

	void make_ramp_triangle(simulator& S) {
		vec3 B(-2.0f, 0.0f,  2.0f);
		vec3 C(-2.0f, 0.0f, -2.0f);
		vec3 D( 2.0f, 4.0f,  0.0f);
		triangle *ramp = new triangle(B, D, C);
		S.add_geometry(ramp);
	}

	void make_bouncer_plane(simulator& S) {
		vec3 E(-5.0f, 0.0f, -1.5f);
		vec3 F(-5.1f, 0.0f,  1.0f);
		vec3 G(-9.0f, 4.0f, -1.0f);
		plane *bouncer = new plane(E, G, F);
		S.add_geometry(bouncer);
	}

	void make_bouncer_triangle(simulator& S) {
		vec3 E(-5.0f, 0.0f, -1.5f);
		vec3 F(-5.1f, 0.0f,  1.0f);
		vec3 G(-9.0f, 4.0f, -1.0f);
		triangle *bouncer = new triangle(E, G, F);
		S.add_geometry(bouncer);
	}

	void tri_tri_floor(int argc, char *argv[]) {
		string output = "none";

		float dt = 0.01f;
		float total_time = 2.0f;
		float lifetime = 2.0f;
		float bounce = 1.0f;
		float friction = 0.0f;

		bool ramp_plane = true;
		bool bouncer_plane = true;

		for (int i = 2; i < argc; ++i) {
			if (strcmp(argv[i], "-h") == 0 or strcmp(argv[i], "--help") == 0) {
				tri2f_usage();
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
			else if (strcmp(argv[i], "-o") == 0 or strcmp(argv[i], "--output") == 0) {
				output = string(argv[i + 1]);
				++i;
			}
			else if (strcmp(argv[i], "--ramp-plane") == 0) {
				ramp_plane = true;
			}
			else if (strcmp(argv[i], "--bouncer-plane") == 0) {
				bouncer_plane = true;
			}
			else if (strcmp(argv[i], "--ramp-triangle") == 0) {
				ramp_plane = false;
			}
			else if (strcmp(argv[i], "--bouncer-triangle") == 0) {
				bouncer_plane = false;
			}
			else {
				cerr << "Unknown option '" << string(argv[i]) << "'" << endl;
			}
		}

		simulator S(solver_type::EulerSemi);
		S.set_initialiser(
		[lifetime,bounce,friction](particle *p) {
			p->set_lifetime(lifetime);
			p->set_position(vec3(0.0f,10.0f,0.0f));
			p->set_previous_velocity(vec3(0.0f,0.0f,0.0f));
			p->set_velocity(vec3(0.0f,0.0f,0.0f));

			p->set_bouncing(bounce);
			p->set_friction(friction);
			p->set_lifetime(lifetime);
		}
		);

		// -----------------------------------------
		// -- initialise simulator
		const particle *p = S.add_particle();

		if (ramp_plane) {
			make_ramp_plane(S);
		}
		else {
			make_ramp_triangle(S);
		}

		if (bouncer_plane) {
			make_bouncer_plane(S);
		}
		else {
			make_bouncer_triangle(S);
		}

		plane *floor = new plane(vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, 0.0f, 0.0f));
		S.add_geometry(floor);
		// -----------------------------------------

		// execute simulation
		timing::time_point begin = timing::now();
		vector<vec3> trajectory;

		while (S.get_current_time() <= total_time) {
			vec3 cur_pos = p->get_position();
			trajectory.push_back(cur_pos);
			S.apply_time_step(dt);
		}

		timing::time_point end = timing::now();

		S.clear_geometry();
		S.clear_particles();

		// output result
		cout << "Simulation completed in " << timing::elapsed_seconds(begin, end) << " s" << endl;

		cout.setf(ios::fixed);
		cout.precision(4);
		cout << "{";
		for (size_t i = 0; i < trajectory.size(); ++i) {
			const vec3& v = trajectory[i];
			cout << "Point({"
				 << v.x << "," << v.y << "," << v.z
				 << "})";
			if (i < trajectory.size() - 1) {
				cout << ",";
			}
		}
		cout << "}" << endl;

		// store trajectory in output file
		if (output != "none") {
			ofstream fout;
			fout.open(output.c_str());
			if (not fout.is_open()) {
				cerr << "Could not open output file '" << output << "'" << endl;
			}
			else {
				fout << "Configuration:" << endl;
				fout << "    step time: " << dt << endl;
				fout << "    total time: " << total_time << endl;
				fout << "    lifetime: " << lifetime << endl;
				fout << "    bounce: " << bounce << endl;
				fout << "    friction: " << friction << endl;
				fout << "    ramp plane: " << (ramp_plane ? "Yes" : "No") << endl;
				fout << "    bouncer plane: " << (bouncer_plane ? "Yes" : "No") << endl;
				for (size_t i = 0; i < trajectory.size(); ++i) {
					const vec3& v = trajectory[i];
					fout << v.x << "," << v.y << "," << v.z << endl;
				}
				fout.close();
			}
		}
	}

} // -- namespace study_cases
