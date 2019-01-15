#include "glut_variables.hpp"

// C++ includes
#include <iostream>
using namespace std;

// physim includes
#include <physim/simulator.hpp>
using namespace physim;

namespace glut_variables {

shader texture_shader;
shader flat_shader;

sim_renderer SR;
int window_id;
int window_width;
int window_height;
timing::time_point sec;

int special_key;
int pressed_button;
bool lock_mouse;

point last_mouse_moved;
point last_mouse_click;
point special_key_pressed;
point regular_key_pressed;

bool display_fps;
int FPS;
int fps_count;
size_t num_threads;
size_t n_iterations;

float dt;
float lenx, leny, lenz;
size_t sidex, sidey, sidez;
float density;
float viscosity;
float h;
float cs;

physim::solver_type solver;

physim::math::vec3 bgd_color;

bool run;
bool record;

void init_variables() {
	special_key = 0;
	pressed_button = 0;
	last_mouse_moved = point(0,0);
	lock_mouse = false;

	display_fps = false;
	FPS = 60;
	fps_count = 0;

	solver = physim::solver_type::EulerSemi;

	bgd_color = physim::math::vec3(0.0f,0.0f,0.0f);

	window_width = 640;
	window_height = 480;

	num_threads = 1;
	n_iterations = 10;

	dt = 0.01f;
	sidex = sidey = sidez = 16;
	lenx = leny = lenz = 0.5f;
	h = 0.03f;
	viscosity = 0.001f;
	density = 1000.0f;
	cs = 1500.0f;

	run = false;
	record = false;

	sec = timing::now();
}

void simulation_info(const fluids::fluid *F) {
	float m = F->get_particles()[0].mass;

	cout << "Fluid characteristics:" << endl;
	cout << "    Number of particles: " << endl;
	cout << "        " << sidex << " x "
					   << sidey << " x "
					   << sidez << " = " << sidex*sidey*sidez << endl;
	cout << "    Initial shape:" << endl;
	cout << "       " << lenx << " x "
					  << leny << " x "
					  << lenz << " = " << lenx*leny*lenz << endl;
	cout << "    Volume: " << lenx*leny*lenz << endl;
	cout << "    Viscosity: " << viscosity << endl;
	cout << "    Density: " << density << endl;
	cout << "    Neighbourhood size: " << h << endl;
	cout << "    Mass per particle: " << m << endl;
	cout << "    Speed of sound: " << cs << endl;
	cout << "Simulation characteristics:" << endl;
	cout << "    time step: " << dt << endl;
	cout << "    dt/mass: " << dt*(1.0f/m) << endl;
	cout << "    iterations per frame: " << n_iterations << endl;
	cout << "    solver: ";
	if (solver == solver_type::EulerOrig) {
		cout << "Euler Original" << endl;
	}
	if (solver == solver_type::EulerSemi) {
		cout << "Euler Semi" << endl;
	}
	if (solver == solver_type::Verlet) {
		cout << "Verlet" << endl;
	}
	cout << "    Number of threads: " << num_threads << endl;
}

} // -- namespace glut_variables
