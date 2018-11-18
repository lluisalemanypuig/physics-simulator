#include "study_cases.hpp"

// C++ includes
#include <iostream>
#include <memory>
using namespace std;

// base includes
#include <base/geometry/rrectangle.hpp>
#include <base/geometry/rsphere.hpp>
#include <base/model/rendered_model.hpp>
#include <base/include_gl.hpp>
#include <base/obj_reader.hpp>

// physim includes
#include <physim/meshes/mesh1d.hpp>
#include <physim/geometry/plane.hpp>
#include <physim/geometry/rectangle.hpp>
#include <physim/geometry/sphere.hpp>
#include <physim/math/math.hpp>
using namespace physim;
using namespace particles;
using namespace meshes;
using namespace math;
using namespace geom;

// custom includes
#include "glut_functions.hpp"
using namespace glut_functions;

namespace study_cases {

	void sim1_make_simulation() {
		SR.set_particle_size(2.0f);
		SR.set_spring_width(1.5f);

		SR.get_simulator().set_solver(solver_type::EulerSemi);
		SR.get_simulator().add_gravity_acceleration(vec3(0.0f,-9.81f,0.0f));

		float sx;

		/* build meshes */

		// -- group 1
		// 10 'free' springs of increasing number of particles
		sx = -2.5f;
		for (int i = 1; i <= 10; ++i) {
			int j = 5*i;
			mesh1d *m = new mesh1d();
			m->allocate(j, 2.0f);
			m->simulate_stretch(glut_functions::stretch);
			m->simulate_bend(glut_functions::bend);
			m->set_elasticity(glut_functions::elasticity/i);
			m->set_damping(glut_functions::damping);
			m->set_friction(glut_functions::friction);
			m->set_bouncing(glut_functions::bouncing);

			mesh_particle **mp = m->get_particles();
			mp[0]->fixed = true;
			mp[0]->cur_pos = vec3(sx, 10.0f, i);
			for (int k = 1; k < j; ++k) {
				mp[k]->cur_pos = vec3(sx + (5.0f/j)*k, 10.0f, i);
			}

			SR.get_simulator().add_mesh(m);
		}

		// -- group 2
		// 10 'free' springs of increasing number of particles
		sx = -2.5f;
		for (int i = 1; i <= 10; ++i) {

			int j = 5*i;
			mesh1d *m = new mesh1d();
			m->allocate(j, 0.5f*j);
			m->simulate_stretch(glut_functions::stretch);
			m->simulate_bend(glut_functions::bend);
			m->set_elasticity(glut_functions::elasticity);
			m->set_damping(glut_functions::damping);
			m->set_friction(glut_functions::friction);
			m->set_bouncing(glut_functions::bouncing);

			mesh_particle **mp = m->get_particles();
			mp[0]->fixed = true;
			mp[0]->cur_pos = vec3(sx, 10.0f, i + 15.0f);
			for (int k = 1; k < j; ++k) {
				mp[k]->cur_pos = vec3(sx + (5.0f/j)*k, 10.0f, i + 15.0f);
			}

			SR.get_simulator().add_mesh(m);
		}

		// -- group 3
		// 10 springs on top of a rectangle
		rrectangle *rect = new rrectangle();
		rect->set_points(
			vec3( 5.5f, 7.0f, -0.5f), vec3( 5.5f, 7.0f, 10.0f),
			vec3(10.5f, 7.0f, 10.0f), vec3(10.5f, 7.0f, -0.5f)
		);
		SR.add_geometry(rect);
		SR.get_box().enlarge_box(vec3(sx,10.0f,0.0f));
		SR.get_box().enlarge_box(vec3(sx + 5.0f,-5.0f,10.0f));
		rectangle *r = new rectangle(rect->p1(), rect->p2(), rect->p3(), rect->p4());
		SR.get_simulator().add_geometry(r);

		sx = 5.0f;
		for (int i = 1; i <= 10; ++i) {
			int j = 5*i;
			mesh1d *m = new mesh1d();
			m->allocate(j, 2.0f);
			m->simulate_stretch(glut_functions::stretch);
			m->simulate_bend(glut_functions::bend);
			m->set_elasticity(glut_functions::elasticity/i);
			m->set_damping(glut_functions::damping);
			m->set_friction(glut_functions::friction);
			m->set_bouncing(glut_functions::bouncing);

			mesh_particle **mp = m->get_particles();
			mp[0]->fixed = true;
			mp[0]->cur_pos = vec3(sx, 10.0f, i);
			for (int k = 1; k < j; ++k) {
				mp[k]->cur_pos = vec3(sx + (5.0f/j)*k, 10.0f, i);
			}

			SR.get_simulator().add_mesh(m);
		}

		// -- group 4
		// 10 springs on top of spheres
		shared_ptr<rendered_model> model_ball(new rendered_model);

		OBJ_reader obj;
		obj.load_object("../../interfaces/models", "sphere.obj", *model_ball);
		model_ball->load_textures();
		model_ball->compile();

		sx = 12.5f;
		for (int i = 1; i <= 10; ++i) {
			rsphere *rs = new rsphere();
			rs->set_center(vec3(sx + 2.5f, 7.0f, 3*i));
			rs->set_radius(2.0f);
			rs->set_model(model_ball);
			SR.add_geometry(rs);

			sphere *s = new sphere(rs->center(), rs->radius());
			SR.get_simulator().add_geometry(s);
		}

		for (int i = 1; i <= 10; ++i) {
			int j = 5*i;
			mesh1d *m = new mesh1d();
			m->allocate(j, 2.0f);
			m->simulate_stretch(glut_functions::stretch);
			m->simulate_bend(glut_functions::bend);
			m->set_elasticity(glut_functions::elasticity/i);
			m->set_damping(glut_functions::damping);
			m->set_friction(glut_functions::friction);
			m->set_bouncing(glut_functions::bouncing);

			mesh_particle **mp = m->get_particles();
			mp[0]->fixed = true;
			mp[0]->cur_pos = vec3(sx, 10.0f, 3*i);
			for (int k = 1; k < j; ++k) {
				mp[k]->cur_pos = vec3(sx + (5.0f/j)*k, 10.0f, 3*i);
			}

			SR.get_simulator().add_mesh(m);
		}

		SR.set_window_dims(iw, ih);
		SR.init_cameras();
	}

	void sim1_help() {
		glut_functions::help();

		cout << "Simulation 1 description:" << endl;
		cout << endl;
		cout << "    This simulation features several springs colliding (or not)" << endl;
		cout << "    with geometrical objects (spheres and planes)." << endl;
		cout << endl;
		cout << "    There are 4 groups of 10 meshes each. Except for one group," << endl;
		cout << "    the mass (2 Kg) is diveded uniformly among the particles of the mesh." << endl;
		cout << "    All springs have the same length but different number of particles." << endl;
		cout << "    All the particles of all meshes in that one group have the same mass" << endl;
		cout << "    (0.5 Kg), and the same elasticity constant (Ke = 500)." << endl;
		cout << "    In the other groups, the i-th spring has 5*i particles, and its elasticity" << endl;
		cout << "    constant (Ke) is set to 500/i, for 1 <= i <= 10." << endl;
		cout << endl;
	}

	void sim1_reset() {
		SR.clear();
		// copy cameras
		perspective old_p = SR.get_perspective_camera();
		orthogonal old_o = SR.get_orthogonal_camera();

		vec3 VRP = SR.get_VRP();
		float theta = SR.get_theta();
		float psi = SR.get_psi();

		vec3 viewer_pos = SR.get_viewer_pos();
		float yaw = SR.get_yaw();
		float pitch = SR.get_pitch();

		sim1_make_simulation();

		SR.set_perspective(old_p);
		SR.set_orthogonal(old_o);
		SR.set_VRP(VRP);
		SR.set_theta(theta);
		SR.set_psi(psi);
		SR.set_viewer_pos(viewer_pos);
		SR.set_yaw(yaw);
		SR.set_pitch(pitch);
	}

	void sim1_regular_keys_keyboard(unsigned char c, int x, int y) {
		regular_keys_keyboard(c, x, y);

		switch (c) {
		case 'h':
			sim1_help();
			break;
		case 'r':
			sim1_reset();
			break;
		}
	}

	void sim1_initGL(int argc, char *argv[]) {
		// ----------------- //
		/* initialise window */
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
		glutInitWindowPosition(50, 25);
		glutInitWindowSize(iw, ih);
		window_id = glutCreateWindow("Spring meshes - Simulation 1");

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

		// --------------------------- //
		/* initialise global variables */
		glut_functions::init_glut_variables();

		glut_functions::stretch = true;
		glut_functions::shear = false;
		glut_functions::bend = false;

		// ---------------- //
		/* build simulation */
		sim1_make_simulation();
	}

	void sim1_1dmeshes(int argc, char *argv[]) {
		sim1_help();
		sim1_initGL(argc, argv);

		glutDisplayFunc(glut_functions::refresh);
		glutReshapeFunc(glut_functions::resize);
		glutMouseFunc(glut_functions::mouse_click_event);
		glutPassiveMotionFunc(glut_functions::mouse_movement);
		glutMotionFunc(glut_functions::mouse_drag_event);
		glutSpecialFunc(glut_functions::special_keys_keyboard);
		glutKeyboardFunc(sim1_regular_keys_keyboard);

		//glutIdleFunc(refresh);
		glutTimerFunc(1000.0f/FPS, glut_functions::timed_refresh, 0);

		glutMainLoop();
	}

} // -- namespace study_cases
