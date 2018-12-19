#include "glut_variables.hpp"

namespace glut_variables {

	rendered_triangle_mesh *wireframe_sphere;
	shader texture_shader;
	shader flat_shader;
	bool use_shaders;

	sim_renderer SR;
	int window_id;
	timing::time_point sec;

	int special_key;
	int pressed_button;
	bool lock_mouse;

	point last_mouse_moved;
	point last_mouse_click;
	point special_key_pressed;
	point regular_key_pressed;

	int n_iterations;
	float time_step;
	float particle_size;
	bool draw_box;
	bool draw_boxes_octree;
	bool draw_sized_particles_wire;

	bool display_fps;
	int FPS;
	int fps_count;

	float lifetime;
	float friction;
	float bouncing;

	physim::solver_type solver;
	glm::vec3 bgd_color;

} // -- namespace glut_variables
