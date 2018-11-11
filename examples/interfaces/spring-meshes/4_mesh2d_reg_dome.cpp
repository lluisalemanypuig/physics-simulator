#include "study_cases.hpp"

// C includes
#include <string.h>

// C++ includes
#include <iostream>
using namespace std;

// base includes
#include <base/include_gl.hpp>
#include <base/obj_reader.hpp>
#include <base/model/rendered_model.hpp>
#include <base/rgeom/rendered_geometry.hpp>

// physim includes
#include <physim/meshes/mesh2d_regular.hpp>
#include <physim/geometry/plane.hpp>
#include <physim/geometry/rectangle.hpp>
#include <physim/geometry/sphere.hpp>
#include <physim/math/math.hpp>
using namespace physim;
using namespace particles;
using namespace meshes;
using namespace math;
using namespace geom;

// custom includes
#include "glut_functions.hpp"
using namespace glut_functions;

namespace study_cases {

	static float mesh_mass;
	static size_t n, m;

	void sim4_make_simulation() {
		SR.set_particle_size(2.0f);
		SR.set_spring_width(1.0f);

		SR.get_simulator().set_solver(glut_functions::solver);
		SR.get_simulator().add_gravity_acceleration(vec3(0.0f,-9.81f,0.0f));

		float length = 10.0f;
		float height = 10.0f;
		float R = (1.0f/2.0f)*std::sqrt(200.0f) + 0.2f;

		// add plane
		plane *p = new plane(vec3(0.0f,1.0f,0.0f), vec3(0.0f,0.0f,0.0f));
		SR.get_simulator().add_geometry(p);
		rplane *rp = new rplane();
		rp->p1 = vec3(-5.0f, 0.0f, -5.0f);
		rp->p2 = vec3(-5.0f, 0.0f, 15.0f);
		rp->p3 = vec3(15.0f, 0.0f, 15.0f);
		rp->p4 = vec3(15.0f, 0.0f, -5.0f);
		SR.add_geometry(rp);

		// build regular mesh
		mesh2d_regular *M = new mesh2d_regular();
		M->simulate_bend(glut_functions::bend);
		M->simulate_shear(glut_functions::shear);
		M->simulate_stretch(glut_functions::stretch);

		M->allocate(n*m, mesh_mass);
		M->set_dimensions(n, m);

		M->set_elasticity(glut_functions::elasticity);
		M->set_damping(glut_functions::damping);

		mesh_particle **mp = M->get_particles();
		// make positions
		for (size_t i = 0; i < n; ++i) {
			for (size_t j = 0; j < m; ++j) {
				float x = (length/n)*i;
				float z = (height/m)*j;
				float y2 = R*R - (x - 5.0f)*(x - 5.0f) - (z - 5.0f)*(z - 5.0f);

				mp[ M->get_global_index(i,j) ]->cur_pos =
					vec3(x, 5.0f + std::sqrt(y2), z);
			}
		}

		SR.get_simulator().add_mesh(M);

		SR.get_box().enlarge_box(vec3(-5,-5,-5));
		SR.get_box().enlarge_box(vec3(15,10,15));

		SR.set_window_dims(iw, ih);
		SR.init_cameras();

		cout << "Initialised simulation 4:" << endl;
		cout << "    mesh mass: " << mesh_mass << endl;
		cout << "    Ke: " << glut_functions::elasticity << endl;
		cout << "    Kd: " << glut_functions::damping << endl;
		cout << "    stretch? " << (stretch ? "Yes" : "No") << endl;
		cout << "    shear? " << (shear ? "Yes" : "No") << endl;
		cout << "    bend? " << (bend ? "Yes" : "No") << endl;
		cout << "    dimensions: " << n << "x" << m << endl;
		cout << "    solver: ";
		if (glut_functions::solver == physim::solver_type::EulerOrig) {
			cout << "Euler explicit" << endl;
		}
		else if (glut_functions::solver == physim::solver_type::EulerSemi) {
			cout << "Euler semi-implicit" << endl;
		}
		else if (glut_functions::solver == physim::solver_type::Verlet) {
			cout << "Verlet" << endl;
		}
	}

	void sim4_help() {
		glut_functions::help();

		cout << "Simulation 4 description:" << endl;
		cout << endl;
		cout << "    This simulation features a single 2d mesh that is dome-shaped." << endl;
		cout << "    The aim of this simualtion is to show how the forces and elasticity" << endl;
		cout << "    parameters of the springs can mantain a mesh's shape." << endl;
		cout << endl;
		cout << "    This simulation has options of its own:" << endl;
		cout << "    SHIFT + m: change some of the mesh's characteristics." << endl;
		cout << "        A message will be displayed 'Enter option: '" << endl;
		cout << "        Then, write one of the following options and its parameters:" << endl;
		cout << "            mass m: change mass of the mesh. The mass is divided uniformly" << endl;
		cout << "            among its particles." << endl;
		cout << "                m is a floating point value. Default: 50" << endl;
		cout << endl;
		cout << "    This simulation allows to set initial values to its parameters" << endl;
		cout << "    --stretch, --shear, --bend: activate bend, shear, and stretch forces" << endl;
		cout << "        Default: stretch=yes, shear,bend=no" << endl;
		cout << "    --mass m : set mesh's mass to m" << endl;
		cout << "        Default: 50 Kg" << endl;
		cout << "    --ke e, --kd d : set elasticity and damping parameters of the mesh" << endl;
		cout << "        Default: Ke=150, Kd=0.5" << endl;
		cout << "    --n #, --m # : number of particles in the x and z dimensions." << endl;
		cout << "        Default: 25x25" << endl;
		cout << "    --solver S: numerical solver used." << endl;
		cout << "        Either 'exp-euler', 'semi-euler', 'verlet'" << endl;
		cout << "        Default: Verlet" << endl;
		cout << endl;
	}

	void sim4_reset() {
		SR.clear();
		// copy cameras
		perspective old_p = SR.get_perspective_camera();
		orthogonal old_o = SR.get_orthogonal_camera();

		vec3 VRP = SR.get_VRP();
		float theta = SR.get_theta();
		float psi = SR.get_psi();

		vec3 viewer_pos = SR.get_viewer_pos();
		float yaw = SR.get_yaw();
		float pitch = SR.get_pitch();

		// remake simulations
		sim4_make_simulation();

		// reset cameras
		SR.set_perspective(old_p);
		SR.set_orthogonal(old_o);
		SR.set_VRP(VRP);
		SR.set_theta(theta);
		SR.set_psi(psi);
		SR.set_viewer_pos(viewer_pos);
		SR.set_yaw(yaw);
		SR.set_pitch(pitch);
	}

	void sim4_regular_keys_keyboard(unsigned char c, int x, int y) {
		regular_keys_keyboard(c, x, y);

		switch (c) {
		case 'h':
			sim4_help();
			break;
		case 'r':
			sim4_reset();
			break;
		}

		if (special_key == SHIFT_KEY) {
			string option;
			switch (c) {
			case 'M':
				cout << "What do you want to change?" << endl;
				cout << "    mass" << endl;
				cout << "    ";
				cin >> option;
				if (option == "mass") {
					cout << "    enter mass: ";
					cin >> mesh_mass;
					SR.get_simulator().get_meshes()[0]->set_mass(mesh_mass);
				}
				break;
			}
		}
		else if (special_key == CTRL_KEY) {
			int num_val = int(c) + 96;
			switch (num_val) {
			case 'd':
				cout << "Enter dimensions (two numbers: x and z): ";
				cin >> n >> m;
				sim4_reset();
				break;
			}
		}
	}

	void sim4_initGL(int argc, char *argv[]) {
		// ----------------- //
		/* initialise window */
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
		glutInitWindowPosition(50, 25);
		glutInitWindowSize(iw, ih);
		window_id = glutCreateWindow("Spring meshes - simulation 4");

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_NORMALIZE);
		glEnable(GL_LIGHTING);

		glEnable(GL_LIGHT0);
		float col[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glLightfv(GL_LIGHT0, GL_DIFFUSE, col);
		float pos[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightfv(GL_LIGHT0, GL_POSITION, pos);
		float amb[] = {0.2f, 0.2f, 0.2f, 1.0f};
		glLightfv(GL_LIGHT0, GL_AMBIENT, amb);

		// --------------------------- //
		/* initialise global variables */
		glut_functions::init_glut_variables();

		glut_functions::stretch = true;
		glut_functions::shear = false;
		glut_functions::bend = false;

		glut_functions::elasticity = 100.0f;
		glut_functions::damping = 0.5f;

		n = 25;
		m = 25;
		mesh_mass = 50.0f;

		glut_functions::parse_common_params(argc, argv);
		for (int i = 2; i < argc; ++i) {
			if (strcmp(argv[i], "--n") == 0) {
				n = atoi(argv[i + 1]);
				++i;
			}
			else if (strcmp(argv[i], "--m") == 0) {
				m = atoi(argv[i + 1]);
				++i;
			}
			else if (strcmp(argv[i], "--mass") == 0) {
				mesh_mass = atof(argv[i + 1]);
				++i;
			}
		}

		// ---------------- //
		/* build simulation */
		sim4_make_simulation();
	}

	void sim4_2dmeshes(int argc, char *argv[]) {
		sim4_help();
		sim4_initGL(argc, argv);

		glutDisplayFunc(glut_functions::refresh);
		glutReshapeFunc(glut_functions::resize);
		glutMouseFunc(glut_functions::mouse_click_event);
		glutPassiveMotionFunc(glut_functions::mouse_movement);
		glutMotionFunc(glut_functions::mouse_drag_event);
		glutSpecialFunc(glut_functions::special_keys_keyboard);
		glutKeyboardFunc(sim4_regular_keys_keyboard);

		//glutIdleFunc(refresh);
		glutTimerFunc(1000.0f/FPS, glut_functions::timed_refresh, 0);

		glutMainLoop();
	}

} // -- namespace study_cases

