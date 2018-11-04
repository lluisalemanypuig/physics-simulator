#pragma once

// C++ includes
#include <utility>

// base includes
#include <base/scene/sim_renderer.hpp>

// custom includes
#include "utils.hpp"

namespace glut_functions {

	typedef std::pair<int,int> point;

	// 'global' variables
	extern sim_renderer SR;
	extern int pressed_button;
	extern point last_mouse;
	extern bool lock_mouse;
	extern int window_id;
	extern int FPS;
	extern int fps_count;
	extern timing::time_point tp;

	// helper functions
	static inline
	bool inside_window(int x, int y, int w, int h) {
		return ((0 <= x) and (x <= w))
		   and ((0 <= y) and (y <= h));
	}

	template<typename T>
	static inline void UNUSED(const T& x) { (void)x; }

	// actual glut functions
	void refresh();
	void resize(int w,int h);
	void mouse_click_event(int b, int s, int x, int y);
	void mouse_movement(int x, int y);
	void mouse_drag_event(int x, int y);

} // -- namespace glut_functions
