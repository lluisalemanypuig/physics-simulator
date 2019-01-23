#include "glut_variables.hpp"

namespace glut_variables {

rendered_triangle_mesh *wireframe_sphere;
shader texture_shader;
shader flat_shader;

sim_renderer SR;
int window_id;
int window_width = 640;
int window_height = 480;
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
size_t num_threads;

float lifetime;
float friction;
float bouncing;

physim::solver_type solver;
glm::vec3 bgd_color;

bool run;
bool record;

} // -- namespace glut_variables
