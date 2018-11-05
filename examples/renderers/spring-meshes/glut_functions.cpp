#include "glut_functions.hpp"

// C++ includes
#include <iostream>
using namespace std;

// base includes
#include <base/include_gl.hpp>

namespace glut_functions {

	// key definitions
	#define ESC 27

	sim_renderer SR;
	int pressed_button;
	point last_mouse;
	bool lock_mouse;
	int window_id;
	timing::time_point sec;

	bool draw_box;

	bool display_fps;
	int FPS;
	int fps_count;

	void init_glut_variables() {
		pressed_button = 0;
		last_mouse = point(0,0);
		lock_mouse = false;

		draw_box = true;

		display_fps = false;
		FPS = 60;
		fps_count = 0;

		sec = timing::now();
	}

	// ---------------
	// SCENE RENDERING
	// ---------------

	void refresh() {
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		SR.apply_view_mode();

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		SR.apply_camera();

		SR.render_simulation();
		SR.apply_time_step();

		if (draw_box) {
			glDisable(GL_LIGHTING);
			glColor3f(1.0f,0.0f,0.0f);
			SR.get_box().draw_box();
		}

		glutSwapBuffers();
	}

	void timed_refresh(int value) {
		refresh();

		++fps_count;
		timing::time_point here = timing::now();
		double elapsed = timing::elapsed_seconds(sec, here);
		if (elapsed >= 1.0) {
			if (display_fps) {
				cout << "fps= " << fps_count << " (" << FPS << ")" << endl;
			}
			fps_count = 0;
			sec = timing::now();
		}

		glutTimerFunc(1000.0f/FPS, timed_refresh, value);
	}

	// ---------------
	// WINDOW RESIZING
	// ---------------

	void resize(int w,int h) {
		float pzoom = SR.get_perspective_camera().get_zoom();
		float ozoom = SR.get_orthogonal_camera().get_zoom();

		SR.set_window_dims(w, h);
		SR.init_cameras();

		SR.get_perspective_camera().set_zoom(pzoom);
		SR.get_orthogonal_camera().set_zoom(ozoom);

		glViewport(0, 0, w, h);
	}

	// -------------
	// MOUSE HANDLER
	// -------------

	void mouse_click_event(int button, int state, int x, int y) {
		UNUSED(x);
		UNUSED(y);

		if (SR.is_flying()) {
			if (button == GLUT_LEFT_BUTTON and state == 0) {
				if (lock_mouse) {
					lock_mouse = false;
					glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
				}
				else {
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

	void special_keys_keyboard(int key, int x, int y) {
		UNUSED(key);
		UNUSED(x);
		UNUSED(y);
	}

	void regular_keys_keyboard(unsigned char c, int x, int y) {
		UNUSED(x);
		UNUSED(y);

		switch (c) {
		case ESC: glutDestroyWindow(window_id); break;
		case 'p': SR.switch_to_perspective(); break;
		case 'o': SR.switch_to_orthogonal(); break;
		case 'b': draw_box = not draw_box; break;
		case 'h': help(); break;
		case 'z': display_fps = not display_fps; break;
		case 'i':
			SR.switch_to_inspection();
			lock_mouse = false;
			glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
			break;
		case 'f':
			SR.switch_to_flight();
			lock_mouse = true;
			glutWarpPointer(SR.window_width()/2,SR.window_height()/2);
			glutSetCursor(GLUT_CURSOR_NONE);
		case 'w':
			if (SR.is_flying()) {
				SR.camera_forwards(0.1f);
			}
			break;
		case 'a':
			if (SR.is_flying()) {
				SR.camera_sideways_left(0.1f);
			}
			break;
		case 's':
			if (SR.is_flying()) {
				SR.camera_backwards(0.1f);
			}
			break;
		case 'd':
			if (SR.is_flying()) {
				SR.camera_sideways_right(0.1f);
			}
			break;
		case '+':
			if (FPS < 60) {
				++FPS;
			}
			break;
		case '-':
			if (FPS > 1) {
				--FPS;
			}
			break;
		}
	}

	void help() {

	}

} // -- namespace glut_functions
