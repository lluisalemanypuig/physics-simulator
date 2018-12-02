#include "study_cases.hpp"

// C++ includes
#include <iostream>
#include <memory>
using namespace std;

// glm includes
#include <glm/vec3.hpp>

// render includes
#include <render/geometry/rplane.hpp>
#include <render/geometry/rtriangle.hpp>
#include <render/triangle_mesh/rendered_triangle_mesh.hpp>
#include <render/shader/shader_helper.hpp>
#include <render/include_gl.hpp>
#include <render/obj_reader.hpp>

// physim includes
#include <physim/geometry/plane.hpp>
#include <physim/geometry/triangle.hpp>
#include <physim/initialiser/initialiser.hpp>
#include <physim/particles/sized_particle.hpp>
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

	void sim_08_make_simulation() {
		draw_sized_particles_wire = true;
		bgd_color = glm::vec3(0.8f,0.8f,0.8f);

		initialiser I;
		I.set_pos_initialiser(
			[&](free_particle *p) {
				p->cur_pos = math::vec3(0.0f,5.0f,0.0f);
			}
		);
		I.set_vel_initialiser(
			[](free_particle *p) {
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
		SR.get_simulator().set_initialiser(&I);
		SR.get_simulator().add_sized_particle();

		plane *pl = new plane(
			math::vec3(0.0f,1.0f,0.0f),
			math::vec3(0.0f,0.0f,0.0f)
		);
		SR.get_simulator().add_geometry(pl);
		triangle *tl = new triangle(
			math::vec3( 1.0f,2.0f, 2.0f),
			math::vec3( 1.0f,2.0f,-2.0f),
			math::vec3(-1.0f,2.0f, 0.0f)
		);
		SR.get_simulator().add_geometry(tl);
		SR.get_simulator().add_gravity_acceleration(
			math::vec3(0.0f,-9.81f,0.0f)
		);

		rplane *floor = new rplane();
		floor->set_points(
			glm::vec3(-5.0f, -0.05f, -5.0f), glm::vec3(-5.0f, -0.05f,  5.0f),
			glm::vec3( 5.0f, -0.05f,  5.0f), glm::vec3( 5.0f, -0.05f, -5.0f)
		);
		SR.add_geometry(floor);
		rtriangle *tri = new rtriangle();
		tri->set_points(
			glm::vec3( 1.0f,2.0f, 2.0f),
			glm::vec3( 1.0f,2.0f,-2.0f),
			glm::vec3(-1.0f,2.0f, 0.0f)
		);
		tri->set_color(1.0f, 1.0f, 0.0f, 1.0f);
		SR.add_geometry(tri);

		OBJ_reader obj;

		SR.get_box().enlarge_box(glm::vec3(0.0f, 6.0f, 0.0f));
		SR.set_window_dims(iw, ih);
		SR.init_cameras();

		wireframe_sphere = new rendered_triangle_mesh();
		obj.load_object
		("../../interfaces/models", "sphere_fsmooth.obj", *wireframe_sphere);

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

	void sim_08_help() {
		glut_functions::help();

		cout << "Simulation 07 description:" << endl;
		cout << endl;
		cout << "Sized particle bouncing on a triangle." << endl;
		cout << endl;
		cout << "Options of this simulation:" << endl;
		cout << "    CTRL + w: activate/deactivate wireframe spheres for sized particles" << endl;
		cout << endl;
	}

	void sim_08_reset() {
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

		sim_08_make_simulation();

		SR.set_perspective(old_p);
		SR.set_orthogonal(old_o);
		SR.set_VRP(VRP);
		SR.set_theta(theta);
		SR.set_psi(psi);
		SR.set_viewer_pos(viewer_pos);
		SR.set_yaw(yaw);
		SR.set_pitch(pitch);
	}

	void sim_08_regular_keys_keyboard(unsigned char c, int x, int y) {
		regular_keys_keyboard(c, x, y);

		switch (c) {
		case 'h':
			sim_08_help();
			break;
		case 'r':
			sim_08_reset();
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

	int sim_08_initGL(int argc, char *argv[]) {
		// ----------------- //
		/* initialise window */
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
		glutInitWindowPosition(50, 25);
		glutInitWindowSize(iw, ih);
		window_id = glutCreateWindow("Particles - Simulation 07");

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
		sim_08_make_simulation();
		return 0;
	}

	void sim_08(int argc, char *argv[]) {
		sim_08_help();
		sim_08_initGL(argc, argv);

		glutDisplayFunc(glut_functions::refresh);
		glutReshapeFunc(glut_functions::resize);
		glutMouseFunc(glut_functions::mouse_click_event);
		glutPassiveMotionFunc(glut_functions::mouse_movement);
		glutMotionFunc(glut_functions::mouse_drag_event);
		glutSpecialFunc(glut_functions::special_keys_keyboard);
		glutKeyboardFunc(sim_08_regular_keys_keyboard);

		//glutIdleFunc(refresh);
		glutTimerFunc(1000.0f/FPS, glut_functions::timed_refresh, 0);

		glutMainLoop();
	}

} // -- namespace study_cases
