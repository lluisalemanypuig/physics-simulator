/*********************************************************************
 * Real-time physics simulation project
 * Copyright (C) 2018-2019 Lluís Alemany Puig
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * Contact: Lluís Alemany Puig (lluis.alemany.puig@gmail.com)
 * 
 ********************************************************************/

#include "point_set_viewer.hpp"

// C includes
#include <string.h>

// C++ includes
#include <iostream>
#include <numeric>
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

static vector<pmbox> __psv_tree_boxes;
static vector<pmvec3> __psv_file_points;
static vector<uint> __psv_file_points_idxs;
static vector<size_t> __psv_query_indices;
static physim::structures::octree __psv_tree;
static pmvec3 __psv_query_point;
static bool __psv_valid_query;
static float __psv_query_radius;

static viewer __psv_SR;

static timing::time_point __psv_sec;
static int __psv_FPS;
static int __psv_fps_count;
static bool __psv_display_fps_count;

static int __psv_pressed_button;
static point __psv_last_mouse;

static bool __psv_lock_mouse;
static int __psv_window_id;

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

void __psv_exit_func() {

}

// -----------------
// INITIALISE OpenGL
// -----------------

void __psv_initGL(int argc, char *argv[]) {

	// initial window size
	int iw = 640;
	int ih = 480;

	// ----------------- //
	/* initialise window */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(50, 25);
	glutInitWindowSize(iw, ih);
	__psv_window_id = glutCreateWindow("Basic viewer");

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
	__psv_pressed_button = 0;
	__psv_last_mouse = point(0,0);
	__psv_lock_mouse = false;

	__psv_display_fps_count = false;
	__psv_FPS = 60;
	__psv_fps_count = 0;

	bool r = physim::input::read_file
			 ("../../command-line/scenes", "points.obj", __psv_file_points);
	if (not r) {
		cerr << "Error in reading file." << endl;
		return;
	}

	cout << "# points: " << __psv_file_points.size() << endl;
	__psv_file_points_idxs.resize(__psv_file_points.size());
	std::iota(__psv_file_points_idxs.begin(), __psv_file_points_idxs.end(), 0);

	__psv_tree.init(__psv_file_points, 1);

	__psv_tree.get_boxes(__psv_tree_boxes);
	pmvec3 allmin, allmax;
	allmin = __psv_file_points[0];
	allmax = __psv_file_points[0];
	for (size_t i = 1; i < __psv_file_points.size(); ++i) {
		physim::math::min(allmin, __psv_file_points[i], allmin);
		physim::math::max(allmax, __psv_file_points[i], allmax);
	}

	__psv_valid_query = false;

	cout << "Minimum: " << allmin.x << "," << allmin.y << "," << allmin.z << endl;
	cout << "Maximum: " << allmax.x << "," << allmax.y << "," << allmax.z << endl;

	__psv_SR.set_window_dims(iw, ih);
	__psv_SR.get_box().set_min_max
	(
		glm::vec3(allmin.x,allmin.y,allmin.z),
		glm::vec3(allmax.x,allmax.y,allmax.z)
	);
	__psv_SR.init_cameras();

	glDisable(GL_LIGHTING);

	__psv_sec = timing::now();
}

// ------------
// RENDER SCENE
// ------------

void __psv_refresh() {
	++__psv_fps_count;

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	__psv_SR.apply_projection();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	__psv_SR.apply_view();

	if (__psv_file_points.size() > 0) {
		glPointSize(5);
		glColor3f(1,0,0);

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, &__psv_file_points[0].x);
		glDrawElements(GL_POINTS, __psv_file_points_idxs.size(),
					   GL_UNSIGNED_INT, &__psv_file_points_idxs[0]);
		glDisableClientState(GL_VERTEX_ARRAY);

		/*
		glBegin(GL_POINTS);
		for (const pmvec3& p : file_points) {
			glVertex3f(p.x, p.y, p.z);
		}
		glEnd();
		*/
	}

	if (__psv_tree_boxes.size() > 0) {
		glColor3f(0.3f,0.3f,1.0f);
		for (const pmbox& B : __psv_tree_boxes) {
			draw_box(B);
		}
	}

	if (__psv_valid_query) {
		glColor3f(1,1,1);
		glBegin(GL_POINTS);
		if (__psv_query_indices.size() > 0) {
			glPointSize(10);
			for (size_t idx : __psv_query_indices) {
				const pmvec3& p = __psv_file_points[idx];
				glVertex3f(p.x + 0.01f, p.y + 0.01f, p.z + 0.01f);
			}
		}
		glColor3f(1,1,0);
		glVertex3f(__psv_query_point.x, __psv_query_point.y, __psv_query_point.z);
		glEnd();

		glPushMatrix();
			glTranslatef(__psv_query_point.x, __psv_query_point.y, __psv_query_point.z);
			glutWireSphere(__psv_query_radius, 10, 10);
		glPopMatrix();
	}

	glutSwapBuffers();
}

void __psv_timed_refresh(int value) {
	UNUSED(value);

	__psv_refresh();

	timing::time_point here = timing::now();
	double elapsed = timing::elapsed_seconds(__psv_sec, here);
	if (elapsed >= 1.0) {
		if (__psv_display_fps_count) {
			cout << "fps " << __psv_fps_count << " (" << __psv_FPS << ")" << endl;
		}
		__psv_fps_count = 0;
		__psv_sec = timing::now();
	}

	glutTimerFunc(1000.0f/__psv_FPS, __psv_timed_refresh, 0);
}

// ---------------
// RESIZE viewport
// ---------------

void __psv_reshape(int w, int h) {
	float pzoom = __psv_SR.get_perspective_camera().get_zoom();
	float ozoom = __psv_SR.get_orthogonal_camera().get_zoom();

	__psv_SR.set_window_dims(w, h);
	__psv_SR.init_cameras();

	__psv_SR.get_perspective_camera().set_zoom(pzoom);
	__psv_SR.get_orthogonal_camera().set_zoom(ozoom);

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

	if (__psv_SR.is_flying()) {
		cout << "Flying..." << endl;
		if (button == GLUT_LEFT_BUTTON and state == 0) {
			cout << "Left click... " << state << endl;
			cout << "    Is mouse locked? " << (__psv_lock_mouse ? "Yes" : "No") << endl;
			if (__psv_lock_mouse) {
				cout << "    Unlock mouse" << endl;
				__psv_lock_mouse = false;
				glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
			}
			else {
				cout << "    Lock mouse" << endl;
				__psv_lock_mouse = true;
				glutWarpPointer(__psv_SR.window_width()/2,__psv_SR.window_height()/2);
				glutSetCursor(GLUT_CURSOR_NONE);
			}
		}
	}
	__psv_pressed_button = button;
}

void __psv_mouse_movement(int x, int y) {
	int dx = x - __psv_last_mouse.first;
	int dy = y - __psv_last_mouse.second;
	__psv_last_mouse = point(x,y);

	if (__psv_SR.is_flying() and __psv_lock_mouse) {
		__psv_SR.increment_yaw(-0.2f*dx);
		__psv_SR.increment_pitch(-0.2f*dy);

		glutWarpPointer(__psv_SR.window_width()/2,__psv_SR.window_height()/2);
		__psv_last_mouse = point(__psv_SR.window_width()/2,__psv_SR.window_height()/2);
	}
}

void __psv_mouse_drag_event(int x, int y) {
	int dx = x - __psv_last_mouse.first;
	int dy = y - __psv_last_mouse.second;
	__psv_last_mouse = point(x,y);

	if (__psv_pressed_button == GLUT_LEFT_BUTTON) {
		if (__psv_SR.is_inspecting()) {
			__psv_SR.increment_psi(-0.3f*dx);
			__psv_SR.increment_theta(0.3f*dy);
		}
	}
	else if (__psv_pressed_button == GLUT_RIGHT_BUTTON) {
		__psv_SR.increment_zoom(0.75f*dy);
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
		glutDestroyWindow(__psv_window_id);
	}
	else if (c == 'p') {
		__psv_SR.switch_to_perspective();
	}
	else if (c == 'o') {
		__psv_SR.switch_to_orthogonal();
	}
	else if (c == 'i') {
		__psv_SR.switch_to_inspection();
		__psv_lock_mouse = false;
		glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
	}
	else if (c == 'f') {
		__psv_SR.switch_to_flight();
		__psv_lock_mouse = true;
		glutWarpPointer(__psv_SR.window_width()/2,__psv_SR.window_height()/2);
		glutSetCursor(GLUT_CURSOR_NONE);
	}
	else if (c == '+') {
		if (__psv_FPS < 59) {
			++__psv_FPS;
		}
	}
	else if (c == '-') {
		if (__psv_FPS > 1) {
			--__psv_FPS;
		}
	}
	else if (c == 'x') {
		__psv_display_fps_count = not __psv_display_fps_count;
	}
	else if (c == 'q') {
		cout << "Enter query point:" << endl;
		cin >> __psv_query_point.x >> __psv_query_point.y >> __psv_query_point.z;
		cout << "Enter radius:" << endl;
		cin >> __psv_query_radius;
		__psv_valid_query = true;

		__psv_query_indices.clear();
		__psv_tree.get_indices(__psv_query_point, __psv_query_radius, __psv_query_indices);

		cout << "Result of query:" << endl;
		cout << "    # indices retrieved: " << __psv_query_indices.size() << endl;
		cout << "    indices:";
		for (size_t idx : __psv_query_indices) {
			cout << " " << idx;
		}
		cout << endl;
	}
	else {
		if (__psv_SR.is_flying()) {
			if (c == 'w') {
				__psv_SR.camera_forwards(0.1f);
			}
			else if (c == 's') {
				__psv_SR.camera_backwards(0.1f);
			}
			else if (c == 'a') {
				__psv_SR.camera_sideways_left(0.1f);
			}
			else if (c == 'd') {
				__psv_SR.camera_sideways_right(0.1f);
			}
		}
	}
}

void __psv_interface(int argc, char *argv[]) {
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
	glutTimerFunc(1000.0f/__psv_FPS, __psv_timed_refresh, 0);

	glutMainLoop();
}
