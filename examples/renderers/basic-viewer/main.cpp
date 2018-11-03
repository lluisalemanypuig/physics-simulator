// C++ includes
#include <iostream>
#include <utility>
using namespace std;

// base includes
#include <base/include_gl.hpp>
#include <base/model/rendered_model.hpp>
#include <base/obj_reader.hpp>

// custom includes
#include <base/renderer.hpp>

typedef pair<int,int> point;

// ------------------
// global variables
// ------------------

renderer SR;

int pressed_button;
point last_mouse;

bool lock_mouse;
int window_id;

// glut helpers
#define ESC 27
#define LEFT_ARROW 100
#define UP_ARROW 101
#define RIGHT_ARROW 102
#define DOWN_AROW 103

// -----------------------
// Global helper functions
// -----------------------

template<typename T>
static inline void UNUSED(const T& x) { (void)x; }

static inline
bool inside_window(int x, int y) {
	return ((0 <= x) and (x <= SR.window_width()))
	   and ((0 <= y) and (y <= SR.window_height()));
}

// -----------------
// INITIALISE OpenGL
// -----------------

void initGL(int argc, char *argv[]) {
	// initial window size
	int iw = 640;
	int ih = 480;

	// ----------------- //
	/* initialise window */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(50, 25);
	glutInitWindowSize(iw, ih);
	window_id = glutCreateWindow("Basic viewer");

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

	// ------------------------ //
	/* load models for geometry */
	rendered_model *m;
	OBJ_reader obj;

	m = new rendered_model();
	obj.load_object("../../renderers/models" , "sphere.obj", *m);
	m->compile();
	SR.add_model(m);

	// --------------------------- //
	/* initialise global variables */
	pressed_button = 0;
	last_mouse = point(0,0);
	lock_mouse = false;

	SR.set_window_dims(iw, ih);
	SR.init_cameras();
}

// ------------
// RENDER SCENE
// ------------

void refresh() {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	SR.apply_view_mode();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	SR.apply_camera();

	glEnable(GL_LIGHTING);
	SR.render_models();

	glDisable(GL_LIGHTING);
	glColor3f(1.0f,0.0f,0.0f);
	SR.get_box().draw_box();

	glutSwapBuffers();
}

// ---------------
// RESIZE viewport
// ---------------

void reshape(int w, int h) {
	float pzoom = SR.get_perspective_camera().get_zoom();
	float ozoom = SR.get_orthogonal_camera().get_zoom();

	SR.set_window_dims(w, h);
	SR.init_cameras();

	SR.get_perspective_camera().set_zoom(pzoom);
	SR.get_orthogonal_camera().set_zoom(ozoom);

	glViewport(0, 0, w, h);
}

// ++++++++++++++++++
// EVENTS
// ++++++++++++++++++

// -------------
// MOUSE HANDLER
// -------------

void mouse_click_event(int button, int state, int x, int y) {
	UNUSED(x);
	UNUSED(y);

	if (SR.is_flying()) {
		cout << "Flying..." << endl;
		if (button == GLUT_LEFT_BUTTON and state == 0) {
			cout << "Left click... " << state << endl;
			cout << "    Is mouse locked? " << (lock_mouse ? "Yes" : "No") << endl;
			if (lock_mouse) {
				cout << "    Unlock mouse" << endl;
				lock_mouse = false;
				glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
			}
			else {
				cout << "    Lock mouse" << endl;
				lock_mouse = true;
				glutWarpPointer(SR.window_width()/2,SR.window_height()/2);
				glutSetCursor(GLUT_CURSOR_NONE);
			}
		}
	}
	pressed_button = button;
}

void mouse_movement(int x, int y) {
	int dx = x - last_mouse.first;
	int dy = y - last_mouse.second;
	last_mouse = point(x,y);

	if (SR.is_flying() and lock_mouse) {
		SR.increment_yaw(-0.2f*dx);
		SR.increment_pitch(-0.2f*dy);

		glutWarpPointer(SR.window_width()/2,SR.window_height()/2);
		last_mouse = point(SR.window_width()/2,SR.window_height()/2);
	}
}

void mouse_drag_event(int x, int y) {
	int dx = x - last_mouse.first;
	int dy = y - last_mouse.second;
	last_mouse = point(x,y);

	if (pressed_button == GLUT_LEFT_BUTTON) {
		if (SR.is_inspecting()) {
			SR.increment_psi(-0.3f*dx);
			SR.increment_theta(0.3f*dy);
		}
	}
	else if (pressed_button == GLUT_RIGHT_BUTTON) {
		SR.increment_zoom(0.75f*dy);
	}
}

// ----------------
// KEYBOARD HANDLER
// ----------------

void special_keys(int key, int x, int y) {
	UNUSED(key);
	UNUSED(x);
	UNUSED(y);
}

void keyboard_event(unsigned char c, int x, int y) {
	UNUSED(x);
	UNUSED(y);

	if (c == ESC) {
		glutDestroyWindow(window_id);
	}
	else if (c == 'p') {
		SR.switch_to_perspective();
	}
	else if (c == 'o') {
		SR.switch_to_orthogonal();
	}
	else if (c == 'i') {
		SR.switch_to_inspection();
		lock_mouse = false;
		glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
	}
	else if (c == 'f') {
		SR.switch_to_flight();
		lock_mouse = true;
		glutWarpPointer(SR.window_width()/2,SR.window_height()/2);
		glutSetCursor(GLUT_CURSOR_NONE);
	}
	else {
		if (SR.is_flying()) {
			if (c == 'w') {
				SR.camera_forwards(0.1f);
			}
			else if (c == 's') {
				SR.camera_backwards(0.1f);
			}
			else if (c == 'a') {
				SR.camera_sideways_left(0.1f);
			}
			else if (c == 'd') {
				SR.camera_sideways_right(0.1f);
			}
		}
	}
}

int main(int argc, char* argv[]) {
	initGL(argc, argv);

	glutDisplayFunc(refresh);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard_event);
	glutSpecialFunc(special_keys);
	glutMouseFunc(mouse_click_event);
	glutPassiveMotionFunc(mouse_movement);
	glutMotionFunc(mouse_drag_event);

	glutIdleFunc(refresh);

	glutMainLoop();
}
