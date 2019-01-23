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

extern bool draw_box;
extern bool draw_boxes_octree;

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

extern int n_iterations;
extern physim::solver_type solver;

extern physim::math::vec3 bgd_color;

extern bool run;
extern bool record;

} // -- namespace glut_variables
