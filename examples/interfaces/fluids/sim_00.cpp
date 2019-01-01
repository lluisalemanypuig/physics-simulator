// C++ includes
#include <iostream>
#include <memory>
using namespace std;

// glm includes
#include <glm/vec3.hpp>

// render includes
#include <render/geometry/rplane.hpp>
#include <render/triangle_mesh/rendered_triangle_mesh.hpp>
#include <render/shader/shader_helper.hpp>
#include <render/include_gl.hpp>
#include <render/obj_reader.hpp>

// physim includes
#include <physim/fluids/fluid.hpp>
#include <physim/math/vec3.hpp>
using namespace physim;
using namespace particles;
using namespace geometric;
using namespace math;
using namespace fluids;

// custom includes
#include "glut_variables.hpp"
#include "glut_functions.hpp"
#include "conversion_helper.hpp"
using namespace glut_functions;
using namespace glut_variables;

namespace study_cases {

	void sim_00_make_simulation() {
		simulator& S = SR.get_simulator();

		SR.set_particle_size(2.0f);

		S.set_solver(solver_type::EulerSemi);
		S.set_gravity_acceleration(vec3(0.0f,-9.81f,0.0f));
		S.set_time_step(0.01f);
		n_iterations = 1;

		size_t side__ = 16;
		size_t N = side__*side__*side__;

		fluid *F = new fluid();
		F->allocate(N, volume, density, viscosity, h, cs);

		float H = h;

		kernel_scalar_function W =
		[H](float r2) -> float
		{
			float k = 1.0f - r2/(H*H);
			float PI = static_cast<float>(M_PI);
			return (315.0f/(64.0f*PI*std::pow(H, 3.0f)))*k*k*k;
		};
		kernel_vectorial_function gW =
		[H](const vec3& r, float r2, vec3& res) -> void
		{
			static const float PI = static_cast<float>(M_PI);

			float k = 1.0f - r2/(H*H);
			float s = (-945.0f/(32.0f*PI*std::pow(H, 3.0f)))*k*k;
			res = r*s;
		};
		kernel_scalar_function g2W =
		[H](float r2) -> float
		{
			float k = 1.0f - r2/(H*H);
			float PI = static_cast<float>(M_PI);
			return (945.0f/(8.0f*PI*std::pow(H, 7.0f)))*k*(r2/(H*H) - 0.75f*k);
		};
		F->set_kernel(W, gW, g2W);

		fluid_particle *Fs = F->get_particles();
		for (size_t i = 0; i < side__; ++i) {
			for (size_t j = 0; j < side__; ++j) {
				for (size_t k = 0; k < side__; ++k) {
					vec3 pos(i*(0.5f/side__), j*(0.5f/side__), k*(0.5f/side__));
					size_t idx = i*side__*side__ + j*side__ + k;
					Fs[idx].cur_pos = pos;
					Fs[idx].cur_vel = vec3(0.0f);
				}
			}
		}

		S.add_fluid(F);
		SR.make_fluid_particle_indices();

		glut_functions::init_shaders();

		SR.get_box().set_min_max(glm::vec3(-1,-1,-1), glm::vec3(1,1,1));
		SR.set_window_dims(window_width, window_height);
		SR.init_cameras();

		plane *base = new plane(vec3(0,1,0), vec3(0,-0.25,0));
		plane *w1 = new plane(vec3(1,0,0), vec3(-0.25,0,0));
		plane *w2 = new plane(vec3(-1,0,0), vec3(0.75,0,0));
		plane *w3 = new plane(vec3(0,0,1), vec3(0,0,-0.25));
		plane *w4 = new plane(vec3(0,0,-1), vec3(0,0,0.75));
		S.add_geometry(base);
		S.add_geometry(w1);
		S.add_geometry(w2);
		S.add_geometry(w3);
		S.add_geometry(w4);

		cout << "Fluid characteristics:" << endl;
		cout << "    Number of particles: " << N << endl;
		cout << "    Volume: " << volume << endl;
		cout << "    Viscosity: " << viscosity << endl;
		cout << "    Density: " << density << endl;
		cout << "    Neighbourhood size: " << h << endl;
		cout << "    Initial shape:" << endl;
		cout << "        0.2 x 0.2 x 0.1" << endl;
	}

	void sim_00_help() {
		glut_functions::help();

		cout << "Simulation 00 description:" << endl;
		cout << endl;
		cout << "    This is a simulation of a very small water-like fluid." << endl;
		cout << endl;
	}

	void sim_00_reset() {
		SR.clear();
		glut_functions::clear_shaders();

		// copy cameras
		perspective old_p = SR.get_perspective_camera();
		orthogonal old_o = SR.get_orthogonal_camera();

		glm::vec3 VRP = SR.get_VRP();
		float theta = SR.get_theta();
		float psi = SR.get_psi();

		glm::vec3 viewer_pos = SR.get_viewer_pos();
		float yaw = SR.get_yaw();
		float pitch = SR.get_pitch();

		sim_00_make_simulation();

		SR.set_perspective(old_p);
		SR.set_orthogonal(old_o);
		SR.set_VRP(VRP);
		SR.set_theta(theta);
		SR.set_psi(psi);
		SR.set_viewer_pos(viewer_pos);
		SR.set_yaw(yaw);
		SR.set_pitch(pitch);
	}

	void sim_00_regular_keys_keyboard(unsigned char c, int x, int y) {
		regular_keys_keyboard(c, x, y);

		switch (c) {
		case 'h':
			sim_00_help();
			break;
		case 'r':
			sim_00_reset();
			break;
		}
	}

	void sim_00_initGL(int argc, char *argv[]) {
		window_width = 640;
		window_height = 480;

		// ----------------- //
		/* initialise window */
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
		glutInitWindowPosition(50, 25);
		glutInitWindowSize(window_width, window_height);
		window_id = glutCreateWindow("Spring meshes - Simulation 00");

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_NORMALIZE);
		glEnable(GL_LIGHTING);

		glEnable(GL_LIGHT0);
		float col[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glLightfv(GL_LIGHT0, GL_DIFFUSE, col);
		float pos[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightfv(GL_LIGHT0, GL_POSITION, pos);
		float amb[] = {0.2f, 0.2f, 0.2f, 1.0f};
		glLightfv(GL_LIGHT0, GL_AMBIENT, amb);

		GLenum err = glewInit();
		if (err != 0) {
			cerr << "initGL - Error:" << endl;
			cerr << "    when initialising glew: " << err << endl;
			exit(1);
		}

		// --------------------------- //
		/* initialise global variables */
		glut_functions::init_glut_variables();

		h = 0.03f;
		volume = 0.01f;
		viscosity = 0.001f;
		density = 1000.0f;
		cs = 1500.0f;

		glut_functions::parse_common_params(argc, argv);

		// ---------------- //
		/* build simulation */
		sim_00_make_simulation();
	}

	void sim_00(int argc, char *argv[]) {
		sim_00_initGL(argc, argv);
		sim_00_help();

		glutDisplayFunc(glut_functions::refresh);
		glutReshapeFunc(glut_functions::resize);
		glutMouseFunc(glut_functions::mouse_click_event);
		glutPassiveMotionFunc(glut_functions::mouse_movement);
		glutMotionFunc(glut_functions::mouse_drag_event);
		glutSpecialFunc(glut_functions::special_keys_keyboard);
		glutKeyboardFunc(sim_00_regular_keys_keyboard);

		//glutIdleFunc(refresh);
		glutTimerFunc(1000.0f/FPS, glut_functions::timed_refresh, 0);

		glutMainLoop();
	}

} // -- namespace study_cases
