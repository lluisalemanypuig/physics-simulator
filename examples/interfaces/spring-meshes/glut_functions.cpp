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

namespace glut_functions {

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

	bool draw_box;
	bool draw_boxes_octree;

	bool display_fps;
	int FPS;
	int fps_count;

	float friction = 0.2f;
	float bouncing = 0.8f;
	float damping = 0.5f;
	float elasticity = 500.0f;

	bool stretch;
	bool shear;
	bool bend;

	int n_iterations;
	physim::solver_type solver;

	physim::math::vec3 bgd_color;

	void init_glut_variables() {
		special_key = 0;
		pressed_button = 0;
		last_mouse_moved = point(0,0);
		lock_mouse = false;

		draw_box = true;
		draw_boxes_octree = false;

		display_fps = false;
		FPS = 60;
		fps_count = 0;

		stretch = true;
		shear = true;
		bend = true;

		solver = physim::solver_type::Verlet;

		bgd_color = physim::math::vec3(0.0f,0.0f,0.0f);

		SR.get_simulator().set_solver(physim::solver_type::Verlet);
		SR.get_simulator().set_time_step(0.001f);

		n_iterations = 10;
		use_shaders = false;

		sec = timing::now();
	}

	void init_shaders() {
		if (use_shaders) {
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
	}

	void clear_shaders() {
		if (use_shaders) {
			flat_shader.clear();
			texture_shader.clear();
		}
	}

	void finish_simulation() {
		SR.clear();
		texture_loader::get_loader().clear_all();
		glutDestroyWindow(window_id);
	}

	void parse_common_params(int argc, char *argv[]) {
		for (int i = 2; i < argc; ++i) {
			if (strcmp(argv[i], "--bend") == 0) {
				glut_functions::bend = true;
			}
			else if (strcmp(argv[i], "--shear") == 0) {
				glut_functions::shear = true;
			}
			else if (strcmp(argv[i], "--stretch") == 0) {
				glut_functions::stretch = true;
			}
			else if (strcmp(argv[i], "--Ke") == 0) {
				glut_functions::elasticity = atof(argv[i + 1]);
				++i;
			}
			else if (strcmp(argv[i], "--Kd") == 0) {
				glut_functions::damping = atof(argv[i + 1]);
				++i;
			}
			else if (strcmp(argv[i], "--use-shaders") == 0) {
				use_shaders = true;
			}
			else if (strcmp(argv[i], "--solver") == 0) {
				string s = string(argv[i + 1]);
				if (s == "exp-euler") {
					glut_functions::solver = physim::solver_type::EulerOrig;
				}
				else if (s == "semi-euler") {
					glut_functions::solver = physim::solver_type::EulerSemi;
				}
				else if (s == "verlet") {
					glut_functions::solver = physim::solver_type::Verlet;
				}
				else {
					cout << "Error: invalid value for solver: '" << s << "'"
						 << endl;
				}
				++i;
			}
		}
	}

	void set_internal_forces() {
		const vector<physim::meshes::mesh *>& ms =
			SR.get_simulator().get_meshes();
		for (physim::meshes::mesh *m : ms) {
			if (m->get_type() == physim::meshes::mesh_type::d1) {
				physim::meshes::mesh1d *m1 =
					static_cast<physim::meshes::mesh1d *>(m);
				m1->simulate_stretch(stretch);
				m1->simulate_bend(bend);
			}
			else if (m->get_type() == physim::meshes::mesh_type::d2_regular) {
				physim::meshes::mesh2d_regular *m2 =
					static_cast<physim::meshes::mesh2d_regular *>(m);
				m2->simulate_stretch(stretch);
				m2->simulate_shear(shear);
				m2->simulate_bend(bend);
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
	}

	// -------------
	// MOUSE HANDLER
	// -------------

	void mouse_click_event(int button, int state, int x, int y) {
		if (inside_window(x,y) and SR.is_flying()) {
			if (button == GLUT_LEFT_BUTTON and state == 0) {
				if (lock_mouse) {
					lock_mouse = false;
					glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
				}
				else {
					lock_mouse = true;
					glutWarpPointer(SR.window_width()/2,SR.window_height()/2);
					glutSetCursor(GLUT_CURSOR_NONE);
				}
			}
		}

		pressed_button = button;
		last_mouse_click = point(x,y);
	}

	void mouse_movement(int x, int y) {
		int dx = x - last_mouse_moved.first;
		int dy = y - last_mouse_moved.second;
		last_mouse_moved = point(x,y);

		if (inside_window(x, y) and SR.is_flying() and lock_mouse) {
			SR.increment_yaw(-0.2f*dx);
			SR.increment_pitch(-0.2f*dy);

			glutWarpPointer(SR.window_width()/2,SR.window_height()/2);
			last_mouse_moved = point(SR.window_width()/2,SR.window_height()/2);
		}
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
		case 'b': draw_box = not draw_box; break;
		case 'z': display_fps = not display_fps; break;
		/*case 'i':
			if (inside_window(x, y)) {
				SR.switch_to_inspection();
				lock_mouse = false;
				glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
			}
			break;
		case 'f':
			if (inside_window(x, y)) {
				SR.switch_to_flight();
				lock_mouse = true;
				glutWarpPointer(SR.window_width()/2,SR.window_height()/2);
				glutSetCursor(GLUT_CURSOR_NONE);
			}
			break;
		case 'w':
			if (inside_window(x, y) and SR.is_flying() and lock_mouse) {
				SR.camera_forwards(0.1f);
			}
			break;
		case 'a':
			if (inside_window(x, y) and SR.is_flying() and lock_mouse) {
				SR.camera_sideways_left(0.1f);
			}
			break;
		case 's':
			if (inside_window(x, y) and SR.is_flying() and lock_mouse) {
				SR.camera_backwards(0.1f);
			}
			break;
		case 'd':
			if (inside_window(x, y) and SR.is_flying() and lock_mouse) {
				SR.camera_sideways_right(0.1f);
			}
			break;*/
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
		case 't':
			stretch = not stretch;
			set_internal_forces();
			cout << "Stretch: " << (stretch ? "On" : "Off") << endl;
			break;
		case 'y':
			shear = not shear;
			set_internal_forces();
			cout << "Shear: " << (shear ? "On" : "Off") << endl;
			break;
		case 'u':
			bend = not bend;
			set_internal_forces();
			cout << "Bend: " << (bend ? "On" : "Off") << endl;
			break;
		case 'm':
			cout << "What do you want to change?" << endl;
			cout << "    elasticity, damping, friction, bouncing" << endl;
			cout << "    ";
			cin >> option;

			physim::simulator& S = SR.get_simulator();

			if (option == "elasticity") {
				cout << "    enter elasticity: ";
				cin >> elasticity;
				cout << "    Use elasticity coefficient: " << elasticity << endl;
				for (physim::meshes::mesh *m : S.get_meshes()) {
					m->set_elasticity(elasticity);
				}
			}
			else if (option == "damping") {
				cout << "    enter damping: ";
				cin >> damping;
				cout << "    Use damping factor: " << damping << endl;
				for (physim::meshes::mesh *m : S.get_meshes()) {
					m->set_damping(damping);
				}
			}
			else if (option == "friction") {
				cout << "    enter friction: ";
				cin >> friction;
				cout << "    Use friction coefficient: " << friction << endl;
				for (physim::meshes::mesh *m : S.get_meshes()) {
					m->set_friction(friction);
				}
			}
			else if (option == "bouncing") {
				cout << "    enter bouncing: ";
				cin >> bouncing;
				cout << "    Use bouncing coefficient: " << bouncing << endl;
				for (physim::meshes::mesh *m : S.get_meshes()) {
					m->set_bouncing(bouncing);
				}
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
		/*cout << "    i: switch to inspect mode." << endl;
		cout << "        Click and drag with:" << endl;
		cout << "        - left mouse button to rotate the scene." << endl;
		cout << "        - right mouse button to zoom in/out." << endl;
		cout << "    f: switch to flight mode." << endl;
		cout << "        Move the cursor to orient the movement of the camera." << endl;
		cout << "        Use the letters 'w','a','s','d' to move the camera." << endl;
		cout << "        When clicking 'f' the mouse becomes locked: left click to unlock." << endl;*/
		cout << "    g: change simulation solver." << endl;
		cout << "        A message will be displayed 'Enter solver: '" << endl;
		cout << "        Then, write one of the following strings and press enter" << endl;
		cout << "        to set the solver:" << endl;
		cout << "            EulerOrig, EulerSemi, Verlet" << endl;
		cout << "        Default: Verlet" << endl;
		cout << "    m: change properties of the mesh." << endl;
		cout << "        A message will be displayed 'Enter option: '" << endl;
		cout << "        Then, write one of the following options and its parameters:" << endl;
		cout << "            elasticity k: change elasticity of the springs of the mesh." << endl;
		cout << "                k is a floating point value. Default: 500" << endl;
		cout << "            damping d: change damping factor of the springs of the mesh." << endl;
		cout << "                d is a floating point value. Default: 0.5" << endl;
		cout << "            friction f: change friction coefficient of all meshes." << endl;
		cout << "                f is a floating point value. Default: 0.0" << endl;
		cout << "            bouncing b: change bouncing coefficient of all meshes." << endl;
		cout << "                b is a floating point value. Default: 0.8" << endl;
		cout << "    r: reset simulation to its initial state" << endl;
		cout << "    t: turn on/off stretch" << endl;
		cout << "    y: turn on/off shear" << endl;
		cout << "    u: turn on/off bend" << endl;
		cout << "    w: display the octrees for the objects" << endl;
		cout << "    h: display the options available" << endl;
		cout << "    +: increase FPS limit by one (up to at most 60)" << endl;
		cout << "    -: decrease FPS limit by one (down to at least 1)" << endl;
		cout << endl;
	}

} // -- namespace glut_functions
