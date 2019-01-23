// C++ includes
#include <iostream>
#include <memory>
using namespace std;

// glm includes
#include <glm/vec3.hpp>

// render includes
#include <render/geometry/rplane.hpp>
#include <render/triangle_mesh/rendered_triangle_mesh.hpp>
#include <render/include_gl.hpp>
#include <render/obj_reader.hpp>

// physim includes
#include <physim/geometry/plane.hpp>
#include <physim/math/vec3.hpp>
using namespace physim;
using namespace particles;
using namespace geometric;

// custom includes
#include "glut_functions.hpp"
#include "conversion_helper.hpp"
using namespace glut_functions;

namespace study_cases {

typedef math::vec3 pm_vec3;

void sim_105_initialise_sim() {
	plane *wall1 = new plane( pm_vec3( 1.0f, 0.0f,  0.0f), pm_vec3( 0.0f, 0.0f,  0.0f) );
	plane *wall2 = new plane( pm_vec3(-1.0f, 0.0f,  0.0f), pm_vec3(20.0f, 0.0f,  0.0f) );
	plane *wall3 = new plane( pm_vec3( 0.0f, 0.0f,  1.0f), pm_vec3( 0.0f, 0.0f,  0.0f) );
	plane *wall4 = new plane( pm_vec3( 0.0f, 0.0f, -1.0f), pm_vec3( 0.0f, 0.0f, 20.0f) );
	SR.get_simulator().add_geometry(wall1);
	SR.get_simulator().add_geometry(wall2);
	SR.get_simulator().add_geometry(wall3);
	SR.get_simulator().add_geometry(wall4);

	sized_particle p1;
	sized_particle p2;
	sized_particle p3;

	p1.lifetime = 99999999.0f;
	p1.bouncing = 1.0f;
	p1.friction = 0.0f;

	p2.lifetime = 99999999.0f;
	p2.bouncing = 1.0f;
	p2.friction = 0.0f;

	p3.lifetime = 99999999.0f;
	p3.bouncing = 1.0f;
	p3.friction = 0.0f;

	p1.cur_pos = math::vec3(2.0f, 0.0f, 2.0f);
	p1.cur_vel = math::vec3(1.0f, 0.0f, 1.0f);
	p1.R = 0.75f;
	p1.mass = 0.75f;

	p2.cur_pos = math::vec3(18.0f, 0.0f, 18.0f);
	p2.cur_vel = math::vec3(-1.0f, 0.0f, -1.0f);
	p2.R = 1.0f;
	p2.mass = 1.0f;

	p3.cur_pos = math::vec3(16.0f, 0.0f, 4.0f);
	p3.cur_vel = math::vec3(1.0f, 0.0f, 1.0f);
	p3.R = 2.0f;
	p3.mass = 2.0f;

	SR.get_simulator().set_particle_particle_collisions(true);
	SR.get_simulator().set_viscous_drag(0.0f);

	SR.get_simulator().add_sized_particle(p1);
	SR.get_simulator().add_sized_particle(p2);
	SR.get_simulator().add_sized_particle(p3);
}

void sim_105_initialise_sim_rend() {
	glm::vec3 A(  0.0f,  0.0f,  0.0f);
	glm::vec3 B(  0.0f,  5.0f,  0.0f);
	glm::vec3 C(  0.0f,  0.0f, 20.0f);
	glm::vec3 D(  0.0f,  5.0f, 20.0f);
	glm::vec3 E( 20.0f,  0.0f, 20.0f);
	glm::vec3 F( 20.0f,  5.0f, 20.0f);
	glm::vec3 G( 20.0f,  0.0f,  0.0f);
	glm::vec3 H( 20.0f,  5.0f,  0.0f);

	rplane *wall1 = new rplane();
	wall1->set_points(A,B,D,C);
	wall1->set_color(0.8f,0.0f,0.2f,1.0f);
	rplane *wall2 = new rplane();
	wall2->set_points(C,D,F,E);
	wall2->set_color(0.8f,0.0f,0.2f,1.0f);
	rplane *wall3 = new rplane();
	wall3->set_points(E,F,H,G);
	wall3->set_color(0.8f,0.0f,0.2f,1.0f);
	rplane *wall4 = new rplane();
	wall4->set_points(G,H,B,A);
	wall4->set_color(0.8f,0.0f,0.2f,1.0f);

	SR.add_geometry(wall1);
	SR.add_geometry(wall2);
	SR.add_geometry(wall3);
	SR.add_geometry(wall4);
}

void sim_105_make_simulation() {
	n_iterations = 10;
	time_step = 0.01f;
	SR.get_simulator().set_time_step(time_step);

	draw_sized_particles_wire = true;
	bgd_color = glm::vec3(0.8f,0.8f,0.8f);

	sim_105_initialise_sim();
	sim_105_initialise_sim_rend();

	SR.get_box().enlarge_box(glm::vec3(0.0f, 12.0f, 0.0f));
	SR.set_window_dims(window_width, window_height);
	SR.init_cameras();

	wireframe_sphere = new rendered_triangle_mesh();
	OBJ_reader obj;
	obj.load_object
	("../../interfaces/models", "sphere_fsmooth.obj", *wireframe_sphere);

	glut_functions::init_shaders();
	SR.get_box().make_buffers();
	wireframe_sphere->make_buffers();
}

void sim_105_help() {
	glut_functions::help();

	cout << "Simulation 105 description:" << endl;
	cout << endl;
	cout << "Three sized particles of different masses (0.75Kg, 1.0Kg and 2.0Kg)" << endl;
	cout << "bounce within a quadrilateral. There is no gravity. Each particle has a" << endl;
	cout << "radius proportional to its mass." << endl;
	cout << endl;
}

void sim_105_reset() {
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

	sim_105_make_simulation();

	SR.set_perspective(old_p);
	SR.set_orthogonal(old_o);
	SR.set_VRP(VRP);
	SR.set_theta(theta);
	SR.set_psi(psi);
	SR.set_viewer_pos(viewer_pos);
	SR.set_yaw(yaw);
	SR.set_pitch(pitch);
}

void sim_105_regular_keys_keyboard(unsigned char c, int x, int y) {
	regular_keys_keyboard(c, x, y);

	switch (c) {
	case 'h':
		sim_105_help();
		break;
	case 'r':
		sim_105_reset();
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

int sim_105_initGL(int argc, char *argv[]) {
	// ----------------- //
	/* initialise window */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(50, 25);
	glutInitWindowSize(window_width, window_height);
	window_id = glutCreateWindow("Particles - Simulation 105");

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
	sim_105_make_simulation();
	return 0;
}

void sim_105(int argc, char *argv[]) {
	sim_105_initGL(argc, argv);
	sim_105_help();

	glutDisplayFunc(glut_functions::refresh);
	glutReshapeFunc(glut_functions::resize);
	glutMouseFunc(glut_functions::mouse_click_event);
	glutPassiveMotionFunc(glut_functions::mouse_movement);
	glutMotionFunc(glut_functions::mouse_drag_event);
	glutSpecialFunc(glut_functions::special_keys_keyboard);
	glutKeyboardFunc(sim_105_regular_keys_keyboard);

	//glutIdleFunc(refresh);
	glutTimerFunc(1000.0f/FPS, glut_functions::timed_refresh, 0);

	glutMainLoop();
}

} // -- namespace study_cases
