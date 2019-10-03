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
#include <limits>
using namespace std;

// physim includes
#include <physim/math/vec3.hpp>
#include <physim/structures/octree.hpp>
#include <physim/fluids/newtonian.hpp>

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
typedef physim::particles::fluid_particle fluid_particle;
typedef pair<pmvec3, pmvec3> pmbox;

// ------------------
// global variables
// ------------------

static pmvec3 __fv_vmin;
static pmvec3 __fv_vmax;
static vector<pmbox> __fv_tree_boxes;
static size_t __fv_slice;
static physim::fluids::fluid *__fv_F;
static vector<uint> __fv_fluid_points_idxs;
static physim::structures::octree __fv_tree;
static vector<size_t> __fv_query_indices;
static pmvec3 __fv_query_point;
static bool __fv_valid_query;
static float __fv_query_radius;

static viewer __fv_SR;

static timing::time_point __fv_sec;
static int __fv_FPS;
static int __fv_fps_count;
static bool __fv_display_fps_count;

static int __fv_pressed_button;
static point __fv_last_mouse;

static bool __fv_lock_mouse;
static int __fv_window_id;

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

void __fv_exit_func() {

}

// -----------------
// INITIALISE OpenGL
// -----------------

void __fv_initGL(int argc, char *argv[]) {

	// initial window size
	int iw = 640;
	int ih = 480;

	// ----------------- //
	/* initialise window */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(50, 25);
	glutInitWindowSize(iw, ih);
	__fv_window_id = glutCreateWindow("Basic viewer");

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
	__fv_pressed_button = 0;
	__fv_last_mouse = point(0,0);
	__fv_lock_mouse = false;

	__fv_display_fps_count = false;
	__fv_FPS = 60;
	__fv_fps_count = 0;

	__fv_slice = 16;
	size_t N = __fv_slice*__fv_slice*__fv_slice;
	__fv_F = new physim::fluids::newtonian();
	__fv_F->allocate(N, 1, 1, 1, 1, 1);

	__fv_fluid_points_idxs.resize(N);
	std::iota(__fv_fluid_points_idxs.begin(), __fv_fluid_points_idxs.end(), 0);

	__fv_tree.get_boxes(__fv_tree_boxes);
	__fv_vmin = pmvec3(numeric_limits<float>::max());
	__fv_vmax = -numeric_limits<float>::max();

	for (size_t i = 0; i < __fv_slice; ++i) {
		for (size_t j = 0; j < __fv_slice; ++j) {
			for (size_t k = 0; k < __fv_slice; ++k) {
				pmvec3 pos = pmvec3(
					i*(0.5f/__fv_slice), j*(0.5f/__fv_slice), k*(0.5f/__fv_slice)
				);

				size_t idx = i*__fv_slice*__fv_slice + j*__fv_slice + k;
				__fv_F->get_particles()[idx].cur_pos = pos;

				physim::math::min(__fv_vmin, pos, __fv_vmin);
				physim::math::max(__fv_vmax, pos, __fv_vmax);
			}
		}
	}

	// compute offset of position w.r.t. to the beginning of its struct
	const fluid_particle *ps = __fv_F->get_particles();

	__fv_tree.init(&ps[0].cur_pos, N, sizeof(fluid_particle), 4);
	__fv_tree.get_boxes(__fv_tree_boxes);

	__fv_valid_query = false;

	cout << "Minimum: " << __fv_vmin.x << "," << __fv_vmin.y << "," << __fv_vmin.z << endl;
	cout << "Maximum: " << __fv_vmax.x << "," << __fv_vmax.y << "," << __fv_vmax.z << endl;

	__fv_SR.set_window_dims(iw, ih);
	__fv_SR.get_box().set_min_max
	(
		glm::vec3(__fv_vmin.x,__fv_vmin.y,__fv_vmin.z),
		glm::vec3(__fv_vmax.x,__fv_vmax.y,__fv_vmax.z)
	);
	__fv_SR.init_cameras();

	glDisable(GL_LIGHTING);

	__fv_sec = timing::now();
}

// ------------
// RENDER SCENE
// ------------

void __fv_refresh() {
	++__fv_fps_count;

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	__fv_SR.apply_projection();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	__fv_SR.apply_view();

	if (__fv_F->size() > 0) {
		glPointSize(5);
		glColor3f(1,0,0);

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer
		(3, GL_FLOAT, sizeof(physim::particles::fluid_particle),
		 &__fv_F->get_particles()[0].cur_pos.x);

		glDrawElements
		(GL_POINTS, __fv_fluid_points_idxs.size(),
		 GL_UNSIGNED_INT, &__fv_fluid_points_idxs[0]);

		glDisableClientState(GL_VERTEX_ARRAY);

		/*
		glBegin(GL_POINTS);
		for (const pmvec3& p : file_points) {
			glVertex3f(p.x, p.y, p.z);
		}
		glEnd();
		*/
	}

	glColor3f(0.3f,0.3f,1.0f);
	draw_box(pmbox(__fv_vmin, __fv_vmax));

	if (__fv_tree_boxes.size() > 0) {
		glColor3f(0.3f,0.3f,1.0f);
		for (const pmbox& B : __fv_tree_boxes) {
			draw_box(B);
		}
	}

	if (__fv_valid_query) {
		glColor3f(1,1,1);
		glBegin(GL_POINTS);
		if (__fv_query_indices.size() > 0) {
			glPointSize(10);
			for (size_t idx : __fv_query_indices) {
				const pmvec3& p = __fv_F->get_particles()[idx].cur_pos;
				glVertex3f(p.x + 0.01f, p.y + 0.01f, p.z + 0.01f);
			}
		}
		glColor3f(1,1,0);
		glVertex3f(__fv_query_point.x, __fv_query_point.y, __fv_query_point.z);
		glEnd();

		glPushMatrix();
			glTranslatef(__fv_query_point.x, __fv_query_point.y, __fv_query_point.z);
			glutWireSphere(__fv_query_radius, 10, 10);
		glPopMatrix();
	}

	glutSwapBuffers();
}

void __fv_timed_refresh(int value) {
	UNUSED(value);

	__fv_refresh();

	timing::time_point here = timing::now();
	double elapsed = timing::elapsed_seconds(__fv_sec, here);
	if (elapsed >= 1.0) {
		if (__fv_display_fps_count) {
			cout << "fps " << __fv_fps_count << " (" << __fv_FPS << ")" << endl;
		}
		__fv_fps_count = 0;
		__fv_sec = timing::now();
	}

	glutTimerFunc(1000.0f/__fv_FPS, __fv_timed_refresh, 0);
}

// ---------------
// RESIZE viewport
// ---------------

void __fv_reshape(int w, int h) {
	float pzoom = __fv_SR.get_perspective_camera().get_zoom();
	float ozoom = __fv_SR.get_orthogonal_camera().get_zoom();

	__fv_SR.set_window_dims(w, h);
	__fv_SR.init_cameras();

	__fv_SR.get_perspective_camera().set_zoom(pzoom);
	__fv_SR.get_orthogonal_camera().set_zoom(ozoom);

	glViewport(0, 0, w, h);
}

// ++++++++++++++++++
// EVENTS
// ++++++++++++++++++

// -------------
// MOUSE HANDLER
// -------------

void __fv_mouse_click_event(int button, int state, int x, int y) {
	UNUSED(x);
	UNUSED(y);

	if (__fv_SR.is_flying()) {
		cout << "Flying..." << endl;
		if (button == GLUT_LEFT_BUTTON and state == 0) {
			cout << "Left click... " << state << endl;
			cout << "    Is mouse locked? " << (__fv_lock_mouse ? "Yes" : "No") << endl;
			if (__fv_lock_mouse) {
				cout << "    Unlock mouse" << endl;
				__fv_lock_mouse = false;
				glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
			}
			else {
				cout << "    Lock mouse" << endl;
				__fv_lock_mouse = true;
				glutWarpPointer(__fv_SR.window_width()/2,__fv_SR.window_height()/2);
				glutSetCursor(GLUT_CURSOR_NONE);
			}
		}
	}
	__fv_pressed_button = button;
}

void __fv_mouse_movement(int x, int y) {
	int dx = x - __fv_last_mouse.first;
	int dy = y - __fv_last_mouse.second;
	__fv_last_mouse = point(x,y);

	if (__fv_SR.is_flying() and __fv_lock_mouse) {
		__fv_SR.increment_yaw(-0.2f*dx);
		__fv_SR.increment_pitch(-0.2f*dy);

		glutWarpPointer(__fv_SR.window_width()/2,__fv_SR.window_height()/2);
		__fv_last_mouse = point(__fv_SR.window_width()/2,__fv_SR.window_height()/2);
	}
}

void __fv_mouse_drag_event(int x, int y) {
	int dx = x - __fv_last_mouse.first;
	int dy = y - __fv_last_mouse.second;
	__fv_last_mouse = point(x,y);

	if (__fv_pressed_button == GLUT_LEFT_BUTTON) {
		if (__fv_SR.is_inspecting()) {
			__fv_SR.increment_psi(-0.3f*dx);
			__fv_SR.increment_theta(0.3f*dy);
		}
	}
	else if (__fv_pressed_button == GLUT_RIGHT_BUTTON) {
		__fv_SR.increment_zoom(0.75f*dy);
	}
}

// ----------------
// KEYBOARD HANDLER
// ----------------

void __fv_special_keys(int key, int x, int y) {
	UNUSED(key);
	UNUSED(x);
	UNUSED(y);
}

void __fv_keyboard_event(unsigned char c, int x, int y) {
	UNUSED(x);
	UNUSED(y);

	if (c == ESC) {
		glutDestroyWindow(__fv_window_id);
	}
	else if (c == 'p') {
		__fv_SR.switch_to_perspective();
	}
	else if (c == 'o') {
		__fv_SR.switch_to_orthogonal();
	}
	else if (c == 'i') {
		__fv_SR.switch_to_inspection();
		__fv_lock_mouse = false;
		glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
	}
	else if (c == 'f') {
		__fv_SR.switch_to_flight();
		__fv_lock_mouse = true;
		glutWarpPointer(__fv_SR.window_width()/2,__fv_SR.window_height()/2);
		glutSetCursor(GLUT_CURSOR_NONE);
	}
	else if (c == '+') {
		if (__fv_FPS < 59) {
			++__fv_FPS;
		}
	}
	else if (c == '-') {
		if (__fv_FPS > 1) {
			--__fv_FPS;
		}
	}
	else if (c == 'x') {
		__fv_display_fps_count = not __fv_display_fps_count;
	}
	else if (c == 'q') {
		cout << "Enter query point:" << endl;
		cin >> __fv_query_point.x >> __fv_query_point.y >> __fv_query_point.z;
		cout << "Enter radius:" << endl;
		cin >> __fv_query_radius;
		__fv_valid_query = true;

		__fv_query_indices.clear();
		__fv_tree.get_indices(__fv_query_point, __fv_query_radius, __fv_query_indices);

		cout << "Result of query:" << endl;
		cout << "    # indices retrieved: " << __fv_query_indices.size() << endl;
		cout << "    indices:";
		for (size_t idx : __fv_query_indices) {
			cout << " " << idx;
		}
		cout << endl;
	}
	else {
		if (__fv_SR.is_flying()) {
			if (c == 'w') {
				__fv_SR.camera_forwards(0.1f);
			}
			else if (c == 's') {
				__fv_SR.camera_backwards(0.1f);
			}
			else if (c == 'a') {
				__fv_SR.camera_sideways_left(0.1f);
			}
			else if (c == 'd') {
				__fv_SR.camera_sideways_right(0.1f);
			}
		}
	}
}

void __fv_interface(int argc, char *argv[]) {
	__fv_initGL(argc, argv);

	atexit(__fv_exit_func);

	glutDisplayFunc(__fv_refresh);
	glutReshapeFunc(__fv_reshape);
	glutKeyboardFunc(__fv_keyboard_event);
	glutSpecialFunc(__fv_special_keys);
	glutMouseFunc(__fv_mouse_click_event);
	glutPassiveMotionFunc(__fv_mouse_movement);
	glutMotionFunc(__fv_mouse_drag_event);

	//glutIdleFunc(refresh);
	glutTimerFunc(1000.0f/__fv_FPS, __fv_timed_refresh, 0);

	glutMainLoop();
}
