#include "study_cases.hpp"

namespace study_cases {

	void hose_usage() {
		cout << "hose, collision with rectangles, triangles, spheres study case:" << endl;
		cout << endl;
		cout << "This study case is meants basically to study performance with" << endl;
		cout << "lots of particles (millions)" << endl;
		cout << endl;
		cout << "Options:" << endl;
		cout << endl;
		cout << "    [-lt, --lifetime] t:    the lifetime of the particle.  Default: 2.0" << endl;
		cout << "    [-tt, --total-time] t:  total time of the simulation.  Default: 2.0" << endl;
		cout << "    [-dt, --step] t:        time step of the simulation.   Default: 0.01" << endl;
		cout << "    --bounce b:      bouncing coefficient of the particle. Default: 1.0" << endl;
		cout << "    --friction f:    friction coefficient of the particle. Default: 0.0" << endl;
		cout << "    --n-particles n: the initial value of the z position of the particle." << endl;
		cout << "                                                           Default: 1000000" << endl;
		cout << "    --solver s:     numerical solver to use.               Default: 'semi-euler'" << endl;
		cout << "        euler:      Euler integration method. Numerically unstable." << endl;
		cout << "        semi-euler: Euler semi-implicit integration method. Numerically stable." << endl;
		cout << "        verlet:     Verlet integration method. Numerically even more stable." << endl;
		cout << endl;
	}

	void hose_case(int argc, char *argv[]) {
		float dt = 0.01f;
		float total_time = 2.0f;
		float lifetime = 2.0f;
		float bounce = 0.8f;
		float friction = 0.2f;
		size_t n_particles = 1000000;
		solver_type solv = solver_type::EulerSemi;

		for (int i = 2; i < argc; ++i) {
			if (strcmp(argv[i], "-h") == 0 or strcmp(argv[i], "--help") == 0) {
				hose_usage();
				return;
			}
			else if (strcmp(argv[i], "-lt") == 0 or strcmp(argv[i], "--lifetime") == 0) {
				lifetime = atof(argv[i + 1]);
				++i;
			}
			else if (strcmp(argv[i], "-tt") == 0 or strcmp(argv[i], "--total-time") == 0) {
				total_time = atof(argv[i + 1]);
				++i;
			}
			else if (strcmp(argv[i], "-dt") == 0 or strcmp(argv[i], "--step") == 0) {
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
			else if (strcmp(argv[i], "--n-particles") == 0) {
				n_particles = atoi(argv[i + 1]);
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
				cerr << "Unknown option '" << string(argv[i]) << "'" << endl;
			}
		}

		simulator SIM(solv, dt);

		// -----------------------------------------
		// -- initialise simulator
		vec3 A(-3.0f, 0.0f,-3.0f);
		vec3 D(-5.0f,-0.5f,-5.0f);
		vec3 E(-5.0f,-0.5f, 5.0f);
		vec3 F( 5.0f,-0.5f,-5.0f);
		vec3 G( 5.0f,-0.5f, 5.0f);
		vec3 H(-5.0f, 5.0f,-5.0f);
		vec3 I(-5.0f, 5.0f, 5.0f);
		vec3 J( 5.0f, 5.0f,-5.0f);
		vec3 K( 5.0f, 5.0f, 5.0f);
		vec3 L( 3.0f, 5.0f, 3.0f);
		vec3 M( 0.0f, 2.0f, 0.0f);
		//vec3 N( 0.0f, 3.0f, 0.0f);
		vec3 O( 0.5f, 3.0f,-1.0f);
		vec3 P(-1.0f, 3.0f, 0.5f);
		vec3 S(-1.5f, 5.0f,-1.5f);

		vec3 hdir = normalise(L - K);

		vec3 hK = K + hdir*0.1f + vec3(0.0f,0.5f,0.0f);
		vec3 hL = L + hdir*0.1f + vec3(0.0f,0.5f,0.0f);

		// displaced hose direction
		vec3 dhdir = hL - hK;

		hose h;
		h.set_hose_source(hK, normalise(dhdir), 0.5f, norm(dhdir));
		h.set_starttime_initialiser(
			[n_particles](free_particle *p) {
				p->starttime = p->index/(float(n_particles));
			}
		);
		h.set_lifetime_initialiser(
			[&](free_particle *p) { p->lifetime = lifetime; }
		);
		h.set_bounce_initialiser(
			[&](free_particle *p) { p->bouncing = bounce; }
		);
		h.set_friction_initialiser(
			[&](free_particle *p) { p->friction = friction; }
		);

		SIM.set_initialiser(&h);

		plane *floor = new plane(vec3(0.0f,1.0f,0.0f), E);
		plane *pl1 = new plane(vec3(0.0f,0.0f,-1.0f), K);
		plane *pl2 = new plane(vec3(-1.0f,0.0f,0.0f), K);
		plane *pl3 = new plane(vec3(0.0f,0.0f,1.0f), J);
		plane *pl4 = new plane(vec3(1.0f,0.0f,0.0f), H);
		SIM.add_geometry(floor);
		SIM.add_geometry(pl1);
		SIM.add_geometry(pl2);
		SIM.add_geometry(pl3);
		SIM.add_geometry(pl4);

		sphere *ball1 = new sphere(M, 1.0f);
		sphere *ball2 = new sphere(A, 1.0f);
		SIM.add_geometry(ball1);
		SIM.add_geometry(ball2);

		triangle *tri = new triangle(S,O,P);
		SIM.add_geometry(tri);

		SIM.add_particles(n_particles);

		SIM.add_gravity_acceleration(vec3(0.0f,-9.81f,0.0f));
		// -----------------------------------------

		// execute simulation
		timing::time_point begin = timing::now();
		while (SIM.get_current_time() <= total_time) {
			SIM.apply_time_step();
		}
		timing::time_point end = timing::now();

		SIM.clear_geometry();
		SIM.clear_particles();

		// output result
		cout << "Simulation completed in " << timing::elapsed_seconds(begin, end) << " s" << endl;
	}

} // -- namespace study_cases

