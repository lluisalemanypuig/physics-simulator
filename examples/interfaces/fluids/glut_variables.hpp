#pragma once

// C++ includes
#include <utility>
#include <set>

// physim includes
#include <physim/fluids/fluid.hpp>
#include <physim/fluids/kernel_function.hpp>

// render includes
#include <render/scene/sim_renderer.hpp>
#include <render/triangle_mesh/rendered_triangle_mesh.hpp>
#include <render/shader/shader.hpp>

// custom includes
#include "kernels.hpp"
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
extern size_t n_iterations;

extern std::set<std::string> allowed_dens_kernel_names;
extern std::set<std::string> allowed_press_kernel_names;
extern std::set<std::string> allowed_visc_kernel_names;

extern std::string dens_kernel_name;
extern std::string press_kernel_name;
extern std::string visc_kernel_name;

extern physim::fluids::kernel_scalar_function density_kernel;
extern physim::fluids::kernel_vectorial_function pressure_kernel;
extern physim::fluids::kernel_scalar_function viscosity_kernel;

extern float dt;
extern float lenx, leny, lenz;
extern size_t sidex, sidey, sidez;
extern float density;
extern float viscosity;
extern float h;
extern float cs;

extern physim::solver_type solver;

extern physim::math::vec3 bgd_color;

extern bool run;
extern bool record;

void init_variables();
void simulation_info(const physim::fluids::fluid *F);
void make_kernels();

} // -- namespace glut_variables
