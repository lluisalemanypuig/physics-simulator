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

// C++ includes
#include <iostream>
#include <memory>
using namespace std;

// glm includes
#include <glm/vec3.hpp>

// render includes
#include <render/geometry/rplane.hpp>
#include <render/include_gl.hpp>

// physim includes
#include <physim/emitter/free_emitter.hpp>
#include <physim/geometry/plane.hpp>
#include <physim/math/vec3.hpp>
using namespace physim;
using namespace particles;
using namespace geometric;
using namespace emitters;

// custom includes
#include "glut_functions.hpp"
#include "conversion_helper.hpp"
using namespace glut_functions;

namespace study_cases {

void sim_004_make_simulation() {
	free_emitter *I = SR.get_simulator().get_free_emitter();
	I->set_pos_initialiser(
		[](base_particle& p) {
			float z = -4.0f + p.index*0.8f;
			p.cur_pos = math::vec3(-5.0f,1.0f,z);
		}
	);
	I->set_vel_initialiser(
		[](base_particle& p) {
			p.cur_vel = math::vec3(5.0f,0.0f,0.0f);
		}
	);
	I->set_friction_initialiser(
		[](free_particle& p) { p.friction = p.index/10.0f; }
	);
	I->set_bounce_initialiser(
		[](free_particle& p) { p.bouncing = 1.0f; }
	);

	glm::vec3 A( -5.0f, -0.25f,-5.0f);
	glm::vec3 B( -5.0f, -0.25f, 5.0f);
	glm::vec3 C( 20.0f, -0.25f, 5.0f);
	glm::vec3 D( 20.0f, -0.25f,-5.0f);

	rplane *floor = new rplane();
	floor->set_points(A, B, C, D);
	SR.add_geometry(floor);

	plane *pl = new plane(
		math::vec3(0.0f,1.0f,0.0f),
		math::vec3(0.0f,0.0f,0.0f)
	);
	SR.get_simulator().add_geometry(pl);
	SR.get_simulator().set_gravity_acceleration(math::vec3(0.0f,-9.81f,0.0f));
	SR.get_simulator().add_free_particles(11);
	SR.make_free_particle_indices();

	SR.get_box().enlarge_box(glm::vec3(0.0f, 5.0f, 0.0f));
	SR.set_window_dims(window_width, window_height);
	SR.init_cameras();

	n_iterations = 1;
	SR.get_simulator().set_time_step(time_step);

	glut_functions::init_shaders();
	SR.get_box().make_buffers();
}

void sim_004_help() {
	glut_functions::help();

	cout << "Simulation 004 description:" << endl;
	cout << endl;
	cout << "This simulation is used to study the effects of the friction" << endl;
	cout << "coefficient: some particles with different friction coefficient" << endl;
	cout << "value roll on a plane. One of them will stop as soon as it touches" << endl;
	cout << "the plane, and one will never stop moving forward. The others will" << endl;
	cout << "eventually stop, the time to do so being dependent on that value." << endl;
	cout << "The value of the bouncing coefficient is fixed to 1.0." << endl;
	cout << endl;
}

void sim_004_reset() {
	clear_simulation();
	clear_shaders();

	// copy cameras
	perspective old_p = SR.get_perspective_camera();
	orthogonal old_o = SR.get_orthogonal_camera();

	glm::vec3 VRP = SR.get_VRP();
	float theta = SR.get_theta();
	float psi = SR.get_psi();

	glm::vec3 viewer_pos = SR.get_viewer_pos();
	float yaw = SR.get_yaw();
	float pitch = SR.get_pitch();

	sim_004_make_simulation();

	SR.set_perspective(old_p);
	SR.set_orthogonal(old_o);
	SR.set_VRP(VRP);
	SR.set_theta(theta);
	SR.set_psi(psi);
	SR.set_viewer_pos(viewer_pos);
	SR.set_yaw(yaw);
	SR.set_pitch(pitch);
}

void sim_004_regular_keys_keyboard(unsigned char c, int x, int y) {
	regular_keys_keyboard(c, x, y);

	switch (c) {
	case 'h':
		sim_004_help();
		break;
	case 'r':
		sim_004_reset();
		break;
	}
}

void sim_004_initGL(int argc, char *argv[]) {
	// ----------------- //
	/* initialise window */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(50, 25);
	glutInitWindowSize(window_width, window_height);
	window_id = glutCreateWindow("Particles - Simulation 004");

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
	glut_functions::parse_common_params(argc, argv);

	// ---------------- //
	/* build simulation */
	sim_004_make_simulation();
}

void sim_004(int argc, char *argv[]) {
	sim_004_initGL(argc, argv);
	sim_004_help();

	glutDisplayFunc(glut_functions::refresh);
	glutReshapeFunc(glut_functions::resize);
	glutMouseFunc(glut_functions::mouse_click_event);
	glutPassiveMotionFunc(glut_functions::mouse_movement);
	glutMotionFunc(glut_functions::mouse_drag_event);
	glutSpecialFunc(glut_functions::special_keys_keyboard);
	glutKeyboardFunc(sim_004_regular_keys_keyboard);

	//glutIdleFunc(refresh);
	glutTimerFunc(1000.0f/FPS, glut_functions::timed_refresh, 0);

	glutMainLoop();
}

} // -- namespace study_cases
