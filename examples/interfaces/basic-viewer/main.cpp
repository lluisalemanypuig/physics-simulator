// C includes
#include <string.h>

// C++ includes
#include <iostream>
#include <utility>
using namespace std;

// glm includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>

// render includes
#include <render/include_gl.hpp>
#include <render/triangle_mesh/rendered_triangle_mesh.hpp>
#include <render/shader/shader_helper.hpp>
#include <render/shader/shader.hpp>
#include <render/scene/viewer.hpp>
#include <render/obj_reader.hpp>

// custom includes
#include <basic-viewer/utils.hpp>

typedef pair<int,int> point;

// ------------------
// global variables
// ------------------

static rendered_triangle_mesh *m;

static shader model_shader;
static shader flat_shader;
static viewer SR;
static bool use_shader = false;

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

// -----------
// Exit viewer
// -----------

void __psv_exit_func() {
	m->clear();
	delete m;
}

// -----------------
// INITIALISE OpenGL
// -----------------

void __psv_initGL(int argc, char *argv[]) {
	if (argc > 1) {
		if (strcmp(argv[1], "--use-shaders") == 0) {
			use_shader = true;
		}
		else if (strcmp(argv[1], "--use-shader") == 0) {
			use_shader = true;
		}
		else if (strcmp(argv[1], "--use-shade") == 0) {
			use_shader = true;
		}
		else if (strcmp(argv[1], "--use-shad") == 0) {
			use_shader = true;
		}
	}

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

	// ------------ //
	/* load shaders */
	if (use_shader) {
		bool r;
		r = model_shader.init
			("../../interfaces/shaders", "textures.vert", "textures.frag");
		if (not r) {
			exit(1);
		}
		r = flat_shader.init
			("../../interfaces/shaders", "flat.vert", "flat.frag");
		if (not r) {
			exit(1);
		}

		model_shader.bind();
		model_shader.set_vec3("light.diffuse", glm::vec3(1.0f,1.0f,1.0f));
		model_shader.set_vec3("light.ambient", glm::vec3(0.2f,0.2f,0.2f));
		model_shader.set_vec3("light.position", glm::vec3(0.f,0.f,0.f));
		model_shader.release();
	}

	// --------------------------- //
	/* initialise global variables */
	pressed_button = 0;
	last_mouse = point(0,0);
	lock_mouse = false;

	sec = timing::now();
	display_fps_count = true;
	FPS = 60;
	fps_count = 0;

	// ------------------------ //
	/* load models for geometry */
	m = new rendered_triangle_mesh();

	OBJ_reader obj;
	obj.load_object("../../interfaces/models" , "sphere.obj", *m);
	m->display_mesh_info();

	box model_box;
	m->make_box(model_box);
	SR.get_box().set_min_max(model_box.get_min(), model_box.get_max());

	SR.set_window_dims(iw, ih);
	SR.init_cameras();

	// ---------------------------------- //
	/* compile or make buffers for models */

	if (use_shader) {
		cout << "Making buffers for models..." << endl;
		SR.get_box().make_buffers();
		m->load_textures();
		m->make_buffers_materials_textures();

		model_shader.bind();
		shader_helper::activate_textures(*m, model_shader);
		model_shader.release();
	}
	else {
		cout << "Compiling models..." << endl;
		m->load_textures();
		m->compile();
	}
}

// ------------
// RENDER SCENE
// ------------

void __psv_refresh() {
	++fps_count;

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (use_shader) {
		glm::mat4 projection(1.0f), view(1.0f);
		SR.make_projection_matrix(projection);
		SR.make_view_matrix(view);
		glm::mat3 normal_matrix = glm::inverseTranspose(glm::mat3(view));

		model_shader.bind();
		model_shader.set_mat4("projection", projection);
		model_shader.set_mat4("modelview", view);
		model_shader.set_mat3("normal_matrix", normal_matrix);
		model_shader.set_vec3("view_pos", glm::vec3(0.f,0.f,0.f));
		shader_helper::set_materials_shader(*m, model_shader);
		m->render();
		model_shader.release();

		flat_shader.bind();
		flat_shader.set_bool("wireframe", true);
		flat_shader.set_vec4("colour", glm::vec4(1.0f,0.0f,0.0f,1.0f));
		flat_shader.set_mat4("projection", projection);
		flat_shader.set_mat4("modelview", view);
		flat_shader.set_mat3("normal_matrix", normal_matrix);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		SR.get_box().fast_render();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		flat_shader.release();
	}
	else {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		SR.apply_projection();

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		SR.apply_view();

		glEnable(GL_LIGHTING);

		m->slow_render();

		glDisable(GL_LIGHTING);
		glColor3f(1.0f,0.0f,0.0f);
		SR.get_box().slow_render();
	}

	glutSwapBuffers();
}

void __psv_timed_refresh(int value) {
	UNUSED(value);

	__psv_refresh();

	timing::time_point here = timing::now();
	double elapsed = timing::elapsed_seconds(sec, here);
	if (elapsed >= 1.0) {
		if (display_fps_count) {
			cout << "fps " << fps_count << " (" << FPS << ")" << endl;
		}
		fps_count = 0;
		sec = timing::now();
	}

	glutTimerFunc(1000.0f/FPS, __psv_timed_refresh, 0);
}

// ---------------
// RESIZE viewport
// ---------------

void __psv_reshape(int w, int h) {
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

void __psv_mouse_click_event(int button, int state, int x, int y) {
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

void __psv_mouse_movement(int x, int y) {
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

void __psv_mouse_drag_event(int x, int y) {
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

void __psv_special_keys(int key, int x, int y) {
	UNUSED(key);
	UNUSED(x);
	UNUSED(y);
}

void __psv_keyboard_event(unsigned char c, int x, int y) {
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
	__psv_initGL(argc, argv);

	atexit(__psv_exit_func);

	glutDisplayFunc(__psv_refresh);
	glutReshapeFunc(__psv_reshape);
	glutKeyboardFunc(__psv_keyboard_event);
	glutSpecialFunc(__psv_special_keys);
	glutMouseFunc(__psv_mouse_click_event);
	glutPassiveMotionFunc(__psv_mouse_movement);
	glutMotionFunc(__psv_mouse_drag_event);

	//glutIdleFunc(refresh);
	glutTimerFunc(1000.0f/FPS, __psv_timed_refresh, 0);

	glutMainLoop();
}
