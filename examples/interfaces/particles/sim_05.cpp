#include "study_cases.hpp"

// C++ includes
#include <iostream>
#include <memory>
using namespace std;

// glm includes
#include <glm/vec3.hpp>

// render includes
#include <render/triangle_mesh/rendered_triangle_mesh.hpp>
#include <render/shader/shader_helper.hpp>
#include <render/geometry/rplane.hpp>
#include <render/include_gl.hpp>
#include <render/obj_reader.hpp>

// physim includes
#include <physim/initialiser/multisource.hpp>
#include <physim/initialiser/hose.hpp>
#include <physim/geometry/plane.hpp>
#include <physim/math/math.hpp>
using namespace physim;
using namespace particles;
using namespace geom;
using namespace init;

// custom includes
#include "glut_functions.hpp"
#include "conversion_helper.hpp"
using namespace glut_functions;

namespace study_cases {

	void sim_05_make_simulation() {
		initialiser *I = SR.get_simulator().get_initialiser();
		I->set_pos_initialiser(
			[](free_particle *p) {
				float z = -4.0f + p->index*0.8f;
				p->cur_pos = math::vec3(-5.0f,1.0f,z);
			}
		);
		I->set_vel_initialiser(
			[](free_particle *p) {
				p->cur_vel = math::vec3(5.0f,0.0f,0.0f);
			}
		);
		I->set_bounce_initialiser(
			[](free_particle *p) {
				p->bouncing = p->index/10.0f;
			}
		);
		I->set_friction_initialiser(
			[](free_particle *p) {
				p->friction = 0.0f;
			}
		);

		glm::vec3 A( -5.0f, -0.25f,-5.0f);
		glm::vec3 B( -5.0f, -0.25f, 5.0f);
		glm::vec3 C( 20.0f, -0.25f, 5.0f);
		glm::vec3 D( 20.0f, -0.25f,-5.0f);

		rplane *floor = new rplane();
		floor->set_points(A, B, C, D);
		SR.add_geometry(floor);

		plane *pl = new plane(
			math::vec3(0.0f,1.0f,0.0f),
			math::vec3(0.0f,0.0f,0.0f)
		);
		SR.get_simulator().add_geometry(pl);
		SR.get_simulator().add_gravity_acceleration(math::vec3(0.0f,-9.81f,0.0f));
		SR.get_simulator().add_free_particles(11);

		SR.get_box().enlarge_box(glm::vec3(0.0f, 5.0f, 0.0f));
		SR.set_window_dims(iw, ih);
		SR.init_cameras();
	}

	void sim_05_help() {
		glut_functions::help();

		cout << "Simulation 05 description:" << endl;
		cout << endl;
		cout << "This simulation is used to study the effects of the bouncing" << endl;
		cout << "coefficient: some particles with different bouncing coefficient" << endl;
		cout << "value roll on a plane. One of them will stop bouncing as soon as" << endl;
		cout << "it touches the plane, and one will never stop bouncing, all of them" << endl;
		cout << "always moving forward since the friction coefficient is fixed to 1.0." << endl;
		cout << "The others will eventually stop bouncing, the time to do so being" << endl;
		cout << "dependent on that value." << endl;
	}

	void sim_05_reset() {
		SR.clear();
		// copy cameras
		perspective old_p = SR.get_perspective_camera();
		orthogonal old_o = SR.get_orthogonal_camera();

		glm::vec3 VRP = SR.get_VRP();
		float theta = SR.get_theta();
		float psi = SR.get_psi();

		glm::vec3 viewer_pos = SR.get_viewer_pos();
		float yaw = SR.get_yaw();
		float pitch = SR.get_pitch();

		sim_05_make_simulation();

		SR.set_perspective(old_p);
		SR.set_orthogonal(old_o);
		SR.set_VRP(VRP);
		SR.set_theta(theta);
		SR.set_psi(psi);
		SR.set_viewer_pos(viewer_pos);
		SR.set_yaw(yaw);
		SR.set_pitch(pitch);
	}

	void sim_05_regular_keys_keyboard(unsigned char c, int x, int y) {
		regular_keys_keyboard(c, x, y);

		switch (c) {
		case 'h':
			sim_05_help();
			break;
		case 'r':
			sim_05_reset();
			break;
		}
	}

	void sim_05_initGL(int argc, char *argv[]) {
		// ----------------- //
		/* initialise window */
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
		glutInitWindowPosition(50, 25);
		glutInitWindowSize(iw, ih);
		window_id = glutCreateWindow("Particles - Simulation 05");

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

		// ---------------- //
		/* build simulation */
		use_shaders = false;
		sim_05_make_simulation();
	}

	void sim_05(int argc, char *argv[]) {
		sim_05_help();
		sim_05_initGL(argc, argv);

		glutDisplayFunc(glut_functions::refresh);
		glutReshapeFunc(glut_functions::resize);
		glutMouseFunc(glut_functions::mouse_click_event);
		glutPassiveMotionFunc(glut_functions::mouse_movement);
		glutMotionFunc(glut_functions::mouse_drag_event);
		glutSpecialFunc(glut_functions::special_keys_keyboard);
		glutKeyboardFunc(sim_05_regular_keys_keyboard);

		//glutIdleFunc(refresh);
		glutTimerFunc(1000.0f/FPS, glut_functions::timed_refresh, 0);

		glutMainLoop();
	}

} // -- namespace study_cases
