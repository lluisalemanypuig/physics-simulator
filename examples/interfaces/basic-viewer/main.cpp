// C++ includes
#include <iostream>
#include <utility>
using namespace std;

// base includes
#include <base/include_gl.hpp>
#include <base/model/rendered_model.hpp>
#include <base/obj_reader.hpp>
#include <base/scene/renderer.hpp>
#include <base/shader.hpp>

// custom includes
#include "utils.hpp"

typedef pair<int,int> point;

// ------------------
// global variables
// ------------------

static renderer SR;
static bool use_shader = true;

static timing::time_point sec;

static int FPS;
static int fps_count;
static bool display_fps_count;

static int pressed_button;
static point last_mouse;

static bool lock_mouse;
static int window_id;

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

	GLenum err = glewInit();
	if (err != 0) {
		cerr << "initGL - Error:" << endl;
		cerr << "    when initialising glew: " << err << endl;
		exit(1);
	}

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

	// --------------------------- //
	/* initialise global variables */
	pressed_button = 0;
	last_mouse = point(0,0);
	lock_mouse = false;

	SR.set_window_dims(iw, ih);
	SR.init_cameras();

	sec = timing::now();
	display_fps_count = true;
	FPS = 60;
	fps_count = 0;

	// ------------------------ //
	/* load models for geometry */
	rendered_model *m;
	OBJ_reader obj;

	m = new rendered_model();
	obj.load_object("../../interfaces/models" , "sphere.obj", *m);
	m->load_textures();

	SR.add_model(m);

	SR.set_window_dims(iw, ih);
	SR.init_cameras();

	if (use_shader) {
		cout << "Initialising shader program..." << endl;
		bool r = SR.init_shader("../../interfaces/shaders", "vertex.vert", "fragment.frag");
		if (not r) {
			exit(1);
		}

		cout << "    Initialised" << endl;

		SR.get_shader().bind();
		m->make_buffers();
		SR.get_shader().release();
	}
	else {
		m->compile();
	}
}

// ------------
// RENDER SCENE
// ------------

void refresh() {
	++fps_count;

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (use_shader) {
		SR.apply_projection(true);
		SR.apply_modelview(true);
	}
	else {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		SR.apply_projection(false);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		SR.apply_modelview(false);

		glEnable(GL_LIGHTING);
	}

	SR.render_models(use_shader);

	glDisable(GL_LIGHTING);
	glColor3f(1.0f,0.0f,0.0f);
	SR.get_box().draw_box();

	glutSwapBuffers();
}

void timed_refresh(int value) {
	UNUSED(value);

	refresh();

	timing::time_point here = timing::now();
	double elapsed = timing::elapsed_seconds(sec, here);
	if (elapsed >= 1.0) {
		if (display_fps_count) {
			cout << "fps " << fps_count << " (" << FPS << ")" << endl;
		}
		fps_count = 0;
		sec = timing::now();
	}

	glutTimerFunc(1000.0f/FPS, timed_refresh, 0);
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

	SR.apply_projection(use_shader);
	SR.apply_modelview(use_shader);

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

		SR.apply_projection(use_shader);
		SR.apply_modelview(use_shader);
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
			SR.apply_modelview(use_shader);
		}
	}
	else if (pressed_button == GLUT_RIGHT_BUTTON) {
		SR.increment_zoom(0.75f*dy);
		SR.apply_projection(use_shader);
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

		SR.apply_projection(use_shader);
	}
	else if (c == 'o') {
		SR.switch_to_orthogonal();

		SR.apply_projection(use_shader);
	}
	else if (c == 'i') {
		SR.switch_to_inspection();
		lock_mouse = false;
		glutSetCursor(GLUT_CURSOR_LEFT_ARROW);

		SR.apply_modelview(use_shader);
	}
	else if (c == 'f') {
		SR.switch_to_flight();
		lock_mouse = true;
		glutWarpPointer(SR.window_width()/2,SR.window_height()/2);
		glutSetCursor(GLUT_CURSOR_NONE);

		SR.apply_modelview(use_shader);
	}
	else if (c == '+') {
		if (FPS < 59) {
			++FPS;
		}
	}
	else if (c == '-') {
		if (FPS > 1) {
			--FPS;
		}
	}
	else if (c == 'x') {
		display_fps_count = not display_fps_count;
	}
	else {
		if (SR.is_flying()) {
			if (c == 'w') {
				SR.camera_forwards(0.1f);
				SR.apply_modelview(use_shader);
			}
			else if (c == 's') {
				SR.camera_backwards(0.1f);
				SR.apply_modelview(use_shader);
			}
			else if (c == 'a') {
				SR.camera_sideways_left(0.1f);
				SR.apply_modelview(use_shader);
			}
			else if (c == 'd') {
				SR.camera_sideways_right(0.1f);
				SR.apply_modelview(use_shader);
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

	//glutIdleFunc(refresh);
	glutTimerFunc(1000.0f/FPS, timed_refresh, 0);

	glutMainLoop();
}
