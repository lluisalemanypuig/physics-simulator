// C includes
#include <string.h>

// C++ includes
#include <iostream>
#include <utility>
using namespace std;

// physim includes
#include <physim/math/vec3.hpp>
#include <physim/input/input.hpp>
#include <physim/structures/octree.hpp>

// render includes
#include <render/include_gl.hpp>
#include <render/triangle_mesh/rendered_triangle_mesh.hpp>
#include <render/shader/shader_helper.hpp>
#include <render/shader/shader.hpp>
#include <render/scene/viewer.hpp>
#include <render/obj_reader.hpp>

#include <octree-viewer/utils.hpp>

typedef pair<int,int> point;
typedef physim::math::vec3 pmvec3;
typedef pair<pmvec3, pmvec3> pmbox;

// ------------------
// global variables
// ------------------

static vector<pmbox> tree_boxes;
static vector<pmvec3> file_points;
static vector<size_t> query_indices;
static physim::structures::octree tree;
static pmvec3 query_point;
static bool valid_query;
static float query_radius;

static viewer SR;

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

inline static
void draw_box(const pmbox& B) {
	const pmvec3& min = B.first;
	const pmvec3& max = B.second;

	glBegin(GL_LINES);
		// -----------------------------
		glVertex3f(min.x, min.y, min.z);
		glVertex3f(max.x, min.y, min.z);

		glVertex3f(max.x, min.y, min.z);
		glVertex3f(max.x, min.y, max.z);

		glVertex3f(max.x, min.y, max.z);
		glVertex3f(min.x, min.y, max.z);

		glVertex3f(min.x, min.y, max.z);
		glVertex3f(min.x, min.y, min.z);

		// -----------------------------
		glVertex3f(min.x, max.y, min.z);
		glVertex3f(max.x, max.y, min.z);

		glVertex3f(max.x, max.y, min.z);
		glVertex3f(max.x, max.y, max.z);

		glVertex3f(max.x, max.y, max.z);
		glVertex3f(min.x, max.y, max.z);

		glVertex3f(min.x, max.y, max.z);
		glVertex3f(min.x, max.y, min.z);

		// -----------------------------
		glVertex3f(min.x, min.y, min.z);
		glVertex3f(min.x, max.y, min.z);

		glVertex3f(max.x, min.y, min.z);
		glVertex3f(max.x, max.y, min.z);

		glVertex3f(max.x, min.y, max.z);
		glVertex3f(max.x, max.y, max.z);

		glVertex3f(min.x, min.y, max.z);
		glVertex3f(min.x, max.y, max.z);
	glEnd();
}

// -----------
// Exit viewer
// -----------

void exit_func() {

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

	display_fps_count = false;
	FPS = 60;
	fps_count = 0;

	bool r = physim::input::read_file
			 ("../../command-line/scenes", "points.obj", file_points);
	if (not r) {
		cerr << "Error in reading file." << endl;
		return;
	}

	cout << "# points: " << file_points.size() << endl;

	tree.init(file_points, 1);

	tree.get_boxes(tree_boxes);
	pmvec3 allmin, allmax;
	allmin = file_points[0];
	allmax = file_points[0];
	for (size_t i = 1; i < file_points.size(); ++i) {
		physim::math::min(allmin, file_points[i], allmin);
		physim::math::max(allmax, file_points[i], allmax);
	}

	valid_query = false;

	cout << "Minimum: " << allmin.x << "," << allmin.y << "," << allmin.z << endl;
	cout << "Maximum: " << allmax.x << "," << allmax.y << "," << allmax.z << endl;

	SR.set_window_dims(iw, ih);
	SR.get_box().set_min_max
	(
		glm::vec3(allmin.x,allmin.y,allmin.z),
		glm::vec3(allmax.x,allmax.y,allmax.z)
	);
	SR.init_cameras();

	glDisable(GL_LIGHTING);

	sec = timing::now();
}

// ------------
// RENDER SCENE
// ------------

void refresh() {
	++fps_count;

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	SR.apply_projection();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	SR.apply_view();

	if (file_points.size() > 0) {
		glPointSize(5);
		glColor3f(1,0,0);
		glBegin(GL_POINTS);
		for (const pmvec3& p : file_points) {
			glVertex3f(p.x, p.y, p.z);
		}
		glEnd();
	}

	if (tree_boxes.size() > 0) {
		glColor3f(1,0,1);
		for (const pmbox& B : tree_boxes) {
			draw_box(B);
		}
	}

	if (valid_query) {
		glColor3f(1,1,1);
		glBegin(GL_POINTS);
		if (query_indices.size() > 0) {
			glPointSize(10);
			for (size_t idx : query_indices) {
				const pmvec3& p = file_points[idx];
				glVertex3f(p.x + 0.01f, p.y + 0.01f, p.z + 0.01f);
			}
		}
		glColor3f(1,1,0);
		glVertex3f(query_point.x, query_point.y, query_point.z);
		glEnd();

		glPushMatrix();
			glTranslatef(query_point.x, query_point.y, query_point.z);
			glutWireSphere(query_radius, 10, 10);
		glPopMatrix();
	}

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
	else if (c == 'q') {
		cout << "Enter query point:" << endl;
		cin >> query_point.x >> query_point.y >> query_point.z;
		cout << "Enter radius:" << endl;
		cin >> query_radius;
		valid_query = true;

		query_indices.clear();
		tree.get_indices(query_point, query_radius, query_indices);

		cout << "Result of query:" << endl;
		cout << "    # indices retrieved: " << query_indices.size() << endl;
		cout << "    indices:";
		for (size_t idx : query_indices) {
			cout << " " << idx;
		}
		cout << endl;
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

	atexit(exit_func);

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
