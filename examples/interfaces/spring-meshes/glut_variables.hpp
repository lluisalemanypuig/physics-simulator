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
