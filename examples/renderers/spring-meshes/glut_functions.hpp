#pragma once

// C++ includes
#include <utility>

// base includes
#include <base/scene/sim_renderer.hpp>

// custom includes
#include "utils.hpp"

// initial window size
#define iw 640
#define ih 480

namespace glut_functions {

	typedef std::pair<int,int> point;

	// 'global' variables
	extern sim_renderer SR;
	extern int pressed_button;
	extern point last_mouse;
	extern bool lock_mouse;
	extern int window_id;
	extern timing::time_point sec;

	extern bool draw_box;

	extern bool display_fps;
	extern int FPS;
	extern int fps_count;

	// helper functions
	static inline
	bool inside_window(int x, int y, int w, int h) {
		return ((0 <= x) and (x <= w))
		   and ((0 <= y) and (y <= h));
	}

	template<typename T>
	static inline void UNUSED(const T& x) { (void)x; }

	/* --------------------- */
	/* actual glut functions */

	void init_glut_variables();

	/* scene rendering */
	void refresh();
	void timed_refresh(int value);

	/* window resizing */
	void resize(int w,int h);

	/* event handling */

	// mouse
	void mouse_click_event(int b, int s, int x, int y);
	void mouse_movement(int x, int y);
	void mouse_drag_event(int x, int y);
	// keyboard
	void special_keys_keyboard(int key, int x, int y);
	void regular_keys_keyboard(unsigned char c, int x, int y);

	/* help display */
	void help();

} // -- namespace glut_functions
