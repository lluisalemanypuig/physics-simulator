#include "study_cases.hpp"

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

	void sim1_make_simulation() {
		SR.set_particle_size(2.0f);
		SR.set_spring_width(1.5f);

		SR.get_simulator().set_solver(solver_type::EulerSemi);
		SR.get_simulator().add_gravity_acceleration(vec3(0.0f,-9.81f,0.0f));

		size_t n = 5;
		size_t m = 5;
		float length = 10.0f;
		float height = 10.0f;

		// build regular mesh
		mesh2d_regular *M = new mesh2d_regular();
		M->simulate_bend(glut_functions::bend);
		M->simulate_shear(glut_functions::shear);
		M->simulate_stretch(glut_functions::stretch);

		M->allocate(n*m, 5.0f);
		M->set_dimensions(n, m);

		M->set_elasticity(500.0f);
		M->set_damping(0.5f);

		mesh_particle **mp = M->get_particles();

		// fix some particles
		mp[ M->get_global_index(0,m-1) ]->fixed = true;
		mp[ M->get_global_index(n-1,m - 1) ]->fixed = true;

		// make positions
		for (size_t i = 0; i < n; ++i) {
			for (size_t j = 0; j < m; ++j) {
				mp[ M->get_global_index(i,j) ]->cur_pos =
						vec3((length/n)*i, (height/m)*j, 0.0f);
			}
		}

		M->make_initial_state();

		SR.get_simulator().add_mesh(M);

		SR.get_box().set_min_max(vec3(-5,-5,-5), vec3(15,15,5));
		SR.set_window_dims(iw, ih);
		SR.init_cameras();
	}

	void sim1_help() {
		glut_functions::help();

		cout << "Simulation 2 description:" << endl;
		cout << endl;

	}

	void sim1_regular_keys_keyboard(unsigned char c, int x, int y) {
		regular_keys_keyboard(c, x, y);

		switch (c) {
		case 'h':
			sim1_help();
			break;
		case 'r':
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

			sim1_make_simulation();

			SR.set_perspective(old_p);
			SR.set_orthogonal(old_o);
			SR.set_VRP(VRP);
			SR.set_theta(theta);
			SR.set_psi(psi);
			SR.set_viewer_pos(viewer_pos);
			SR.set_yaw(yaw);
			SR.set_pitch(pitch);

			break;
		}
	}

	void sim1_initGL(int argc, char *argv[]) {
		// ----------------- //
		/* initialise window */
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
		glutInitWindowPosition(50, 25);
		glutInitWindowSize(iw, ih);
		window_id = glutCreateWindow("Spring meshes - Simulation 1");

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_NORMALIZE);
		glEnable(GL_LIGHTING);

		glEnable(GL_LIGHT0);
		float col[] = {1.0, 1.0, 1.0, 1.0};
		glLightfv(GL_LIGHT0, GL_DIFFUSE, col);
		float pos[] = {0.0, 0.0, 0.0, 1.0};
		glLightfv(GL_LIGHT0, GL_POSITION, pos);
		float amb[] = {0.2, 0.2, 0.2, 1.0};
		glLightfv(GL_LIGHT0, GL_AMBIENT, amb);

		// --------------------------- //
		/* initialise global variables */
		glut_functions::init_glut_variables();

		glut_functions::stretch = true;
		glut_functions::shear = false;
		glut_functions::bend = false;

		// ---------------- //
		/* build simulation */
		sim1_make_simulation();
	}

	void sim1_2dmeshes(int argc, char *argv[]) {
		sim1_help();
		sim1_initGL(0, nullptr);

		glutDisplayFunc(glut_functions::refresh);
		glutReshapeFunc(glut_functions::resize);
		glutMouseFunc(glut_functions::mouse_click_event);
		glutPassiveMotionFunc(glut_functions::mouse_movement);
		glutMotionFunc(glut_functions::mouse_drag_event);
		glutSpecialFunc(glut_functions::special_keys_keyboard);
		glutKeyboardFunc(sim1_regular_keys_keyboard);

		//glutIdleFunc(refresh);
		glutTimerFunc(1000.0f/FPS, glut_functions::timed_refresh, 0);

		glutMainLoop();
	}

} // -- namespace study_cases
