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
#include <render/geometry/robject.hpp>
#include <render/triangle_mesh/rendered_triangle_mesh.hpp>
#include <render/shader/shader_helper.hpp>
#include <render/include_gl.hpp>
#include <render/obj_reader.hpp>

// physim includes
#include <physim/geometry/object.hpp>
#include <physim/emitter/free_emitter.hpp>
#include <physim/emitter/free_emitters/rect_shower.hpp>
#include <physim/input/input.hpp>
#include <physim/math/vec3.hpp>
using namespace physim;
using namespace particles;
using namespace emitters::free_emitters;
using namespace geometric;
using namespace emitters;

// custom includes
#include "glut_functions.hpp"
#include "conversion_helper.hpp"
using namespace glut_functions;

namespace study_cases {

typedef math::vec3 pm_vec3;

void sim_007_initialise_sim() {
	object *O = new object();
	input::read_file("../../interfaces/models", "pipe-geometric.obj", O);
	SR.get_simulator().add_geometry(O);

	float lx = O->get_max().x - O->get_min().x;
	float lz = O->get_max().z - O->get_min().z;

	float sx = O->get_min().x;
	float sy = O->get_max().y;
	float sz = O->get_min().z;

	simulator& S = SR.get_simulator();

	// these parts of the emitter_free are scene-dependent

	rect_shower w;
	w.set_straight_source(math::vec3(sx,sy+0.5f,sz), lx,lz);
	w.set_mass_initialiser(
		[](base_particle& p) { p.mass = 1.0f; }
	);
	w.set_starttime_initialiser(
		[](free_particle& p) {
			p.starttime = p.index/5000.0f;
		}
	);
	w.set_charge_initialiser(
		[](free_particle& p) { p.charge = 1.0f; }
	);
	w.set_lifetime_initialiser(
		[&](free_particle& p) { p.lifetime = lifetime; }
	);
	w.set_bounce_initialiser(
		[&](free_particle& p) { p.bouncing = bouncing; }
	);
	w.set_friction_initialiser(
		[&](free_particle& p) { p.friction = friction; }
	);
	SR.get_simulator().set_free_emitter(&w);

	SR.get_simulator().add_free_particles(5000);
	SR.make_free_particle_indices();
	S.set_gravity_acceleration(pm_vec3(0.0f, -2.0f, 0.0f));
}

void sim_007_initialise_sim_rend() {
	shared_ptr<rendered_triangle_mesh> model_pipe(new rendered_triangle_mesh);
	OBJ_reader obj;
	obj.load_object("../../interfaces/models", "pipe-artistic.obj", *model_pipe);

	const geometry *G = SR.get_simulator().get_fixed_objects()[0];
	const object *O = static_cast<const object *>(G);

	vector<pair<pm_vec3,pm_vec3> > boxes;
	O->get_partition().get_boxes(boxes);

	robject *ro = new robject();
	ro->set_model(model_pipe);
	ro->set_boxes(boxes);

	SR.add_geometry(ro);

	model_pipe->load_textures();
	glut_functions::init_shaders();

	SR.get_box().make_buffers();
	ro->make_boxes_buffers();

	model_pipe->make_buffers_materials_textures();
	shader& ts = texture_shader;
	ts.bind();
	shader_helper::activate_materials_textures(*model_pipe, ts);
	ts.release();
	SR.get_box().make_buffers();
}

void sim_007_make_simulation() {
	n_iterations = 10;
	time_step = 0.01f;
	SR.get_simulator().set_time_step(time_step);

	draw_sized_particles_wire = true;
	bgd_color = glm::vec3(0.0f,0.0f,0.0f);

	sim_007_initialise_sim();
	sim_007_initialise_sim_rend();

	SR.set_window_dims(window_width, window_height);
	SR.init_cameras();

	n_iterations = 1;
}

void sim_007_help() {
	glut_functions::help();

	cout << "Simulation 007 description:" << endl;
	cout << endl;
	cout << "This simulation shows a particle bouncing on non-trivial geometry." << endl;
	cout << "A model is loaded from disk and then stored in an octree" << endl;
	cout << "A single size-less particle bounces on it to debug collisions." << endl;
	cout << "The user can visualise the octree's boxes by pressing letter 'w'" << endl;
	cout << endl;
}

void sim_007_reset() {
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

	sim_007_make_simulation();

	SR.set_perspective(old_p);
	SR.set_orthogonal(old_o);
	SR.set_VRP(VRP);
	SR.set_theta(theta);
	SR.set_psi(psi);
	SR.set_viewer_pos(viewer_pos);
	SR.set_yaw(yaw);
	SR.set_pitch(pitch);
}

void sim_007_regular_keys_keyboard(unsigned char c, int x, int y) {
	regular_keys_keyboard(c, x, y);

	switch (c) {
	case 'h':
		sim_007_help();
		break;
	case 'r':
		sim_007_reset();
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

int sim_007_initGL(int argc, char *argv[]) {
	// ----------------- //
	/* initialise window */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(50, 25);
	glutInitWindowSize(window_width, window_height);
	window_id = glutCreateWindow("Particles - Simulation 007");

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

	glut_variables::draw_boxes_octree = true;

	// ---------------- //
	/* build simulation */
	sim_007_make_simulation();
	return 0;
}

void sim_007(int argc, char *argv[]) {
	sim_007_initGL(argc, argv);
	sim_007_help();

	glutDisplayFunc(glut_functions::refresh);
	glutReshapeFunc(glut_functions::resize);
	glutMouseFunc(glut_functions::mouse_click_event);
	glutPassiveMotionFunc(glut_functions::mouse_movement);
	glutMotionFunc(glut_functions::mouse_drag_event);
	glutSpecialFunc(glut_functions::special_keys_keyboard);
	glutKeyboardFunc(sim_007_regular_keys_keyboard);

	//glutIdleFunc(refresh);
	glutTimerFunc(1000.0f/FPS, glut_functions::timed_refresh, 0);

	glutMainLoop();
}

} // -- namespace study_cases
