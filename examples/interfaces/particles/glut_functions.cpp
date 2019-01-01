#include "glut_functions.hpp"

// C++ includes
#include <iostream>
#include <memory>
using namespace std;

// C includes
#include <string.h>

// render includes
#include <render/include_gl.hpp>
#include <render/textures/texture_loader.hpp>
#include <render/shader/shader_helper.hpp>
#include <render/shader/shader.hpp>
#include <render/geometry/rsphere.hpp>

namespace glut_functions {

	void init_glut_variables() {
		wireframe_sphere = nullptr;

		lifetime = 10.0f;
		friction = 0.2f;
		bouncing = 0.8f;

		special_key = 0;
		pressed_button = 0;
		last_mouse_moved = point(0,0);
		lock_mouse = false;

		n_iterations = 10;
		time_step = 0.01f;
		draw_box = true;
		draw_boxes_octree = false;
		draw_sized_particles_wire = false;

		window_width = iw;
		window_height = ih;

		display_fps = false;
		FPS = 60;
		fps_count = 0;
		num_threads = 1;

		solver = physim::solver_type::Verlet;

		bgd_color = glm::vec3(0.0f,0.0f,0.0f);

		SR.get_simulator().set_solver(physim::solver_type::Verlet);

		particle_size = 1.0f;

		sec = timing::now();
	}

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

	void clear_simulation() {
		if (wireframe_sphere != nullptr) {
			wireframe_sphere->clear();
			delete wireframe_sphere;
			wireframe_sphere = nullptr;
		}
		SR.clear();
		texture_loader::get_loader().clear_all();
	}

	void finish_simulation() {
		clear_simulation();
		glutDestroyWindow(window_id);
	}

	void parse_common_params(int argc, char *argv[]) {
		for (int i = 2; i < argc; ++i) {
			if (strcmp(argv[i], "--friction") == 0) {
				friction = atof(argv[i + 1]);
				++i;
			}
			else if (strcmp(argv[i], "--bouncing") == 0) {
				bouncing = atof(argv[i + 1]);
				++i;
			}
			else if (strcmp(argv[i], "--lifetime") == 0) {
				lifetime = atof(argv[i + 1]);
				++i;
			}
			else if (strcmp(argv[i], "--threads") == 0) {
				num_threads = atoi(argv[i + 1]);
				++i;
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
					cout << "Error: invalid value for solver: '" << s << "'" << endl;
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
			SR.increment_psi(-0.3f*dx);
			SR.increment_theta(0.3f*dy);
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
		case 'b': draw_box = not draw_box; break;
		case 'z': display_fps = not display_fps; break;
		case 'w':
			draw_boxes_octree = not draw_boxes_octree;
			break;
		case '+':
			if (FPS < 60) {
				++FPS;
			}
			break;
		case '-':
			if (FPS > 1) {
				--FPS;
			}
			break;
		case 'g':
			cout << "Enter solver: ";
			cin >> option;
			if (option != "EulerOrig" and option != "EulerSemi" and option != "Verlet") {
				cout << "    Invalid value for solver '" << option << "'" << endl;
			}
			else {
				if (option == "EulerOrig") {
					SR.get_simulator().set_solver(physim::solver_type::EulerOrig);
					solver = physim::solver_type::EulerOrig;
				}
				else if (option == "EulerSemi") {
					SR.get_simulator().set_solver(physim::solver_type::EulerSemi);
					solver = physim::solver_type::EulerSemi;
				}
				else if (option == "Verlet") {
					SR.get_simulator().set_solver(physim::solver_type::Verlet);
					solver = physim::solver_type::Verlet;
				}
			}
			break;
		}

		if (GLUT_ACTIVE_SHIFT) {
			switch (c) {
			case 'P':
				particle_size += 0.5f;
				particle_size = (particle_size >= 10.0f ? 10.0f : particle_size);
				cout << "particle size set to: " << particle_size << endl;
				glPointSize(particle_size);
				break;
			}
		}
		if (GLUT_ACTIVE_CTRL) {
			c = c + 'a' - 1;
			// If 'p' is pressed then c = 1 + actual_char - 'a'
			// So: actual_char = c + 'a' - 1
			switch(c) {
			case 'p':
				particle_size -= 0.5f;
				particle_size = (particle_size <= 1.0f ? 1.0f : particle_size);
				cout << "particle size set to: " << particle_size << endl;
				glPointSize(particle_size);
				break;
			}
		}
	}

	void help() {
		cout << "Options common to all simulations:" << endl;
		cout << endl;
		cout << "    ESC: terminate the simulation" << endl;
		cout << "    o: switch to orthogonal camera." << endl;
		cout << "    p: switch to perspective camera." << endl;
		cout << "    b: turn on/off drawing the bounding box of the scene." << endl;
		cout << "    z: display fps count." << endl;
		cout << "    g: change simulation solver." << endl;
		cout << "        A message will be displayed 'Enter solver: '" << endl;
		cout << "        Then, write one of the following strings and press enter" << endl;
		cout << "        to set the solver:" << endl;
		cout << "            EulerOrig, EulerSemi, Verlet" << endl;
		cout << "        Default: Verlet" << endl;
		cout << "    r: reset simulation to its initial state" << endl;
		cout << "    w: display the octrees for the objects" << endl;
		cout << "    h: display the options available" << endl;
		cout << "    +: increase FPS limit by one (up to at most 60)" << endl;
		cout << "    -: decrease FPS limit by one (down to at least 1)" << endl;
		cout << endl;
		cout << "Special keys options:" << endl;
		cout << "    SHIFT + p: increment particle size" << endl;
		cout << "    CTRL + p: decrease particle size" << endl;
		cout << endl;
	}

} // -- namespace glut_functions
