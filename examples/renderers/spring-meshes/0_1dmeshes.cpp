#include "study_cases.hpp"

// base includes
#include <base/include_gl.hpp>
#include <base/obj_reader.hpp>
#include <base/model/rendered_model.hpp>

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
	}

	void sim0_keyboard_event(unsigned char c, int x, int y) {

	}

	void sim0_special_keys(int key, int x, int y) {

	}

	void sim0_1dmeshes(int argc, char *argv[]) {

		// parse arguments

		// build simulator

		rendered_model *m;
		OBJ_reader obj;

		m = new rendered_model();
		obj.load_object("../../renderers/models" , "sphere.obj", *m);
		m->compile();
		SR.add_model(m);

		SR.set_window_dims(iw, ih);
		SR.init_cameras();

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
