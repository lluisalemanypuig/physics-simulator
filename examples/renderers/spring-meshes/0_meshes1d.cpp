#include "study_cases.hpp"

// base includes
#include <base/include_gl.hpp>
#include <base/obj_reader.hpp>
#include <base/model/rendered_model.hpp>
#include <base/rgeom/rendered_geometry.hpp>

// physim includes
#include <physim/meshes/mesh1d.hpp>
#include <physim/geometry/plane.hpp>
#include <physim/math/math.hpp>
using namespace physim;
using namespace particles;
using namespace math;
using namespace geom;

// custom includes
#include "glut_functions.hpp"
using namespace glut_functions;

namespace study_cases {

	// initial window size
	#define iw 640
	#define ih 480

	void sim0_help() {

	}

	void sim0_initGL(int argc, char *argv[]) {
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
		pressed_button = 0;
		last_mouse = point(0,0);
		lock_mouse = false;

		// -------------- //
		/* build renderer */
		SR.set_particle_size(2.0f);
		SR.set_spring_width(1.5f);
		rplane *floor = new rplane();
		floor->p1 = vec3(-5.0f, 7.0f, -5.0f);
		floor->p2 = vec3(-5.0f, 7.0f,  5.0f);
		floor->p3 = vec3( 5.0f, 7.0f,  5.0f);
		floor->p4 = vec3( 5.0f, 7.0f, -5.0f);
		SR.add_geometry(floor);
		SR.get_box().enlarge_box(vec3(0.0f,10.0f,0.0f));
		SR.set_window_dims(iw, ih);
		SR.init_cameras();
		FPS = 30;
		fps_count = 0;

		// ---------------- //
		/* build simulation */
		plane *pl = new plane(
			vec3(0.0f,1.0f,0.0f),
			vec3(0.0f,7.0f,0.0f)
		);
		SR.get_simulator().add_geometry(pl);

		SR.get_simulator().add_gravity_acceleration(vec3(0.0f,-9.81f,0.0f));

		meshes::mesh1d *m = new meshes::mesh1d();
		m->allocate(5);
		m->set_elasticity(500.0f);
		m->set_damping(0.5f);

		mesh_particle **mp = m->get_particles();

		mp[0]->fixed = true;

		mp[0]->cur_pos = vec3(0.0f, 10.0f, 0.0f);
		mp[1]->cur_pos = vec3(1.0f, 10.0f, 0.0f);
		mp[2]->cur_pos = vec3(2.0f, 10.0f, 0.0f);
		mp[3]->cur_pos = vec3(3.0f, 10.0f, 0.0f);
		mp[4]->cur_pos = vec3(4.0f, 10.0f, 0.0f);

		m->make_initial_state();

		SR.get_simulator().add_mesh(m);

		// ----------- //
		/* start timer */
		tp = timing::now();
	}

	void sim0_keyboard_event(unsigned char c, int x, int y) {

	}

	void sim0_special_keys(int key, int x, int y) {

	}

	void sim0_1dmeshes(int argc, char *argv[]) {

		// parse arguments

		// build simulator

		// call glut functions
		sim0_initGL(0, nullptr);

		glutDisplayFunc(glut_functions::refresh);
		glutReshapeFunc(glut_functions::resize);
		glutKeyboardFunc(sim0_keyboard_event);
		glutSpecialFunc(sim0_special_keys);
		glutMouseFunc(glut_functions::mouse_click_event);
		glutPassiveMotionFunc(glut_functions::mouse_movement);
		glutMotionFunc(glut_functions::mouse_drag_event);

		glutIdleFunc(refresh);

		glutMainLoop();
	}

} // -- namespace study_cases
