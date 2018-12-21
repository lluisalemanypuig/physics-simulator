// C++ includes
#include <iostream>
#include <memory>
using namespace std;

// glm includes
#include <glm/vec3.hpp>

// render includes
#include <render/geometry/rplane.hpp>
#include <render/triangle_mesh/rendered_triangle_mesh.hpp>
#include <render/include_gl.hpp>
#include <render/obj_reader.hpp>

// physim includes
#include <physim/geometry/plane.hpp>
#include <physim/emitter/sized_emitter.hpp>
#include <physim/particles/sized_particle.hpp>
#include <physim/math/vec3.hpp>
using namespace physim;
using namespace particles;
using namespace geometric;
using namespace emitters;

// custom includes
#include "glut_functions.hpp"
#include "conversion_helper.hpp"
using namespace glut_functions;

namespace study_cases {

	void sim_100_make_simulation() {
		draw_sized_particles_wire = true;
		bgd_color = glm::vec3(0.8f,0.8f,0.8f);

		sized_emitter I;
		I.set_pos_initialiser(
			[&](base_particle *p) {
				p->cur_pos = math::vec3(0.0f,5.0f,0.0f);
			}
		);
		I.set_vel_initialiser(
			[](base_particle *p) {
				p->cur_vel = math::vec3(0.0f,0.0f,0.0f);
			}
		);
		I.set_lifetime_initialiser(
			[&](free_particle *p) { p->lifetime = lifetime; }
		);
		I.set_bounce_initialiser(
			[&](free_particle *p) { p->bouncing = bouncing; }
		);
		I.set_friction_initialiser(
			[&](free_particle *p) { p->friction = friction; }
		);
		I.set_radius_initialiser(
			[&](sized_particle *p) { p->R = 1.0f; }
		);
		SR.get_simulator().set_sized_emitter(&I);

		SR.get_simulator().add_sized_particle();

		plane *pl = new plane(
			math::vec3(0.0f,1.0f,0.0f),
			math::vec3(0.0f,0.0f,0.0f)
		);
		SR.get_simulator().add_geometry(pl);
		SR.get_simulator().set_gravity_acceleration(
			math::vec3(0.0f,-9.81f,0.0f)
		);

		rplane *floor = new rplane();
		floor->set_points(
			glm::vec3(-5.0f, -0.05f, -5.0f), glm::vec3(-5.0f, -0.05f,  5.0f),
			glm::vec3( 5.0f, -0.05f,  5.0f), glm::vec3( 5.0f, -0.05f, -5.0f)
		);
		SR.add_geometry(floor);

		SR.get_box().enlarge_box(glm::vec3(0.0f, 6.0f, 0.0f));
		SR.set_window_dims(iw, ih);
		SR.init_cameras();

		wireframe_sphere = new rendered_triangle_mesh();
		OBJ_reader obj;
		obj.load_object("../../interfaces/models", "sphere_fsmooth.obj", *wireframe_sphere);

		if (use_shaders) {
			glut_functions::init_shaders();
			SR.get_box().make_buffers();
			wireframe_sphere->make_buffers();
		}
		else {
			wireframe_sphere->compile();
		}

		n_iterations = 1;
		SR.get_simulator().set_time_step(time_step);
	}

	void sim_100_help() {
		glut_functions::help();

		cout << "Simulation 100 description:" << endl;
		cout << endl;
		cout << "Sized particle bouncing on a plane." << endl;
		cout << endl;
		cout << "Options of this simulation:" << endl;
		cout << "    CTRL + w: activate/deactivate wireframe spheres for sized particles" << endl;
		cout << endl;
	}

	void sim_100_reset() {
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

		sim_100_make_simulation();

		SR.set_perspective(old_p);
		SR.set_orthogonal(old_o);
		SR.set_VRP(VRP);
		SR.set_theta(theta);
		SR.set_psi(psi);
		SR.set_viewer_pos(viewer_pos);
		SR.set_yaw(yaw);
		SR.set_pitch(pitch);
	}

	void sim_100_regular_keys_keyboard(unsigned char c, int x, int y) {
		regular_keys_keyboard(c, x, y);

		switch (c) {
		case 'h':
			sim_100_help();
			break;
		case 'r':
			sim_100_reset();
			break;
		}

		if (GLUT_ACTIVE_CTRL) {
			c = c + 'a' - 1;
			// If 'p' is pressed then c = 1 + actual_char - 'a'
			// So: actual_char = c + 'a' - 1
			switch(c) {
			case 'w':
				draw_sized_particles_wire = not draw_sized_particles_wire;
				break;
			}
		}
	}

	int sim_100_initGL(int argc, char *argv[]) {
		// ----------------- //
		/* initialise window */
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
		glutInitWindowPosition(50, 25);
		glutInitWindowSize(iw, ih);
		window_id = glutCreateWindow("Particles - Simulation 06");

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
		sim_100_make_simulation();
		return 0;
	}

	void sim_100(int argc, char *argv[]) {
		cout << "Simulation 100" << endl;

		sim_100_initGL(argc, argv);
		sim_100_help();

		glutDisplayFunc(glut_functions::refresh);
		glutReshapeFunc(glut_functions::resize);
		glutMouseFunc(glut_functions::mouse_click_event);
		glutPassiveMotionFunc(glut_functions::mouse_movement);
		glutMotionFunc(glut_functions::mouse_drag_event);
		glutSpecialFunc(glut_functions::special_keys_keyboard);
		glutKeyboardFunc(sim_100_regular_keys_keyboard);

		//glutIdleFunc(refresh);
		glutTimerFunc(1000.0f/FPS, glut_functions::timed_refresh, 0);

		glutMainLoop();
	}

} // -- namespace study_cases
