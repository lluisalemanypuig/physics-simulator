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

#include "glut_functions.hpp"

// C++ includes
#include <iostream>
#include <memory>
using namespace std;

// C includes
#include <string.h>

// glm includes
#include <glm/gtc/matrix_inverse.hpp>

// render includes
#include <render/include_gl.hpp>
#include <render/textures/texture_loader.hpp>
#include <render/shader/shader_helper.hpp>
#include <render/shader/shader.hpp>
#include <render/geometry/rsphere.hpp>

// physim includes
#include <physim/meshes/mesh.hpp>
#include <physim/meshes/mesh1d.hpp>
#include <physim/meshes/mesh2d_regular.hpp>

#include "glut_variables.hpp"
using namespace glut_variables;

namespace glut_functions {

void init_shaders() {
	bool r;
	r = texture_shader.init
		("../../interfaces/shaders", "textures.vert", "textures.frag");
	if (not r) {
		exit(1);
	}
	r = flat_shader.init
		("../../interfaces/shaders", "flat.vert", "flat.frag");
	if (not r) {
		exit(1);
	}

	texture_shader.bind();
	texture_shader.set_vec3("light.diffuse", glm::vec3(1.0f,1.0f,1.0f));
	texture_shader.set_vec3("light.ambient", glm::vec3(0.2f,0.2f,0.2f));
	texture_shader.set_vec3("light.position", glm::vec3(0.f,0.f,0.f));
	texture_shader.release();
}

void clear_shaders() {
	flat_shader.clear();
	texture_shader.clear();
}

void finish_simulation() {
	SR.clear();
	texture_loader::get_loader().clear_all();
	glutDestroyWindow(window_id);
}

void parse_common_params(int argc, char *argv[]) {
	for (int i = 2; i < argc; ++i) {
		if (strcmp(argv[i], "--nt") == 0) {
			num_threads = atoi(argv[i + 1]);
			++i;
		}
		else if (strcmp(argv[i], "--lenx") == 0) {
			lenx = atof(argv[i + 1]);
			++i;
		}
		else if (strcmp(argv[i], "--leny") == 0) {
			leny = atof(argv[i + 1]);
			++i;
		}
		else if (strcmp(argv[i], "--lenz") == 0) {
			lenz = atof(argv[i + 1]);
			++i;
		}
		else if (strcmp(argv[i], "--sidex") == 0) {
			sidex = atoi(argv[i + 1]);
			++i;
		}
		else if (strcmp(argv[i], "--sidey") == 0) {
			sidey = atoi(argv[i + 1]);
			++i;
		}
		else if (strcmp(argv[i], "--sidez") == 0) {
			sidez = atoi(argv[i + 1]);
			++i;
		}
		else if (strcmp(argv[i], "--dens") == 0) {
			density = atof(argv[i + 1]);
			++i;
		}
		else if (strcmp(argv[i], "--visc") == 0) {
			viscosity = atof(argv[i + 1]);
			++i;
		}
		else if (strcmp(argv[i], "--radius") == 0) {
			h = atof(argv[i + 1]);
			++i;
		}
		else if (strcmp(argv[i], "--cs") == 0) {
			cs = atof(argv[i + 1]);
			++i;
		}
		else if (strcmp(argv[i], "--dt") == 0) {
			dt = atof(argv[i + 1]);
			++i;
		}
		else if (strcmp(argv[i], "--it") == 0) {
			n_iterations = atoi(argv[i + 1]);
			++i;
		}
		else if (strcmp(argv[i], "--dens-kernel") == 0) {
			dens_kernel_name = string(argv[i + 1]);
			auto it = allowed_dens_kernel_names.find(dens_kernel_name);
			if (it == allowed_dens_kernel_names.end()) {
				cerr << "Error: density kernel '" << dens_kernel_name << "' not recognised" << endl;
				cerr << "    Choosing poly6" << endl;
				dens_kernel_name = "poly6";
			}
		}
		else if (strcmp(argv[i], "--press-kernel") == 0) {
			press_kernel_name = string(argv[i + 1]);
			auto it = allowed_press_kernel_names.find(press_kernel_name);
			if (it == allowed_press_kernel_names.end()) {
				cerr << "Error: pressure kernel '" << press_kernel_name << "' not recognised" << endl;
				cerr << "    Choosing poly6" << endl;
				press_kernel_name = "poly6";
			}
		}
		else if (strcmp(argv[i], "--visc-kernel") == 0) {
			visc_kernel_name = string(argv[i + 1]);
			auto it = allowed_visc_kernel_names.find(visc_kernel_name);
			if (it == allowed_visc_kernel_names.end()) {
				cerr << "Error: viscosity kernel '" << visc_kernel_name << "' not recognised" << endl;
				cerr << "    Choosing poly6" << endl;
				visc_kernel_name = "poly6";
			}
		}
		else if (strcmp(argv[i], "--solver") == 0) {
			string s = string(argv[i + 1]);
			if (s == "exp-euler") {
				solver = physim::solver_type::EulerOrig;
			}
			else if (s == "semi-euler") {
				solver = physim::solver_type::EulerSemi;
			}
			else if (s == "verlet") {
				solver = physim::solver_type::Verlet;
			}
			else {
				cout << "Error: invalid value for solver: '" << s << "'"
					 << endl;
			}
			++i;
		}
	}
}

// ---------------
// WINDOW RESIZING
// ---------------

void resize(int w,int h) {
	float pzoom = SR.get_perspective_camera().get_zoom();
	float ozoom = SR.get_orthogonal_camera().get_zoom();

	SR.set_window_dims(w, h);
	SR.init_cameras();

	SR.get_perspective_camera().set_zoom(pzoom);
	SR.get_orthogonal_camera().set_zoom(ozoom);

	glViewport(0, 0, w, h);

	window_width = w;
	window_height = h;
}

// -------------
// MOUSE HANDLER
// -------------

void mouse_click_event(int button, int state, int x, int y) {

	pressed_button = button;
	last_mouse_click = point(x,y);
}

void mouse_movement(int x, int y) {
	int dx = x - last_mouse_moved.first;
	int dy = y - last_mouse_moved.second;
	last_mouse_moved = point(x,y);

}

void mouse_drag_event(int x, int y) {
	int dx = x - last_mouse_moved.first;
	int dy = y - last_mouse_moved.second;
	last_mouse_moved = point(x,y);

	if (pressed_button == GLUT_LEFT_BUTTON) {
		if (SR.is_inspecting()) {
			SR.increment_psi(-0.3f*dx);
			SR.increment_theta(0.3f*dy);
		}
	}
	else if (pressed_button == GLUT_RIGHT_BUTTON) {
		SR.increment_zoom(0.75f*dy);
	}
}

// ----------------
// KEYBOARD HANDLER
// ----------------

void special_keys_keyboard(int key, int x, int y) {
	special_key_pressed = point(x,y);
	special_key = key;
}

void regular_keys_keyboard(unsigned char c, int x, int y) {
	regular_key_pressed = point(x,y);

	string option;

	switch (c) {
	case ESC_KEY:
		finish_simulation();
		break;
	case 'p': SR.switch_to_perspective(); break;
	case 'o': SR.switch_to_orthogonal(); break;
	case 'z': display_fps = not display_fps; break;
	case '+': if (FPS < 60) { ++FPS; } break;
	case '-': if (FPS > 1)  { --FPS; } break;
	case 'w': draw_boxes_octree = not draw_boxes_octree; break;
	case 'g':
		record = not record;
		if (record) {
			cout << "Start recording!" << endl;
		}
		else {
			cout << "STOP recording!" << endl;
		}
		break;
	case 'x':
		run = not run;
		if (run) {
			cout << "Start simulation!" << endl;
		}
		else {
			cout << "STOP simulation!" << endl;
		}
		break;
	default:
		;
	}
}

void help() {
	cout << "Options common to all simulations:" << endl;
	cout << endl;
	cout << "    ESC: terminate the simulation" << endl;
	cout << "    o: switch to orthogonal camera." << endl;
	cout << "    p: switch to perspective camera." << endl;
	cout << "    z: display fps count." << endl;
	cout << "    r: reset simulation to its initial state" << endl;
	cout << "    h: display the options available" << endl;
	cout << "    +: increase FPS limit by one (up to at most 60)" << endl;
	cout << "    -: decrease FPS limit by one (down to at least 1)" << endl;
	cout << "    g: start/stop record video" << endl;
	cout << "    x: start/stop simulation" << endl;
	cout << endl;
	cout << "Parameters:" << endl;
	cout << "    --dt t" << endl;
	cout << "    --it n" << endl;
	cout << "    --nt n" << endl;
	cout << "    --lenx x" << endl;
	cout << "    --leny y" << endl;
	cout << "    --lenz z" << endl;
	cout << "    --sidex x" << endl;
	cout << "    --sidey y" << endl;
	cout << "    --sidez z" << endl;
	cout << "    --dens d" << endl;
	cout << "    --visc v" << endl;
	cout << "    --radius r" << endl;
	cout << "    --cs c" << endl;
	cout << "    --solver s" << endl;
	cout << "        exp-euler" << endl;
	cout << "        semi-euler" << endl;
	cout << "        verlet" << endl;
	cout << "    --dens-kernel  {poly6, spline}" << endl;
	cout << "    --press-kernel {poly6, spiky}" << endl;
	cout << "    --visc-kernel  {poly6, spiky}" << endl;
	cout << endl;
}

} // -- namespace glut_functions
