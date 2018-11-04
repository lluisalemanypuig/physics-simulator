#include "glut_functions.hpp"

// C++ includes
#include <iostream>
using namespace std;

// base includes
#include <base/include_gl.hpp>

namespace glut_functions {

	// 'global' variables
	sim_renderer SR;
	int pressed_button;
	point last_mouse;
	bool lock_mouse;
	int window_id;
	int FPS;
	int fps_count;
	timing::time_point tp;

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

		glDisable(GL_LIGHTING);
		glColor3f(1.0f,0.0f,0.0f);
		SR.get_box().draw_box();

		glutSwapBuffers();

		timing::time_point end = timing::now();

		++fps_count;
		double sec = timing::elapsed_seconds(tp, end);
		if (sec >= 1.0) {
			cout << "fps: " << fps_count << " (max: " << FPS << ")" << endl;
			fps_count = 0;
			tp = timing::now();
		}
	}

	void resize(int w,int h) {
		float pzoom = SR.get_perspective_camera().get_zoom();
		float ozoom = SR.get_orthogonal_camera().get_zoom();

		SR.set_window_dims(w, h);
		SR.init_cameras();

		SR.get_perspective_camera().set_zoom(pzoom);
		SR.get_orthogonal_camera().set_zoom(ozoom);

		glViewport(0, 0, w, h);
	}

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

} // -- namespace glut_functions
