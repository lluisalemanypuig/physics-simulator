#pragma once

// C++ includes
#include <utility>

// physim includes
#include <physim/math/vec3.hpp>

// base includes
#include <base/scene/sim_renderer.hpp>
#include <base/shader.hpp>

// custom includes
#include "utils.hpp"

// initial window size
#define iw 640
#define ih 480

// key definitions
#define ESC_KEY 27
#define CTRL_KEY 114
#define SHIFT_KEY 112

namespace glut_functions {

	typedef std::pair<int,int> point;

	// 'global' variables

	extern shader texture_shader;
	extern shader flat_shader;
	extern bool use_shaders;

	extern sim_renderer SR;
	extern int window_id;
	extern timing::time_point sec;

	extern int special_key;
	extern int pressed_button;
	extern bool lock_mouse;

	extern point last_mouse_moved;
	extern point last_mouse_click;
	extern point special_key_pressed;
	extern point regular_key_pressed;

	extern bool draw_box;

	extern bool display_fps;
	extern int FPS;
	extern int fps_count;

	extern float friction;
	extern float bouncing;
	extern float damping;
	extern float elasticity;

	extern bool stretch;
	extern bool shear;
	extern bool bend;

	extern physim::solver_type solver;

	extern physim::math::vec3 bgd_color;

	/* ---------------- */
	/* helper functions */

	static inline
	bool inside_window(int x, int y) {
		int w = SR.window_width();
		int h = SR.window_height();
		return ((0 <= x) and (x <= w))
		   and ((0 <= y) and (y <= h));
	}

	void set_internal_forces();

	/* --------------------- */
	/* actual glut functions */

	void init_glut_variables();
	void init_shaders();
	void clear_shaders();

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
