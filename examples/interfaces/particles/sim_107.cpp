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
#include <physim/emitter/sized_emitter.hpp>
#include <physim/input/input.hpp>
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

	typedef math::vec3 pm_vec3;

	void sim_107_initialise_sim() {
		object *O = new object();
		input::read_file("../../interfaces/models", "pipe-geometric.obj", O);
		SR.get_simulator().add_geometry(O);

		float mx = (O->get_min().x + O->get_max().x)/2.0f;
		float mz = (O->get_min().z + O->get_max().z)/2.0f;

		simulator& S = SR.get_simulator();

		sized_emitter *I = SR.get_simulator().get_sized_emitter();
		I->set_pos_initialiser(
			[mx,mz](base_particle *p) { p->cur_pos = pm_vec3(mx + 0.5f,1.86f,mz); }
		);
		I->set_vel_initialiser(
			[](base_particle *p) { p->cur_vel = pm_vec3(0.0f,0.0f,0.0f); }
		);
		I->set_friction_initialiser(
			[](free_particle *p) { p->friction = 0.2f; }
		);
		I->set_bounce_initialiser(
			[](free_particle *p) { p->bouncing = 1.0f; }
		);
		I->set_radius_initialiser(
			[](sized_particle *p) { p->R = 0.25f; }
		);

		S.add_sized_particle();
		S.set_gravity_acceleration(pm_vec3(0.0f, -2.0f, 0.0f));
	}

	void sim_107_initialise_sim_rend() {
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

		wireframe_sphere = new rendered_triangle_mesh();
		obj.load_object("../../interfaces/models", "sphere_fsmooth.obj", *wireframe_sphere);

		if (use_shaders) {
			glut_functions::init_shaders();

			SR.get_box().make_buffers();
			ro->make_boxes_buffers();

			model_pipe->make_buffers_materials_textures();
			shader& ts = texture_shader;
			ts.bind();
			shader_helper::activate_materials_textures(*model_pipe, ts);
			ts.release();

			wireframe_sphere->make_buffers();
		}
		else {
			model_pipe->compile();
			wireframe_sphere->compile();
		}
	}

	void sim_107_make_simulation() {
		n_iterations = 10;
		time_step = 0.01f;
		SR.get_simulator().set_time_step(time_step);

		draw_sized_particles_wire = true;
		bgd_color = glm::vec3(0.0f,0.0f,0.0f);

		sim_107_initialise_sim();
		sim_107_initialise_sim_rend();

		SR.set_window_dims(iw, ih);
		SR.init_cameras();

		n_iterations = 1;
	}

	void sim_107_help() {
		glut_functions::help();

		cout << "Simulation 107 description:" << endl;
		cout << endl;
		cout << "PENDING" << endl;
		cout << endl;
	}

	void sim_107_reset() {
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

		use_shaders = false;
		sim_107_make_simulation();

		SR.set_perspective(old_p);
		SR.set_orthogonal(old_o);
		SR.set_VRP(VRP);
		SR.set_theta(theta);
		SR.set_psi(psi);
		SR.set_viewer_pos(viewer_pos);
		SR.set_yaw(yaw);
		SR.set_pitch(pitch);
	}

	void sim_107_regular_keys_keyboard(unsigned char c, int x, int y) {
		regular_keys_keyboard(c, x, y);

		switch (c) {
		case 'h':
			sim_107_help();
			break;
		case 'r':
			sim_107_reset();
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

	int sim_107_initGL(int argc, char *argv[]) {
		// ----------------- //
		/* initialise window */
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
		glutInitWindowPosition(50, 25);
		glutInitWindowSize(iw, ih);
		window_id = glutCreateWindow("Particles - Simulation 107");

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
		sim_107_make_simulation();
		return 0;
	}

	void sim_107(int argc, char *argv[]) {
		sim_107_initGL(argc, argv);
		sim_107_help();

		glutDisplayFunc(glut_functions::refresh);
		glutReshapeFunc(glut_functions::resize);
		glutMouseFunc(glut_functions::mouse_click_event);
		glutPassiveMotionFunc(glut_functions::mouse_movement);
		glutMotionFunc(glut_functions::mouse_drag_event);
		glutSpecialFunc(glut_functions::special_keys_keyboard);
		glutKeyboardFunc(sim_107_regular_keys_keyboard);

		//glutIdleFunc(refresh);
		glutTimerFunc(1000.0f/FPS, glut_functions::timed_refresh, 0);

		glutMainLoop();
	}

} // -- namespace study_cases
