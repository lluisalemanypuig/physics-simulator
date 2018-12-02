#include "glut_functions.hpp"

// glm includes
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

// physim includes
#include <physim/particles/sized_particle.hpp>
typedef physim::particles::sized_particle SP;

// custom includes
#include "conversion_helper.hpp"

namespace glut_functions {

	// ---------------
	// SCENE RENDERING
	// ---------------

	void shader_render() {
		// texture shader for geometry
		glm::mat4 projection = SR.make_projection_matrix();
		glm::mat4 view = SR.make_view_matrix();

		texture_shader.bind();
		texture_shader.set_vec3("view_pos", glm::vec3(0.f,0.f,0.f));
		texture_shader.set_mat4("projection", projection);

		// render models of geometry
		for (rgeom *r : SR.get_geometry()) {
			shared_ptr<rendered_triangle_mesh> m = r->get_model();
			if (m != nullptr) {

				glm::mat4 model(1.0f);
				r->make_model_matrix(model);

				glm::mat4 modelview = view*model;
				glm::mat3 normal_matrix = glm::inverseTranspose(
					glm::mat3(modelview)
				);

				texture_shader.set_mat4("modelview", modelview);
				texture_shader.set_mat3("normal_matrix", normal_matrix);

				m->render();
			}
		}
		texture_shader.release();

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		SR.apply_projection();

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		SR.apply_view();

		SR.render_simulation();
		// render geometry without models
		for (rgeom *r : SR.get_geometry()) {
			shared_ptr<rendered_triangle_mesh> m = r->get_model();
			if (m == nullptr) {
				r->draw();
			}
		}
		if (draw_box) {
			glDisable(GL_LIGHTING);
			glColor3f(1.0f,0.0f,0.0f);
			SR.get_box().slow_render();
		}
	}

	void no_shader_render() {
		// no shader for all
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		SR.apply_projection();

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		SR.apply_view();

		SR.render_simulation();
		SR.render_geometry();
		if (draw_box) {
			glDisable(GL_LIGHTING);
			glColor3f(1.0f,0.0f,0.0f);
			SR.get_box().slow_render();
		}
	}

	void refresh() {
		glClearColor(bgd_color.x, bgd_color.y, bgd_color.z, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (use_shaders) {
			shader_render();
		}
		else {
			no_shader_render();
		}

		for (int i = 0; i < 10; ++i) {
			SR.apply_time_step();
		}

		glutSwapBuffers();
	}

	void timed_refresh(int value) {
		refresh();

		++fps_count;
		timing::time_point here = timing::now();
		double elapsed = timing::elapsed_seconds(sec, here);
		if (elapsed >= 1.0) {
			if (display_fps) {
				cout << "fps= " << fps_count << " (" << FPS << ")" << endl;
			}
			fps_count = 0;
			sec = timing::now();
		}

		glutTimerFunc(1000.0f/FPS, timed_refresh, value);
	}

} // -- namespace glut_functions
