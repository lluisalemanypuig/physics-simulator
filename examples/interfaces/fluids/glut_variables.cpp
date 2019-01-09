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

	float dt;
	float length_x, length_y, length_z;
	size_t side_x, side_y, side_z;
	float density;
	float viscosity;
	float h;
	float cs;

	physim::solver_type solver;

	physim::math::vec3 bgd_color;

	bool record;

	void init_variables() {
		special_key = 0;
		pressed_button = 0;
		last_mouse_moved = point(0,0);
		lock_mouse = false;

		display_fps = false;
		FPS = 60;
		fps_count = 0;

		solver = physim::solver_type::Verlet;

		bgd_color = physim::math::vec3(0.0f,0.0f,0.0f);

		window_width = 640;
		window_height = 480;

		num_threads = 1;

		dt = 0.01f;
		side_x = side_y = side_z = 16;
		length_x = length_y = length_z = 0.5f;
		h = 0.03f;
		viscosity = 0.001f;
		density = 1000.0f;
		cs = 1500.0f;

		record = false;

		sec = timing::now();
	}

} // -- namespace glut_variables
