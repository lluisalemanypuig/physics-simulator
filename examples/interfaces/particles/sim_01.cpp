// C++ includes
#include <iostream>
#include <memory>
using namespace std;

// glm includes
#include <glm/vec3.hpp>

// render includes
#include <render/geometry/rrectangle.hpp>
#include <render/geometry/rtriangle.hpp>
#include <render/geometry/rplane.hpp>
#include <render/include_gl.hpp>

// physim includes
#include <physim/geometry/plane.hpp>
#include <physim/geometry/rectangle.hpp>
#include <physim/geometry/triangle.hpp>
#include <physim/initialiser/rect_shower.hpp>
#include <physim/math/vec3.hpp>
using namespace physim;
using namespace particles;
using namespace geometric;
using namespace init;

// custom includes
#include "glut_functions.hpp"
#include "conversion_helper.hpp"
using namespace glut_functions;

namespace study_cases {

	void sim_01_make_simulation() {
		rect_shower w;
		w.set_straight_source(math::vec3(-4.5f,10.0f,-1.5f), 3.0f,3.0f);
		w.set_starttime_initialiser(
			[](free_particle *p) {
				p->starttime = p->index/300.0f;
			}
		);
		w.set_lifetime_initialiser(
			[&](free_particle *p) { p->lifetime = lifetime; }
		);
		w.set_bounce_initialiser(
			[&](free_particle *p) { p->bouncing = bouncing; }
		);
		w.set_friction_initialiser(
			[&](free_particle *p) { p->lifetime = friction; }
		);
		SR.get_simulator().set_initialiser(&w);

		rrectangle *ramp = new rrectangle();
		ramp->set_points(
			glm::vec3(-2.0f, 2.0f,  1.0f), glm::vec3(-2.0f, 2.0f, -1.0f),
			glm::vec3(-4.0f, 4.0f, -1.0f), glm::vec3(-4.0f, 4.0f,  1.0f)
		);
		ramp->set_color(1.0f, 1.0f, 0.0f, 1.0f);

		rtriangle *bouncer = new rtriangle();
		bouncer->set_points(
			glm::vec3( 0.0f, 2.0f,  1.0f), glm::vec3( 0.0f, 2.0f, -1.0f),
			glm::vec3( 2.0f, 4.0f,  0.0f)
		);
		bouncer->set_color(1.0f, 0.0f, 1.0f, 1.0f);

		rplane *floor = new rplane();
		floor->set_points(
			glm::vec3(-5.0f, -0.05f, -5.0f), glm::vec3(-5.0f, -0.05f,  5.0f),
			glm::vec3( 5.0f, -0.05f,  5.0f), glm::vec3( 5.0f, -0.05f, -5.0f)
		);

		SR.add_geometry(ramp);
		SR.add_geometry(bouncer);
		SR.add_geometry(floor);

		rectangle *rl = new rectangle(
			to_physim(ramp->p1()), to_physim(ramp->p2()),
			to_physim(ramp->p3()), to_physim(ramp->p4())
		);
		triangle *tl = new triangle(
			to_physim(bouncer->p1()), to_physim(bouncer->p2()),
			to_physim(bouncer->p3())
		);
		plane *pl = new plane(
			math::vec3(0.0f,1.0f,0.0f),
			math::vec3(0.0f,0.0f,0.0f)
		);

		SR.get_simulator().add_geometry(rl);
		SR.get_simulator().add_geometry(tl);
		SR.get_simulator().add_geometry(pl);

		SR.get_simulator().set_gravity_acceleration(math::vec3(0.0f,-9.81f,0.0f));

		SR.get_simulator().add_free_particles(300);

		SR.get_box().enlarge_box(glm::vec3(0.0f, 12.0f, 0.0f));
		SR.set_window_dims(iw, ih);
		SR.init_cameras();

		n_iterations = 1;
		SR.get_simulator().set_time_step(time_step);
	}

	void sim_01_help() {
		glut_functions::help();

		cout << "Simulation 01 description:" << endl;
		cout << endl;
		cout << "Exactly 300 generated with a rectangular source. Some of" << endl;
		cout << "these particles will bounce on top a rectangle placed beneath" << endl;
		cout << "the source, and some others will not - these will bounce" << endl;
		cout << "on top of the plane underneath. The particles that collide" << endl;
		cout << "with the rectangle will bounce on it and move towards a triangle." << endl;
		cout << "Again, some particles will skip the triangle and some others" << endl;
		cout << "will not." << endl;
		cout << endl;
	}

	void sim_01_reset() {
		clear_simulation();
		if (use_shaders) {
			clear_shaders();
		}

		// copy cameras
		perspective old_p = SR.get_perspective_camera();
		orthogonal old_o = SR.get_orthogonal_camera();

		glm::vec3 VRP = SR.get_VRP();
		float theta = SR.get_theta();
		float psi = SR.get_psi();

		glm::vec3 viewer_pos = SR.get_viewer_pos();
		float yaw = SR.get_yaw();
		float pitch = SR.get_pitch();

		sim_01_make_simulation();

		SR.set_perspective(old_p);
		SR.set_orthogonal(old_o);
		SR.set_VRP(VRP);
		SR.set_theta(theta);
		SR.set_psi(psi);
		SR.set_viewer_pos(viewer_pos);
		SR.set_yaw(yaw);
		SR.set_pitch(pitch);
	}

	void sim_01_regular_keys_keyboard(unsigned char c, int x, int y) {
		regular_keys_keyboard(c, x, y);

		switch (c) {
		case 'h':
			sim_01_help();
			break;
		case 'r':
			sim_01_reset();
			break;
		}
	}

	void sim_01_initGL(int argc, char *argv[]) {
		// ----------------- //
		/* initialise window */
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
		glutInitWindowPosition(50, 25);
		glutInitWindowSize(iw, ih);
		window_id = glutCreateWindow("Particles - Simulation 01");

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
		glut_functions::init_glut_variables();
		glut_functions::parse_common_params(argc, argv);
		use_shaders = false;

		// ---------------- //
		/* build simulation */
		sim_01_make_simulation();
	}

	void sim_01(int argc, char *argv[]) {
		sim_01_initGL(argc, argv);
		sim_01_help();

		glutDisplayFunc(glut_functions::refresh);
		glutReshapeFunc(glut_functions::resize);
		glutMouseFunc(glut_functions::mouse_click_event);
		glutPassiveMotionFunc(glut_functions::mouse_movement);
		glutMotionFunc(glut_functions::mouse_drag_event);
		glutSpecialFunc(glut_functions::special_keys_keyboard);
		glutKeyboardFunc(sim_01_regular_keys_keyboard);

		//glutIdleFunc(refresh);
		glutTimerFunc(1000.0f/FPS, glut_functions::timed_refresh, 0);

		glutMainLoop();
	}

} // -- namespace study_cases
