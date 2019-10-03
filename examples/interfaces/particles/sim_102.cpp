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

// C includes
#include <string.h>

// C++ includes
#include <iostream>
#include <memory>
using namespace std;

// glm includes
#include <glm/vec3.hpp>

// render includes
#include <render/geometry/rplane.hpp>
#include <render/geometry/rtriangle.hpp>
#include <render/triangle_mesh/rendered_triangle_mesh.hpp>
#include <render/include_gl.hpp>
#include <render/obj_reader.hpp>

// physim includes
#include <physim/geometry/plane.hpp>
#include <physim/geometry/triangle.hpp>
#include <physim/emitter/sized_emitter.hpp>
#include <physim/particles/sized_particle.hpp>
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

static float px_08, py_08, pz_08;

void sim_102_add_triangle
(const math::vec3& p1, const math::vec3& p2, const math::vec3& p3,
 const glm::vec4& col)
{
	SR.get_simulator().add_geometry(new triangle(p1,p2,p3));
	rtriangle *tri = new rtriangle();
	tri->set_points(to_glm(p1), to_glm(p2), to_glm(p3));
	tri->set_color(col[0], col[1], col[2], col[3]);
	SR.add_geometry(tri);
}

void sim_102_make_simulation() {
	draw_sized_particles_wire = true;
	bgd_color = glm::vec3(0.8f,0.8f,0.8f);

	sized_emitter I;
	I.set_pos_initialiser(
		[&](base_particle& p) {
			p.cur_pos = math::vec3(px_08,py_08,pz_08);
		}
	);
	I.set_lifetime_initialiser(
		[&](free_particle& p) { p.lifetime = lifetime; }
	);
	I.set_bounce_initialiser(
		[&](free_particle& p) { p.bouncing = bouncing; }
	);
	I.set_friction_initialiser(
		[&](free_particle& p) { p.friction = friction; }
	);
	I.set_radius_initialiser(
		[&](sized_particle& p) { p.R = 1.0f; }
	);
	SR.get_simulator().set_sized_emitter(&I);
	SR.get_simulator().add_sized_particle();

	sim_102_add_triangle
	(math::vec3(1.73f, 4.45f, -0.25f), math::vec3( 4.35f, 3.33f, -0.92f),
	 math::vec3(3.73f, 1.75f,  2.55f), glm::vec4(1.0f,0.8f,0.0f,1.0f));
	sim_102_add_triangle
	(math::vec3( 2.05f, 2.55f, 1.77f), math::vec3( 1.44f, 2.55f,  4.44f),
	 math::vec3(-0.56f, 2.55f, 2.44f), glm::vec4(0.8f,1.0f,0.0f,1.0f));
	sim_102_add_triangle
	(math::vec3(-2.55f, 2.71f, -0.81f), math::vec3(-1.66f, 2.71f, 1.86f),
	 math::vec3(-3.70f, 1.94f,  1.41f), glm::vec4(1.0f,1.0f,0.2f,1.0f));
	sim_102_add_triangle
	(math::vec3( 1.24f, 3.56f, -3.38f), math::vec3(-1.38f, 2.55f, -2.70f),
	 math::vec3( 0.62f, 0.46f, -0.70f), glm::vec4(0.3f,0.3f,1.0f,1.0f));

	plane *pl = new plane(
		math::vec3(0.0f,1.0f,0.0f),
		math::vec3(0.0f,0.0f,0.0f)
	);
	SR.get_simulator().add_geometry(pl);
	SR.get_simulator().set_gravity_acceleration(
		math::vec3(0.0f,-9.81f,0.0f)
	);

	rplane *floor = new rplane();
	floor->set_points(
		glm::vec3(-5.0f, -0.05f, -5.0f), glm::vec3(-5.0f, -0.05f,  5.0f),
		glm::vec3( 5.0f, -0.05f,  5.0f), glm::vec3( 5.0f, -0.05f, -5.0f)
	);
	SR.add_geometry(floor);

	SR.get_box().enlarge_box(glm::vec3(0.0f, 6.0f, 0.0f));
	SR.set_window_dims(window_width, window_height);
	SR.init_cameras();

	OBJ_reader obj;
	wireframe_sphere = new rendered_triangle_mesh();
	obj.load_object
	("../../interfaces/models", "sphere_fsmooth.obj", *wireframe_sphere);

	glut_functions::init_shaders();
	SR.get_box().make_buffers();
	wireframe_sphere->make_buffers();

	n_iterations = 1;
	SR.get_simulator().set_time_step(time_step);
}

void sim_102_help() {
	glut_functions::help();

	cout << "Simulation 102:" << endl;
	cout << endl;
	cout << "Sized particle bouncing on a triangle." << endl;
	cout << endl;
	cout << "Command line parameters:" << endl;
	cout << "    --x : set initial value of position's x-coordinate" << endl;
	cout << "    --y : set initial value of position's y-coordinate" << endl;
	cout << "    --z : set initial value of position's z-coordinate" << endl;
	cout << endl;
	cout << "Keys interaction:" << endl;
	cout << "    CTRL + w: render the sized particles in wireframe" << endl;
	cout << endl;
}

void sim_102_reset() {
	clear_simulation();
	clear_shaders();

	// copy_08 cameras
	perspective old_p = SR.get_perspective_camera();
	orthogonal old_o = SR.get_orthogonal_camera();

	glm::vec3 VRP = SR.get_VRP();
	float theta = SR.get_theta();
	float psi = SR.get_psi();

	glm::vec3 viewer_pos = SR.get_viewer_pos();
	float yaw = SR.get_yaw();
	float pitch = SR.get_pitch();

	sim_102_make_simulation();

	SR.set_perspective(old_p);
	SR.set_orthogonal(old_o);
	SR.set_VRP(VRP);
	SR.set_theta(theta);
	SR.set_psi(psi);
	SR.set_viewer_pos(viewer_pos);
	SR.set_yaw(yaw);
	SR.set_pitch(pitch);
}

void sim_102_regular_keys_keyboard(unsigned char c, int x, int y) {
	regular_keys_keyboard(c, x, y);

	switch (c) {
	case 'h':
		sim_102_help();
		break;
	case 'r':
		sim_102_reset();
		break;
	}

	if (GLUT_ACTIVE_CTRL) {
		c = c + 'a' - 1;
		// If 'p' is pressed then c = 1 + actual_char - 'a'
		// So: actual_char = c + 'a' - 1
		switch(c) {
		case 'w':
			draw_sized_particles_wire = not draw_sized_particles_wire;
			break;
		}
	}
}

int sim_102_initGL(int argc, char *argv[]) {
	// ----------------- //
	/* initialise window */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(50, 25);
	glutInitWindowSize(window_width, window_height);
	window_id = glutCreateWindow("Particles - Simulation 102");

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

	px_08 = 0.0f;
	py_08 = 3.5f;
	pz_08 = 0.0f;

	for (int i = 2; i < argc; ++i) {
		if (strcmp(argv[i], "--x") == 0) {
			px_08 = atof(argv[i + 1]);
			++i;
		}
		else if (strcmp(argv[i], "--y") == 0) {
			py_08 = atof(argv[i + 1]);
			++i;
		}
		else if (strcmp(argv[i], "--z") == 0) {
			pz_08 = atof(argv[i + 1]);
			++i;
		}
	}

	// ---------------- //
	/* build simulation */
	sim_102_make_simulation();
	return 0;
}

void sim_102(int argc, char *argv[]) {
	sim_102_initGL(argc, argv);
	sim_102_help();

	glutDisplayFunc(glut_functions::refresh);
	glutReshapeFunc(glut_functions::resize);
	glutMouseFunc(glut_functions::mouse_click_event);
	glutPassiveMotionFunc(glut_functions::mouse_movement);
	glutMotionFunc(glut_functions::mouse_drag_event);
	glutSpecialFunc(glut_functions::special_keys_keyboard);
	glutKeyboardFunc(sim_102_regular_keys_keyboard);

	//glutIdleFunc(refresh);
	glutTimerFunc(1000.0f/FPS, glut_functions::timed_refresh, 0);

	glutMainLoop();
}

} // -- namespace study_cases
