#pragma once

// C++ includes
#include <utility>

// render includes
#include <render/scene/sim_renderer.hpp>
#include <render/triangle_mesh/rendered_triangle_mesh.hpp>
#include <render/shader/shader.hpp>

// custom includes
#include "utils.hpp"

namespace glut_variables {

	typedef std::pair<int,int> point;

	// 'global' variables

	extern shader texture_shader;
	extern shader flat_shader;

	extern sim_renderer SR;
	extern int window_id;
	extern int window_width;
	extern int window_height;
	extern timing::time_point sec;

	extern int special_key;
	extern int pressed_button;
	extern bool lock_mouse;

	extern point last_mouse_moved;
	extern point last_mouse_click;
	extern point special_key_pressed;
	extern point regular_key_pressed;

	extern bool display_fps;
	extern int FPS;
	extern int fps_count;
	extern size_t num_threads;

	extern float length_x, length_y, length_z;
	extern float density;
	extern float viscosity;
	extern float h;
	extern float cs;

	extern physim::solver_type solver;

	extern physim::math::vec3 bgd_color;

	extern bool record;

	void init_variables();

} // -- namespace glut_variables
