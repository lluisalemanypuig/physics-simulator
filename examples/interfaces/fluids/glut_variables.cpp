#include "glut_variables.hpp"

namespace glut_variables {

	shader texture_shader;
	shader flat_shader;

	sim_renderer SR;
	int window_id;
	int window_width;
	int window_height;
	timing::time_point sec;

	int special_key;
	int pressed_button;
	bool lock_mouse;

	point last_mouse_moved;
	point last_mouse_click;
	point special_key_pressed;
	point regular_key_pressed;

	bool display_fps;
	int FPS;
	int fps_count;
	size_t num_threads;

	float volume;
	float density;
	float viscosity;
	float h;
	float cs;

	int n_iterations;
	physim::solver_type solver;

	physim::math::vec3 bgd_color;

} // -- namespace glut_variables
