#include "study_cases.hpp"

#include <iostream>
using namespace std;

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
		glut_functions::init_glut_variables();

		// ---------------- //
		/* build simulation */
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

		SR.get_simulator().add_gravity_acceleration(vec3(0.0f,-9.81f,0.0f));

		/* build meshes */
		for (int i = 0; i < 1; ++i) {
			// 10 'free' meshes of increasing
			// number of particles

			for (int j = 5; j <= 50; j += 5) {
				meshes::mesh1d *m = new meshes::mesh1d();
				m->allocate(j);
				m->set_elasticity(500.0f);
				m->set_damping(0.5f);

				mesh_particle **mp = m->get_particles();
				mp[0]->fixed = true;
				mp[0]->cur_pos = vec3(0.0f, 10.0f, i);
				for (int k = 1; k < j; ++k) {
					mp[k]->cur_pos = vec3((5.0f/j)*k, 10.0f, i);
				}
				m->make_initial_state();

				SR.get_simulator().add_mesh(m);
			}
		}
	}

	void sim0_1dmeshes(int argc, char *argv[]) {

		sim0_initGL(0, nullptr);

		glutDisplayFunc(glut_functions::refresh);
		glutReshapeFunc(glut_functions::resize);
		glutMouseFunc(glut_functions::mouse_click_event);
		glutPassiveMotionFunc(glut_functions::mouse_movement);
		glutMotionFunc(glut_functions::mouse_drag_event);
		glutSpecialFunc(glut_functions::special_keys_keyboard);
		glutKeyboardFunc(glut_functions::regular_keys_keyboard);

		//glutIdleFunc(refresh);
		glutTimerFunc(1000.0f/FPS, glut_functions::timed_refresh, 0);

		glutMainLoop();
	}

} // -- namespace study_cases
