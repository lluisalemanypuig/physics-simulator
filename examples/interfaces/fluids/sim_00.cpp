// C++ includes
#include <iostream>
#include <memory>
using namespace std;

// glm includes
#include <glm/vec3.hpp>
typedef glm::vec3 gvec3;

// render includes
#include <render/geometry/rplane.hpp>
#include <render/triangle_mesh/rendered_triangle_mesh.hpp>
#include <render/shader/shader_helper.hpp>
#include <render/include_gl.hpp>
#include <render/obj_reader.hpp>

// physim includes
#include <physim/fluids/newtonian.hpp>
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
#include "kernels.hpp"
using namespace glut_functions;
using namespace glut_variables;

namespace study_cases {

void sim_00_make_simulation() {
	simulator& S = SR.get_simulator();

	SR.set_particle_size(2.0f);

	S.set_solver(solver);
	S.set_gravity_acceleration(vec3(0.0f,-9.81f,0.0f));
	S.set_time_step(dt);

	size_t N = sidex*sidey*sidez;
	float volume = lenx*leny*lenz;

	fluid *F = new newtonian();
	F->allocate(N, volume, density, viscosity, h, cs);

	make_kernels();
	F->set_kernel_density(density_kernel);
	F->set_kernel_pressure(pressure_kernel);
	F->set_kernel_viscosity(viscosity_kernel);

	float boxx = 0.5f;
	float boxy = 0.5f;
	float boxz = 0.5f;

	float cx = boxx/2.0f;
	float cy = boxy/2.0f;
	float cz = boxz/2.0f;

	fluid_particle *Fs = F->get_particles();
	for (size_t i = 0; i < sidex; ++i) {
		for (size_t j = 0; j < sidey; ++j) {
			for (size_t k = 0; k < sidez; ++k) {
				int r1 = rand();
				float fr1 = static_cast<float>(r1);
				int r2 = rand();
				float fr2 = static_cast<float>(r2);
				int r3 = rand();
				float fr3 = static_cast<float>(r3);

				float sx = (r1%2 == 0 ? 1.0f : -1.0f);
				float sy = (r2%2 == 0 ? 1.0f : -1.0f);
				float sz = (r3%2 == 0 ? 1.0f : -1.0f);

				float dx = cx + sx*(fr1/RAND_MAX)*lenx/2.0f;
				float dy = cy + sy*(fr2/RAND_MAX)*leny/2.0f;
				float dz = cz + sz*(fr3/RAND_MAX)*lenz/2.0f;

				vec3 pos(dx, dy, dz);
				size_t idx = j*sidex*sidez + k*sidex + i;
				Fs[idx].prev_pos = pos;
				Fs[idx].cur_pos = pos;
				Fs[idx].cur_vel = vec3(0.1f, 1.0f, 0.1f);
			}
		}
	}

	S.add_fluid(F);
	S.set_particle_particle_collisions(true);
	SR.make_fluid_particle_indices();

	glut_functions::init_shaders();

	SR.get_box().set_min_max(glm::vec3(-1,-1,-1), glm::vec3(1,1,1));
	SR.set_window_dims(window_width, window_height);
	SR.init_cameras();

	float px = boxx + 0.05f;
	float mx = -0.05f;
	float my = -0.05f;
	float pz = boxz + 0.05f;
	float mz = -0.05f;

	plane *base = new plane(vec3(0,1,0), vec3(0,my,0));
	plane *w1 = new plane(vec3(1,0,0), vec3(mx,0,0));
	plane *w2 = new plane(vec3(-1,0,0), vec3(px,0,0));
	plane *w3 = new plane(vec3(0,0,1), vec3(0,0,mz));
	plane *w4 = new plane(vec3(0,0,-1), vec3(0,0,pz));
	S.add_geometry(base);
	S.add_geometry(w1);
	S.add_geometry(w2);
	S.add_geometry(w3);
	S.add_geometry(w4);

	float topy = boxy + 0.01f;

	gvec3 A00(my, my, mz);
	gvec3 A0p(my, my, pz);
	gvec3 Ap0(px, my, mz);
	gvec3 App(px, my, pz);
	gvec3 B00(my, topy, mz);
	gvec3 B0p(my, topy, pz);
	gvec3 Bp0(px, topy, mz);
	gvec3 Bpp(px, topy, pz);

	rplane *rbase = new rplane();
	rbase->set_points(A00, Ap0, App, A0p);
	rbase->set_color(1.0f, 0.0f, 0.0f, 0.5f);
	SR.add_geometry(rbase);

	/*rplane *rw1 = new rplane();
	rw1->set_points(A00, Ap0, Bp0, B00);
	rw1->set_color(1.0f, 0.0f, 0.0f, 0.5f);
	rplane *rw2 = new rplane();
	rw2->set_points(Ap0, Bp0, Bpp, App);
	rw2->set_color(1.0f, 0.0f, 0.0f, 0.5f);
	rplane *rw3 = new rplane();
	rw3->set_points(App, Bpp, B0p, A0p);
	rw3->set_color(1.0f, 0.0f, 0.0f, 0.5f);
	rplane *rw4 = new rplane();
	rw4->set_points(A0p, B0p, B00, A00);
	rw4->set_color(1.0f, 0.0f, 0.0f, 0.5f);
	SR.add_geometry(rw1);
	SR.add_geometry(rw2);
	SR.add_geometry(rw3);
	SR.add_geometry(rw4);*/

	bgd_color.x = bgd_color.y = bgd_color.z = 0.8f;
	simulation_info(F);
}

void sim_00_help() {
	glut_functions::help();

	cout << "Simulation 00 description:" << endl;
	cout << endl;
	cout << "    This is a simulation of a very small fluid." << endl;
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
	window_id = glutCreateWindow("Fluids - Simulation 00");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
	glut_variables::init_variables();
	glut_functions::parse_common_params(argc, argv);

	// ---------------- //
	/* build simulation */
	sim_00_make_simulation();
}

void sim_00(int argc, char *argv[]) {
	sim_00_help();

	sim_00_initGL(argc, argv);

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
