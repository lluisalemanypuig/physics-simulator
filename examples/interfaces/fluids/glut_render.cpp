#include "glut_functions.hpp"

// C++ includes
#include <iostream>
using namespace std;

// glm includes
#define GLM_FORCE_RADIANS
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

// physim includes
#include <physim/particles/sized_particle.hpp>
#include <physim/fluids/fluid.hpp>
using namespace physim;
using namespace fluids;
using namespace particles;

// render includes
#include <render/geometry/robject.hpp>

// custom includes
#include "conversion_helper.hpp"
#include "glut_variables.hpp"
using namespace glut_variables;

namespace glut_functions {

	// ---------------
	// SCENE RENDERING
	// ---------------

	void refresh() {
		glClearColor(bgd_color.x, bgd_color.y, bgd_color.z, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// texture shader for geometry
		glm::mat4 projection(1.0f), view(1.0f);
		SR.make_projection_matrix(projection);
		SR.make_view_matrix(view);

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

		glDisable(GL_LIGHTING);
		for (const rgeom *r : SR.get_geometry()) {
			if (r->get_model() == nullptr) {
				r->draw_geometry();
			}
		}

		for (int i = 0; i < n_iterations; ++i) {
			SR.get_simulator().simulate_fluids();
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
