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
#include <physim/emitter/free_emitters/rect_shower.hpp>
#include <physim/emitter/sized_emitter.hpp>
#include <physim/particles/free_particle.hpp>
#include <physim/particles/sized_particle.hpp>
#include <physim/math/vec3.hpp>
using namespace physim;
using namespace math;
using namespace particles;
using namespace geometric;
using namespace emitters;
using namespace free_emitters;

// custom includes
#include "glut_functions.hpp"
#include "conversion_helper.hpp"
using namespace glut_functions;

namespace study_cases {

	void sim_201_make_simulation_free() {
		// these parts of the emitter_free are scene-dependent

		rect_shower w;
		w.set_straight_source(vec3(-3.0f,10.0f,-3.0f), 6.0f,6.0f);
		w.set_mass_initialiser(
			[](base_particle& p) { p.mass = 1.0f; }
		);
		w.set_starttime_initialiser(
			[](free_particle& p) {
				p.starttime = p.index/1000.0f;
			}
		);
		w.set_charge_initialiser(
			[](free_particle& p) { p.charge = 1.0f; }
		);
		w.set_lifetime_initialiser(
			[&](free_particle& p) { p.lifetime = lifetime; }
		);
		w.set_bounce_initialiser(
			[&](free_particle& p) { p.bouncing = bouncing; }
		);
		w.set_friction_initialiser(
			[&](free_particle& p) { p.friction = friction; }
		);
		SR.get_simulator().set_free_emitter(&w);
		SR.get_simulator().add_free_particles(1000);
		SR.make_free_particle_indices();
	}

	void sim_201_make_simulation_sized() {
		sized_emitter s_em;
		s_em.set_pos_initialiser(
			[](base_particle& p) {
				p.cur_pos = vec3(-20.0f, 1.5f, 0.0f);
			}
		);
		s_em.set_vel_initialiser(
			[](base_particle& p) {
				p.cur_vel = vec3(15.0f, 10.0f, 0.0f);
			}
		);
		s_em.set_lifetime_initialiser(
			[&](free_particle& p) { p.lifetime = lifetime; }
		);
		s_em.set_bounce_initialiser(
			[](free_particle& p) { p.bouncing = 1.0f; }
		);
		s_em.set_friction_initialiser(
			[](free_particle& p) { p.friction = 0.0f; }
		);
		s_em.set_radius_initialiser(
			[](sized_particle& p) { p.R = 1.0f; }
		);
		SR.get_simulator().set_sized_emitter(&s_em);
		SR.get_simulator().add_sized_particles(1);
	}

	void sim_201_make_simulation() {

		sim_201_make_simulation_free();
		sim_201_make_simulation_sized();

		plane *pl = new plane(
			vec3(0.0f,1.0f,0.0f),
			vec3(0.0f,0.0f,0.0f)
		);
		SR.get_simulator().add_geometry(pl);
		SR.get_simulator().set_gravity_acceleration(
			vec3(0.0f,-9.81f,0.0f)
		);

		SR.get_simulator().set_particle_particle_collisions(true);

		rplane *floor = new rplane();
		floor->set_points(
			glm::vec3(-5.0f, -0.05f, -5.0f), glm::vec3(-5.0f, -0.05f,  5.0f),
			glm::vec3( 5.0f, -0.05f,  5.0f), glm::vec3( 5.0f, -0.05f, -5.0f)
		);
		SR.add_geometry(floor);

		SR.get_box().enlarge_box(glm::vec3(0.0f, 12.0f, 0.0f));
		SR.set_window_dims(window_width, window_height);
		SR.init_cameras();
		n_iterations = 1;
		SR.get_simulator().set_time_step(time_step);

		wireframe_sphere = new rendered_triangle_mesh();
		OBJ_reader obj;
		obj.load_object
		("../../interfaces/models", "sphere_fsmooth.obj", *wireframe_sphere);

		glut_functions::init_shaders();
		SR.get_box().make_buffers();
		wireframe_sphere->make_buffers();
	}

	void sim_201_help() {
		glut_functions::help();

		cout << "Simulation 200 description:" << endl;
		cout << endl;
		cout << "Exactly 1000 particles bouncing on top of a plane." << endl;
		cout << "These particles are generated with a rectangular source." << endl;
		cout << "Just right above the plane is a sized particle" << endl;
		cout << endl;
	}

	void sim_201_reset() {
		clear_simulation();
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

		sim_201_make_simulation();

		SR.set_perspective(old_p);
		SR.set_orthogonal(old_o);
		SR.set_VRP(VRP);
		SR.set_theta(theta);
		SR.set_psi(psi);
		SR.set_viewer_pos(viewer_pos);
		SR.set_yaw(yaw);
		SR.set_pitch(pitch);
	}

	void sim_201_regular_keys_keyboard(unsigned char c, int x, int y) {
		regular_keys_keyboard(c, x, y);

		switch (c) {
		case 'h':
			sim_201_help();
			break;
		case 'r':
			sim_201_reset();
			break;
		}
	}

	int sim_201_initGL(int argc, char *argv[]) {
		// ----------------- //
		/* initialise window */
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
		glutInitWindowPosition(50, 25);
		glutInitWindowSize(window_width, window_height);
		window_id = glutCreateWindow("Particles - Simulation 200");

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
		draw_sized_particles_wire = true;

		// ---------------- //
		/* build simulation */
		sim_201_make_simulation();
		return 0;
	}

	void sim_201(int argc, char *argv[]) {
		sim_201_initGL(argc, argv);
		sim_201_help();

		glutDisplayFunc(glut_functions::refresh);
		glutReshapeFunc(glut_functions::resize);
		glutMouseFunc(glut_functions::mouse_click_event);
		glutPassiveMotionFunc(glut_functions::mouse_movement);
		glutMotionFunc(glut_functions::mouse_drag_event);
		glutSpecialFunc(glut_functions::special_keys_keyboard);
		glutKeyboardFunc(sim_201_regular_keys_keyboard);

		//glutIdleFunc(refresh);
		glutTimerFunc(1000.0f/FPS, glut_functions::timed_refresh, 0);

		glutMainLoop();
	}

} // -- namespace study_cases
