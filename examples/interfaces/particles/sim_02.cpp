#include "study_cases.hpp"

// C++ includes
#include <iostream>
#include <memory>
using namespace std;

// glm includes
#include <glm/vec3.hpp>

// render includes
#include <render/geometry/rrectangle.hpp>
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
#include <physim/initialiser/initialiser.hpp>
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

	void sim_02_make_simulation() {
		initialiser i;
		i.set_pos_initialiser(
			[&](free_particle *p) {

				size_t idx = p->index;
				float iz = 0.0;
				if (idx == 0)		{ iz = -1.00f; }
				else if (idx == 1)	{ iz = -0.75f; }
				else if (idx == 2)	{ iz = -0.50f; }
				else if (idx == 3)	{ iz = -0.25f; }
				else if (idx == 4)	{ iz =  0.00f; }
				else if (idx == 5)	{ iz =  0.25f; }
				else if (idx == 6)	{ iz =  0.50f; }
				else if (idx == 7)	{ iz =  0.75f; }
				else if (idx == 8)	{ iz =  1.00f; }
				p->cur_pos = math::vec3(-2.0f,4.5f,iz);
			}
		);
		i.set_vel_initialiser(
			[](free_particle *p) {
				p->cur_vel = math::vec3(0.0f,0.0f,0.0f);
			}
		);
		i.set_lifetime_initialiser(
			[&](free_particle *p) { p->lifetime = lifetime; }
		);
		i.set_bounce_initialiser(
			[&](free_particle *p) { p->bouncing = bouncing; }
		);
		i.set_friction_initialiser(
			[&](free_particle *p) { p->lifetime = friction; }
		);
		SR.get_simulator().set_initialiser(&i);

		rplane *floor = new rplane();
		floor->set_points(
			glm::vec3(-5.0f, -0.05f, -5.0f), glm::vec3(-5.0f, -0.05f,  5.0f),
			glm::vec3( 5.0f, -0.05f,  5.0f), glm::vec3( 5.0f, -0.05f, -5.0f)
		);

		rsphere *rball = new rsphere();
		rball->set_center(glm::vec3(0.0f,2.0f,0.0f));
		rball->set_radius(1.0f);

		shared_ptr<rendered_triangle_mesh> sim_ball(new rendered_triangle_mesh);
		OBJ_reader obj;
		obj.load_object("../../interfaces/models", "sphere.obj", *sim_ball);

		rball->set_model(sim_ball);

		rrectangle *ramp = new rrectangle();
		ramp->set_points(
			glm::vec3( 0.0f, 2.0f,  1.0f), glm::vec3( 0.0f, 2.0f, -1.0f),
			glm::vec3(-3.0f, 5.0f, -1.0f), glm::vec3(-3.0f, 5.0f,  1.0f)
		);
		ramp->set_color(0.0f,0.3f,0.0f,1.0f);

		SR.add_geometry(floor);
		SR.add_geometry(rball);
		SR.add_geometry(ramp);

		plane *pl = new plane(
			math::vec3(0.0f,1.0f,0.0f),
			math::vec3(0.0f,0.0f,0.0f)
		);
		sphere *s = new sphere(to_physim(rball->center()), 1.0f);
		rectangle *rl = new rectangle(
			to_physim(ramp->p1()), to_physim(ramp->p2()),
			to_physim(ramp->p3()), to_physim(ramp->p3())
		);
		SR.get_simulator().add_geometry(pl);
		SR.get_simulator().add_geometry(s);
		SR.get_simulator().add_geometry(rl);
		SR.get_simulator().add_gravity_acceleration(math::vec3(0.0f,-9.81f,0.0f));
		SR.get_simulator().add_free_particles(9);

		SR.get_box().enlarge_box(glm::vec3(0.0f, 7.0f, 0.0f));
		SR.set_window_dims(iw, ih);
		SR.init_cameras();

		sim_ball->load_textures();
		if (use_shaders) {
			glut_functions::init_shaders();
			SR.get_box().make_buffers();
			sim_ball->make_buffers_materials_textures();
			texture_shader.bind();
			shader_helper::activate_materials_textures(*sim_ball, texture_shader);
			texture_shader.release();
		}
		else {
			sim_ball->compile();
		}

		n_iterations = 1;
		SR.get_simulator().set_time_step(time_step);
	}

	void sim_02_help() {
		glut_functions::help();

		cout << "Simulation 02 description:" << endl;
		cout << endl;
		cout << "This simulation consists on 9 particles rolling down a" << endl;
		cout << "rectangle until they collide with a sphere at the bottom" << endl;
		cout << "of the triangle. 8 of these particles will collide with the" << endl;
		cout << "sphere in a way that will make them fall off the rectangle." << endl;
		cout << "One of them will bounce on the sphere and roll the rectangle" << endl;
		cout << "back up again. It will repeat the process until it loses its" << endl;
		cout << "'energy'." << endl;
		cout << endl;
	}

	void sim_02_reset() {
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

		sim_02_make_simulation();

		SR.set_perspective(old_p);
		SR.set_orthogonal(old_o);
		SR.set_VRP(VRP);
		SR.set_theta(theta);
		SR.set_psi(psi);
		SR.set_viewer_pos(viewer_pos);
		SR.set_yaw(yaw);
		SR.set_pitch(pitch);
	}

	void sim_02_regular_keys_keyboard(unsigned char c, int x, int y) {
		regular_keys_keyboard(c, x, y);

		switch (c) {
		case 'h':
			sim_02_help();
			break;
		case 'r':
			sim_02_reset();
			break;
		}
	}

	void sim_02_initGL(int argc, char *argv[]) {
		// ----------------- //
		/* initialise window */
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
		glutInitWindowPosition(50, 25);
		glutInitWindowSize(iw, ih);
		window_id = glutCreateWindow("Particles - Simulation 02");

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
		sim_02_make_simulation();
	}

	void sim_02(int argc, char *argv[]) {
		sim_02_help();
		sim_02_initGL(argc, argv);

		glutDisplayFunc(glut_functions::refresh);
		glutReshapeFunc(glut_functions::resize);
		glutMouseFunc(glut_functions::mouse_click_event);
		glutPassiveMotionFunc(glut_functions::mouse_movement);
		glutMotionFunc(glut_functions::mouse_drag_event);
		glutSpecialFunc(glut_functions::special_keys_keyboard);
		glutKeyboardFunc(sim_02_regular_keys_keyboard);

		//glutIdleFunc(refresh);
		glutTimerFunc(1000.0f/FPS, glut_functions::timed_refresh, 0);

		glutMainLoop();
	}

} // -- namespace study_cases
