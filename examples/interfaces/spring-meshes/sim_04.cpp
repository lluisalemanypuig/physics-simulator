// C includes
#include <string.h>

// C++ includes
#include <iostream>
using namespace std;

// glm includes
#include <glm/vec3.hpp>

// render includes
#include <render/triangle_mesh/rendered_triangle_mesh.hpp>
#include <render/geometry/robject.hpp>
#include <render/shader/shader_helper.hpp>
#include <render/include_gl.hpp>
#include <render/obj_reader.hpp>

// physim includes
#include <physim/meshes/mesh2d_regular.hpp>
#include <physim/geometry/object.hpp>
#include <physim/input/input.hpp>
#include <physim/math/vec3.hpp>
using namespace physim;
using namespace particles;
using namespace meshes;
using namespace math;
using namespace geometric;

// custom includes
#include "glut_functions.hpp"
#include "glut_variables.hpp"
#include "conversion_helper.hpp"
using namespace glut_functions;
using namespace glut_variables;

namespace study_cases {

	static float mesh_mass;
	static size_t n, m;

	void sim_04_make_simulation() {
		SR.set_particle_size(2.0f);
		SR.set_spring_width(1.0f);

		SR.get_simulator().set_solver(solver);
		SR.get_simulator().set_gravity_acceleration(vec3(0.0f,-9.81f,0.0f));

		float length = 1.0f;
		float height = 1.0f;

		// build regular mesh
		mesh2d_regular *M = new mesh2d_regular();
		M->simulate_bend(glut_variables::bend);
		M->simulate_shear(glut_variables::shear);
		M->simulate_stretch(glut_variables::stretch);

		M->allocate(n*m, mesh_mass);
		M->set_dimensions(n, m);

		M->set_elasticity(elasticity);
		M->set_damping(damping);

		mesh_particle *mp = M->get_particles();

		shared_ptr<rendered_triangle_mesh> model_pipe(new rendered_triangle_mesh);
		OBJ_reader obj;
		obj.load_object("../../interfaces/models", "monkey-artistic.obj", *model_pipe);

		robject *ro = new robject();
		ro->set_model(model_pipe);
		SR.add_geometry(ro);

		box B;
		ro->make_box(B);
		float mx = (B.get_max().x + B.get_min().x)/2.0f;
		float lx = B.get_max().x - B.get_min().x;
		float mz = (B.get_max().z + B.get_min().z)/2.0f;
		float lz = B.get_max().z - B.get_min().z;

		// make positions
		for (size_t i = 0; i < n; ++i) {
			for (size_t j = 0; j < m; ++j) {
				mp[ M->get_global_index(i,j) ].cur_pos =
					vec3(
						mx - lx/2.0f + (length/n)*i,
						1.0f,
						mz - lz/2.0f + (height/m)*j);
			}
		}

		object *O = new object();
		input::read_file("../../interfaces/models", "monkey-geometric.obj", O);
		SR.get_simulator().add_geometry(O);

		SR.get_simulator().add_mesh(M);
		SR.get_simulator().set_time_step(0.001f);

		SR.set_window_dims(window_width, window_height);
		SR.init_cameras();

		vector<pair<vec3, vec3> > pmboxes;
		O->get_partition().get_boxes(pmboxes);
		ro->set_boxes(pmboxes);

		model_pipe->load_textures();
		init_shaders();

		SR.get_box().make_buffers();
		ro->make_boxes_buffers();

		model_pipe->make_buffers_materials_textures();
		shader& ts = texture_shader;
		ts.bind();
		shader_helper::activate_materials_textures(*model_pipe, ts);
		ts.release();

		cout << "Initialised simulation 02:" << endl;
		cout << "    mesh mass: " << mesh_mass << endl;
		cout << "    Ke: " << elasticity << endl;
		cout << "    Kd: " << damping << endl;
		cout << "    stretch? " << (stretch ? "Yes" : "No") << endl;
		cout << "    shear? " << (shear ? "Yes" : "No") << endl;
		cout << "    bend? " << (bend ? "Yes" : "No") << endl;
		cout << "    dimensions: " << n << "x" << m << endl;
		cout << "    solver: ";
		if (solver == physim::solver_type::EulerOrig) {
			cout << "Euler explicit" << endl;
		}
		else if (solver == physim::solver_type::EulerSemi) {
			cout << "Euler semi-implicit" << endl;
		}
		else if (solver == physim::solver_type::Verlet) {
			cout << "Verlet" << endl;
		}
	}

	void sim_04_help() {
		help();

		cout << "Simulation 04 description:" << endl;
		cout << endl;
		cout << "    This simulation features a single 2d mesh falling on a" << endl;
		cout << "    geometrical object." << endl;
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
		cout << "        Default: 1 Kg" << endl;
		cout << "    --ke e, --kd d : set elasticity and damping parameters of the mesh" << endl;
		cout << "        Default: Ke=150, Kd=0.5" << endl;
		cout << "    --n #, --m # : number of particles in the x and z dimensions." << endl;
		cout << "        Default: 25x25" << endl;
		cout << "    --solver S: numerical solver used." << endl;
		cout << "        Either 'exp-euler', 'semi-euler', 'verlet'" << endl;
		cout << "        Default: Verlet" << endl;
		cout << endl;
	}

	void sim_04_reset() {
		SR.clear();
		clear_shaders();

		// copy cameras
		perspective old_p = SR.get_perspective_camera();
		orthogonal old_o = SR.get_orthogonal_camera();

		glm::vec3 VRP = SR.get_VRP();
		float theta = SR.get_theta();
		float psi = SR.get_psi();

		glm::vec3 viewer_pos = SR.get_viewer_pos();
		float yaw = SR.get_yaw();
		float pitch = SR.get_pitch();

		// remake simulations
		sim_04_make_simulation();

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

	void sim_04_regular_keys_keyboard(unsigned char c, int x, int y) {
		regular_keys_keyboard(c, x, y);

		switch (c) {
		case 'h':
			sim_04_help();
			break;
		case 'r':
			sim_04_reset();
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
				sim_04_reset();
				break;
			}
		}
	}

	void sim_04_initGL(int argc, char *argv[]) {
		// ----------------- //
		/* initialise window */
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
		glutInitWindowPosition(50, 25);
		glutInitWindowSize(window_width, window_height);
		window_id = glutCreateWindow("Spring meshes - Simulation 04");

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

		GLenum err = glewInit();
		if (err != 0) {
			cerr << "initGL - Error:" << endl;
			cerr << "    when initialising glew: " << err << endl;
			exit(1);
		}

		// --------------------------- //
		/* initialise global variables */
		init_glut_variables();

		stretch = true;
		shear = false;
		bend = false;

		elasticity = 100.0f;
		damping = 0.5f;

		draw_boxes_octree = true;

		n = 25;
		m = 25;
		mesh_mass = 5.0f;

		parse_common_params(argc, argv);
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
		sim_04_make_simulation();
	}

	void sim_04(int argc, char *argv[]) {
		sim_04_initGL(argc, argv);
		sim_04_help();

		glutDisplayFunc(refresh);
		glutReshapeFunc(resize);
		glutMouseFunc(mouse_click_event);
		glutPassiveMotionFunc(mouse_movement);
		glutMotionFunc(mouse_drag_event);
		glutSpecialFunc(special_keys_keyboard);
		glutKeyboardFunc(sim_04_regular_keys_keyboard);

		//glutIdleFunc(refresh);
		glutTimerFunc(1000.0f/FPS, timed_refresh, 0);

		glutMainLoop();
	}

} // -- namespace study_cases
