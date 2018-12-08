#include "study_cases.hpp"

// C++ includes
#include <iostream>
#include <memory>
using namespace std;

// glm includes
#include <glm/vec3.hpp>

// render includes
#include <render/geometry/rplane.hpp>
#include <render/geometry/rsphere.hpp>
#include <render/triangle_mesh/rendered_triangle_mesh.hpp>
#include <render/shader/shader_helper.hpp>
#include <render/include_gl.hpp>
#include <render/obj_reader.hpp>

// physim includes
#include <physim/geometry/plane.hpp>
#include <physim/geometry/rectangle.hpp>
#include <physim/geometry/sphere.hpp>
#include <physim/initialiser/rect_shower.hpp>
#include <physim/math/math.hpp>
using namespace physim;
using namespace particles;
using namespace geometry;
using namespace init;

// custom includes
#include "glut_functions.hpp"
#include "conversion_helper.hpp"
using namespace glut_functions;

namespace study_cases {

	typedef math::vec3 pm_vec3;

	void sim_10_initialise_sim() {
		plane *wall1 = new plane( pm_vec3( 1.0f, 0.0f,  0.0f), pm_vec3( 0.0f, 0.0f,  0.0f) );
		plane *wall2 = new plane( pm_vec3(-1.0f, 0.0f,  0.0f), pm_vec3(20.0f, 0.0f,  0.0f) );
		plane *wall3 = new plane( pm_vec3( 0.0f, 0.0f,  1.0f), pm_vec3( 0.0f, 0.0f,  0.0f) );
		plane *wall4 = new plane( pm_vec3( 0.0f, 0.0f, -1.0f), pm_vec3( 0.0f, 0.0f, 20.0f) );
		SR.get_simulator().add_geometry(wall1);
		SR.get_simulator().add_geometry(wall2);
		SR.get_simulator().add_geometry(wall3);
		SR.get_simulator().add_geometry(wall4);

		sized_particle *p1 = new sized_particle();
		sized_particle *p2 = new sized_particle();

		p1->lifetime = 99999999.0f;
		p1->bouncing = 1.0f;
		p1->friction = 0.0f;

		p2->lifetime = 99999999.0f;
		p2->bouncing = 1.0f;
		p2->friction = 0.0f;

		p1->cur_pos = math::vec3(2.3f, 0.0f, 2.0f);
		p1->cur_vel = math::vec3(0.8f, 0.0f, 1.0f);
		p1->R = 1.0f;

		p2->cur_pos = math::vec3(18.2f, 0.0f, 18.0f);
		p2->cur_vel = math::vec3(-1.5f, 0.0f, -1.0f);
		p2->R = 1.0f;

		SR.get_simulator().set_particle_particle_collisions(true);
		SR.get_simulator().set_viscous_drag(0.0f);

		SR.get_simulator().add_sized_particle(p1);
		SR.get_simulator().add_sized_particle(p2);
	}

	void sim_10_initialise_rend() {
		glm::vec3 A(  0.0f,  0.0f,  0.0f);
		glm::vec3 B(  0.0f,  5.0f,  0.0f);
		glm::vec3 C(  0.0f,  0.0f, 20.0f);
		glm::vec3 D(  0.0f,  5.0f, 20.0f);
		glm::vec3 E( 20.0f,  0.0f, 20.0f);
		glm::vec3 F( 20.0f,  5.0f, 20.0f);
		glm::vec3 G( 20.0f,  0.0f,  0.0f);
		glm::vec3 H( 20.0f,  5.0f,  0.0f);

		rplane *wall1 = new rplane();
		wall1->set_points(A,B,D,C);
		wall1->set_color(0.8f,0.0f,0.2f,1.0f);
		rplane *wall2 = new rplane();
		wall2->set_points(C,D,F,E);
		wall2->set_color(0.8f,0.0f,0.2f,1.0f);
		rplane *wall3 = new rplane();
		wall3->set_points(E,F,H,G);
		wall3->set_color(0.8f,0.0f,0.2f,1.0f);
		rplane *wall4 = new rplane();
		wall4->set_points(G,H,B,A);
		wall4->set_color(0.8f,0.0f,0.2f,1.0f);

		SR.add_geometry(wall1);
		SR.add_geometry(wall2);
		SR.add_geometry(wall3);
		SR.add_geometry(wall4);
	}

	void sim_10_make_simulation() {
		n_iterations = 10;
		time_step = 0.01f;
		SR.get_simulator().set_time_step(time_step);

		draw_sized_particles_wire = true;
		bgd_color = glm::vec3(0.8f,0.8f,0.8f);

		sim_10_initialise_sim();
		sim_10_initialise_rend();

		SR.get_box().enlarge_box(glm::vec3(0.0f, 12.0f, 0.0f));
		SR.set_window_dims(iw, ih);
		SR.init_cameras();

		wireframe_sphere = new rendered_triangle_mesh();
		OBJ_reader obj;
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
	}

	void sim_10_help() {
		glut_functions::help();

		cout << "Simulation 10 description:" << endl;
		cout << endl;
		cout << "PENDING" << endl;
		cout << endl;
	}

	void sim_10_reset() {
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

		use_shaders = false;
		sim_10_make_simulation();

		SR.set_perspective(old_p);
		SR.set_orthogonal(old_o);
		SR.set_VRP(VRP);
		SR.set_theta(theta);
		SR.set_psi(psi);
		SR.set_viewer_pos(viewer_pos);
		SR.set_yaw(yaw);
		SR.set_pitch(pitch);
	}

	void sim_10_regular_keys_keyboard(unsigned char c, int x, int y) {
		regular_keys_keyboard(c, x, y);

		switch (c) {
		case 'h':
			sim_10_help();
			break;
		case 'r':
			sim_10_reset();
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

	int sim_10_initGL(int argc, char *argv[]) {
		// ----------------- //
		/* initialise window */
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
		glutInitWindowPosition(50, 25);
		glutInitWindowSize(iw, ih);
		window_id = glutCreateWindow("Particles - Simulation 10");

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
		sim_10_make_simulation();
		return 0;
	}

	void sim_10(int argc, char *argv[]) {
		sim_10_help();
		sim_10_initGL(argc, argv);

		glutDisplayFunc(glut_functions::refresh);
		glutReshapeFunc(glut_functions::resize);
		glutMouseFunc(glut_functions::mouse_click_event);
		glutPassiveMotionFunc(glut_functions::mouse_movement);
		glutMotionFunc(glut_functions::mouse_drag_event);
		glutSpecialFunc(glut_functions::special_keys_keyboard);
		glutKeyboardFunc(sim_10_regular_keys_keyboard);

		//glutIdleFunc(refresh);
		glutTimerFunc(1000.0f/FPS, glut_functions::timed_refresh, 0);

		glutMainLoop();
	}

} // -- namespace study_cases
