// C++ includes
#include <iostream>
#include <memory>
using namespace std;

// glm includes
#include <glm/vec3.hpp>

// render includes
#include <render/geometry/rrectangle.hpp>
#include <render/geometry/rsphere.hpp>
#include <render/triangle_mesh/rendered_triangle_mesh.hpp>
#include <render/shader/shader_helper.hpp>
#include <render/include_gl.hpp>
#include <render/obj_reader.hpp>

// physim includes
#include <physim/meshes/mesh1d.hpp>
#include <physim/geometry/rectangle.hpp>
#include <physim/geometry/sphere.hpp>
#include <physim/math/vec3.hpp>
using namespace physim;
using namespace particles;
using namespace meshes;
using namespace math;
using namespace geometric;

// custom includes
#include "glut_functions.hpp"
#include "glut_variables.hpp"
#include "conversion_helper.hpp"
using namespace glut_functions;
using namespace glut_variables;

namespace study_cases {

	void sim_00_make_simulation() {
		SR.set_particle_size(2.0f);
		SR.set_spring_width(1.5f);

		SR.get_simulator().set_solver(solver_type::EulerSemi);
		SR.get_simulator().set_gravity_acceleration(vec3(0.0f,-9.81f,0.0f));

		float sx;

		/* build meshes */

		// -- group 1
		// 10 'free' springs of increasing number of particles
		sx = -2.5f;
		for (int i = 1; i <= 10; ++i) {
			int j = 5*i;
			mesh1d *m = new mesh1d();
			m->allocate(j, 2.0f);
			m->simulate_stretch(stretch);
			m->simulate_bend(bend);
			m->set_elasticity(elasticity/i);
			m->set_damping(damping);
			m->set_friction(friction);
			m->set_bouncing(bouncing);

			mesh_particle *mp = m->get_particles();
			mp[0].fixed = true;
			mp[0].cur_pos = vec3(sx, 10.0f, i);
			for (int k = 1; k < j; ++k) {
				mp[k].cur_pos = vec3(sx + (5.0f/j)*k, 10.0f, i);
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
			m->simulate_stretch(stretch);
			m->simulate_bend(bend);
			m->set_elasticity(elasticity);
			m->set_damping(damping);
			m->set_friction(friction);
			m->set_bouncing(bouncing);

			mesh_particle *mp = m->get_particles();
			mp[0].fixed = true;
			mp[0].cur_pos = vec3(sx, 10.0f, i + 15.0f);
			for (int k = 1; k < j; ++k) {
				mp[k].cur_pos = vec3(sx + (5.0f/j)*k, 10.0f, i + 15.0f);
			}

			SR.get_simulator().add_mesh(m);
		}

		// -- group 3
		// 10 springs on top of a rectangle
		rrectangle *rect = new rrectangle();
		rect->set_points(
			glm::vec3( 5.5f, 7.0f, -0.5f), glm::vec3( 5.5f, 7.0f, 10.0f),
			glm::vec3(10.5f, 7.0f, 10.0f), glm::vec3(10.5f, 7.0f, -0.5f)
		);
		SR.add_geometry(rect);
		SR.get_box().enlarge_box(glm::vec3(sx,10.0f,0.0f));
		SR.get_box().enlarge_box(glm::vec3(sx + 5.0f,-5.0f,10.0f));
		rectangle *r = new rectangle(
			vec3( 5.5f, 7.0f, -0.5f), vec3( 5.5f, 7.0f, 10.0f),
			vec3(10.5f, 7.0f, 10.0f), vec3(10.5f, 7.0f, -0.5f)
		);
		SR.get_simulator().add_geometry(r);

		sx = 5.0f;
		for (int i = 1; i <= 10; ++i) {
			int j = 5*i;
			mesh1d *m = new mesh1d();
			m->allocate(j, 2.0f);
			m->simulate_stretch(stretch);
			m->simulate_bend(bend);
			m->set_elasticity(elasticity/i);
			m->set_damping(damping);
			m->set_friction(friction);
			m->set_bouncing(bouncing);

			mesh_particle *mp = m->get_particles();
			mp[0].fixed = true;
			mp[0].cur_pos = vec3(sx, 10.0f, i);
			for (int k = 1; k < j; ++k) {
				mp[k].cur_pos = vec3(sx + (5.0f/j)*k, 10.0f, i);
			}

			SR.get_simulator().add_mesh(m);
		}

		// -- group 4
		// 10 springs on top of spheres
		shared_ptr<rendered_triangle_mesh> sim_ball(new rendered_triangle_mesh);

		OBJ_reader obj;
		obj.load_object("../../interfaces/models", "sphere.obj", *sim_ball);

		sx = 12.5f;
		for (int i = 1; i <= 10; ++i) {
			rsphere *rs = new rsphere();
			rs->set_center(glm::vec3(sx + 2.5f, 7.0f, 3*i));
			rs->set_radius(2.0f);
			rs->set_model(sim_ball);
			SR.add_geometry(rs);

			sphere *s = new sphere(to_physim(rs->center()), rs->radius());
			SR.get_simulator().add_geometry(s);
		}

		for (int i = 1; i <= 10; ++i) {
			int j = 5*i;
			mesh1d *m = new mesh1d();
			m->allocate(j, 2.0f);
			m->simulate_stretch(stretch);
			m->simulate_bend(bend);
			m->set_elasticity(elasticity/i);
			m->set_damping(damping);
			m->set_friction(friction);
			m->set_bouncing(bouncing);

			mesh_particle *mp = m->get_particles();
			mp[0].fixed = true;
			mp[0].cur_pos = vec3(sx, 10.0f, 3*i);
			for (int k = 1; k < j; ++k) {
				mp[k].cur_pos = vec3(sx + (5.0f/j)*k, 10.0f, 3*i);
			}

			SR.get_simulator().add_mesh(m);
		}

		SR.set_window_dims(window_width, window_height);
		SR.init_cameras();

		init_shaders();
		sim_ball->load_textures();
		SR.get_box().make_buffers();
		sim_ball->make_buffers_materials_textures();
		texture_shader.bind();
		shader_helper::activate_materials_textures(*sim_ball, texture_shader);
		texture_shader.release();
	}

	void sim_00_help() {
		help();

		cout << "Simulation 00 description:" << endl;
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

	void sim_00_reset() {
		SR.clear();
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
		init_glut_variables();

		stretch = true;
		shear = false;
		bend = false;

		parse_common_params(argc, argv);

		// ---------------- //
		/* build simulation */
		sim_00_make_simulation();
	}

	void sim_00(int argc, char *argv[]) {
		sim_00_initGL(argc, argv);
		sim_00_help();

		glutDisplayFunc(refresh);
		glutReshapeFunc(resize);
		glutMouseFunc(mouse_click_event);
		glutPassiveMotionFunc(mouse_movement);
		glutMotionFunc(mouse_drag_event);
		glutSpecialFunc(special_keys_keyboard);
		glutKeyboardFunc(sim_00_regular_keys_keyboard);

		//glutIdleFunc(refresh);
		glutTimerFunc(1000.0f/FPS, timed_refresh, 0);

		glutMainLoop();
	}

} // -- namespace study_cases
