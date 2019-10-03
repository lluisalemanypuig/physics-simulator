/*********************************************************************
 * Real-time physics simulation project
 * Copyright (C) 2018-2019 Lluís Alemany Puig
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * Contact: Lluís Alemany Puig (lluis.alemany.puig@gmail.com)
 * 
 ********************************************************************/

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
