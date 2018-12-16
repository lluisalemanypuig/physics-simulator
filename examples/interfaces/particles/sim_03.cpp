// C++ includes
#include <iostream>
#include <memory>
using namespace std;

// glm includes
#include <glm/glm.hpp>

// render includes
#include <render/triangle_mesh/rendered_triangle_mesh.hpp>
#include <render/shader/shader_helper.hpp>
#include <render/geometry/rtriangle.hpp>
#include <render/geometry/rsphere.hpp>
#include <render/geometry/rplane.hpp>
#include <render/include_gl.hpp>
#include <render/obj_reader.hpp>

// physim includes
#include <physim/initialiser/multisource.hpp>
#include <physim/initialiser/hose.hpp>
#include <physim/geometry/rectangle.hpp>
#include <physim/geometry/triangle.hpp>
#include <physim/geometry/sphere.hpp>
#include <physim/geometry/plane.hpp>
#include <physim/math/math.hpp>
using namespace physim;
using namespace particles;
using namespace geometry;
using namespace init;

// custom includes
#include "glut_functions.hpp"
#include "conversion_helper.hpp"
using namespace glut_functions;

namespace study_cases {

	void sim_03_make_simulation() {

		glm::vec3 A(-3.0f, 0.0f,-3.0f);
		glm::vec3 D(-5.0f,-0.5f,-5.0f);
		glm::vec3 E(-5.0f,-0.5f, 5.0f);
		glm::vec3 F( 5.0f,-0.5f,-5.0f);
		glm::vec3 G( 5.0f,-0.5f, 5.0f);
		glm::vec3 H(-5.0f, 5.0f,-5.0f);
		glm::vec3 I(-5.0f, 5.0f, 5.0f);
		glm::vec3 J( 5.0f, 5.0f,-5.0f);
		glm::vec3 K( 5.0f, 5.0f, 5.0f);
		glm::vec3 L( 3.0f, 5.0f, 3.0f);
		glm::vec3 M( 0.0f, 2.0f, 0.0f);
		//glm::vec3 N( 0.0f, 3.0f, 0.0f);
		glm::vec3 O( 0.5f, 3.0f,-1.0f);
		glm::vec3 P(-1.0f, 3.0f, 0.5f);
		glm::vec3 S(-1.5f, 5.0f,-1.5f);

		glm::vec3 scene_top_center = (H + I + J + K)/4.0f;

		glm::vec3 dir1 = glm::normalize(scene_top_center - H);
		glm::vec3 dir2 = glm::normalize(scene_top_center - I);
		glm::vec3 dir3 = glm::normalize(scene_top_center - J);
		glm::vec3 dir4 = glm::normalize(scene_top_center - K);

		glm::vec3 source1 = H + dir1*0.25f;
		glm::vec3 source2 = I + dir2*0.25f;
		glm::vec3 source3 = J + dir3*0.25f;
		glm::vec3 source4 = K + dir4*0.25f;

		multisource<hose> ms;
		ms.allocate(16000, 4);

		std::vector<hose>& hoses = ms.get_sources();
		hoses[0].set_hose_source(to_physim(source1), to_physim(dir1), 1.0f, 16.0f);
		hoses[1].set_hose_source(to_physim(source2), to_physim(dir2), 8.0f, 12.0f);
		hoses[2].set_hose_source(to_physim(source3), to_physim(dir3), 12.0f, 8.0f);
		hoses[3].set_hose_source(to_physim(source4), to_physim(dir4), 16.0f, 1.0f);

		ms.make_position_init();
		ms.use_position_init();
		ms.make_velocity_init();
		ms.use_velocity_init();
		ms.set_starttime_initialiser(
			[](particles::free_particle *p) {
				p->starttime = p->index/4000.0f;
			}
		);
		ms.set_lifetime_initialiser(
			[&](free_particle *p) { p->lifetime = lifetime; }
		);
		ms.set_bounce_initialiser(
			[&](free_particle *p) { p->bouncing = bouncing; }
		);
		ms.set_friction_initialiser(
			[&](free_particle *p) { p->lifetime = friction; }
		);
		SR.get_simulator().set_initialiser(&ms);

		rplane *floor = new rplane();
		floor->set_points(E, G, F, D);

		rplane *wall1 = new rplane();
		wall1->set_points(K, I, E, G);
		wall1->set_color(1.0f,1.0f,0.0f,1.0f);
		rplane *wall2 = new rplane();
		wall2->set_points(K, J, F, G);
		wall2->set_color(1.0f,0.0f,1.0f,1.0f);
		rplane *wall3 = new rplane();
		wall3->set_points(J, H, D, F);
		wall3->set_color(0.0f,1.0f,1.0f,1.0f);
		rplane *wall4 = new rplane();
		wall4->set_points(H, I, E, D);
		wall4->set_color(0.0f,0.0f,1.0f,1.0f);

		shared_ptr<rendered_triangle_mesh> sim_ball(new rendered_triangle_mesh);
		OBJ_reader obj;
		obj.load_object("../../interfaces/models", "sphere.obj", *sim_ball);

		rsphere *ball1 = new rsphere();
		ball1->set_center(M);
		ball1->set_radius(1.0f);
		ball1->set_model(sim_ball);

		rsphere *ball2 = new rsphere();
		ball2->set_center(A);
		ball2->set_radius(1.0f);
		ball2->set_model(sim_ball);

		rtriangle *ramp = new rtriangle();
		ramp->set_points(S, O, P);
		ramp->set_color(0.0f,1.0f,0.0f,1.0f);

		SR.add_geometry(floor);
		SR.add_geometry(wall1);
		SR.add_geometry(wall2);
		SR.add_geometry(wall3);
		SR.add_geometry(wall4);
		SR.add_geometry(ball1);
		SR.add_geometry(ball2);
		SR.add_geometry(ramp);

		plane *floor_pl = new plane(math::vec3( 0.0f,1.0f, 0.0f), to_physim(floor->p1()));
		plane *wall1_pl = new plane(math::vec3( 0.0f,0.0f,-1.0f), to_physim(wall1->p1()));
		plane *wall2_pl = new plane(math::vec3(-1.0f,0.0f, 0.0f), to_physim(wall2->p1()));
		plane *wall3_pl = new plane(math::vec3( 0.0f,0.0f, 1.0f), to_physim(wall3->p1()));
		plane *wall4_pl = new plane(math::vec3( 1.0f,0.0f, 0.0f), to_physim(wall4->p1()));
		sphere *ball1_pl = new sphere(to_physim(ball1->center()), ball1->radius());
		sphere *ball2_pl = new sphere(to_physim(ball2->center()), ball2->radius());
		triangle *tri_ramp = new triangle(to_physim(S),to_physim(O),to_physim(P));

		SR.get_simulator().add_geometry(floor_pl);
		SR.get_simulator().add_geometry(wall1_pl);
		SR.get_simulator().add_geometry(wall2_pl);
		SR.get_simulator().add_geometry(wall3_pl);
		SR.get_simulator().add_geometry(wall4_pl);
		SR.get_simulator().add_geometry(ball1_pl);
		SR.get_simulator().add_geometry(ball2_pl);
		SR.get_simulator().add_geometry(tri_ramp);

		SR.get_simulator().set_gravity_acceleration(math::vec3(0.0f,-9.81f,0.0f));

		SR.get_simulator().add_free_particles(16000);

		SR.get_box().enlarge_box(glm::vec3(0.0f, 7.0f, 0.0f));
		SR.set_window_dims(iw, ih);
		SR.init_cameras();

		sim_ball->load_textures();
		if (use_shaders) {
			glut_functions::init_shaders();
			SR.get_box().make_buffers();
			sim_ball->make_buffers_materials_textures();
			shader& ts = texture_shader;
			ts.bind();
			shader_helper::activate_materials_textures(*sim_ball, ts);
			ts.release();
		}
		else {
			sim_ball->compile();
		}

		n_iterations = 1;
		SR.get_simulator().set_time_step(time_step);
	}

	void sim_03_help() {
		glut_functions::help();

		cout << "Simulation 03 description:" << endl;
		cout << endl;
		cout << "This is a simulation of 16000 particles randonmly shot" << endl;
		cout << "into a quadrilateral in which there are 2 spheres and a triangle." << endl;
		cout << "This simulation is meant to be used to test the efficiency of" << endl;
		cout << "the simulator." << endl;
		cout << endl;
	}

	void sim_03_reset() {
		clear_simulation();
		if (use_shaders) {
			clear_shaders();
		}

		// copy cameras
		perspective old_p = SR.get_perspective_camera();
		orthogonal old_o = SR.get_orthogonal_camera();

		glm::vec3 VRP = SR.get_VRP();
		float theta = SR.get_theta();
		float psi = SR.get_psi();

		glm::vec3 viewer_pos = SR.get_viewer_pos();
		float yaw = SR.get_yaw();
		float pitch = SR.get_pitch();

		sim_03_make_simulation();

		SR.set_perspective(old_p);
		SR.set_orthogonal(old_o);
		SR.set_VRP(VRP);
		SR.set_theta(theta);
		SR.set_psi(psi);
		SR.set_viewer_pos(viewer_pos);
		SR.set_yaw(yaw);
		SR.set_pitch(pitch);
	}

	void sim_03_regular_keys_keyboard(unsigned char c, int x, int y) {
		regular_keys_keyboard(c, x, y);

		switch (c) {
		case 'h':
			sim_03_help();
			break;
		case 'r':
			sim_03_reset();
			break;
		}
	}

	void sim_03_initGL(int argc, char *argv[]) {
		// ----------------- //
		/* initialise window */
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
		glutInitWindowPosition(50, 25);
		glutInitWindowSize(iw, ih);
		window_id = glutCreateWindow("Particles - Simulation 03");

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
		sim_03_make_simulation();
	}

	void sim_03(int argc, char *argv[]) {
		sim_03_initGL(argc, argv);
		sim_03_help();

		glutDisplayFunc(glut_functions::refresh);
		glutReshapeFunc(glut_functions::resize);
		glutMouseFunc(glut_functions::mouse_click_event);
		glutPassiveMotionFunc(glut_functions::mouse_movement);
		glutMotionFunc(glut_functions::mouse_drag_event);
		glutSpecialFunc(glut_functions::special_keys_keyboard);
		glutKeyboardFunc(sim_03_regular_keys_keyboard);

		//glutIdleFunc(refresh);
		glutTimerFunc(1000.0f/FPS, glut_functions::timed_refresh, 0);

		glutMainLoop();
	}

} // -- namespace study_cases
