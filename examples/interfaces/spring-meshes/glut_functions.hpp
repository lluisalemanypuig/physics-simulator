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
