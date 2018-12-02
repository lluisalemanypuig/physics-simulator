#pragma once

// C++ includes
#include <utility>

// physim includes
#include <physim/math/vec3.hpp>

// render includes
#include <render/scene/sim_renderer.hpp>
#include <render/shader/shader.hpp>

// custom includes
#include "utils.hpp"
#include "glut_variables.hpp"
using namespace glut_variables;

// initial window size
#define iw 640
#define ih 480

// key definitions
#define ESC_KEY 27
#define CTRL_KEY 114
#define SHIFT_KEY 112

namespace glut_functions {
	/* ---------------- */
	/* helper functions */

	static inline
	bool inside_window(int x, int y) {
		int w = SR.window_width();
		int h = SR.window_height();
		return ((0 <= x) and (x <= w))
		   and ((0 <= y) and (y <= h));
	}

	/* --------------------- */
	/* actual glut functions */

	void init_glut_variables();
	void init_shaders();
	void clear_shaders();

	void clear_simulation();
	void finish_simulation();
	void parse_common_params(int argc, char *argv[]);

	/* scene rendering */
	void shader_render();
	void no_shader_render();
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
