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

void __fv_exit_func();
void __fv_initGL(int argc, char *argv[]);
void __fv_refresh();
void __fv_timed_refresh(int value);
void __fv_reshape(int w, int h);
void __fv_mouse_click_event(int button, int state, int x, int y);
void __fv_mouse_movement(int x, int y);
void __fv_mouse_drag_event(int x, int y);
void __fv_special_keys(int key, int x, int y);
void __fv_keyboard_event(unsigned char c, int x, int y);

void __fv_interface(int argc, char *argv[]);
