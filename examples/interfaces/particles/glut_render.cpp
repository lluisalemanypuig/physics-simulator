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
typedef physim::particles::sized_particle SP;

// render includes
#include <render/geometry/robject.hpp>

// custom includes
#include "conversion_helper.hpp"

#include <png.h>

namespace glut_functions {

	// ---------------
	// SCENE RENDERING
	// ---------------

	void shader_render() {
		// texture shader for geometry
		glm::mat4 projection(1.0f), view(1.0f);
		SR.make_projection_matrix(projection);
		SR.make_view_matrix(view);

		/* TEXTURE BIND */
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
				glm::mat3 normal_matrix = glm::inverseTranspose(glm::mat3(modelview));

				texture_shader.set_mat4("modelview", modelview);
				texture_shader.set_mat3("normal_matrix", normal_matrix);
				m->render();
			}
		}
		texture_shader.release();
		/* TEXTURE RELEASE */

		const vector<SP *>& ps =
			SR.get_simulator().get_sized_particles();

		/* FLAT BIND */
		flat_shader.bind();
		flat_shader.set_vec3("view_pos", glm::vec3(0.f,0.f,0.f));
		flat_shader.set_mat4("projection", projection);
		if (draw_sized_particles_wire) {
			// draw sized particles using wireframe sphere
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

			glm::mat4 model;
			glm::mat4 modelview;
			glm::mat3 normal_matrix;

			flat_shader.set_bool("wireframe", true);
			flat_shader.set_vec4("colour", glm::vec4(0.0f,0.0f,1.0f,1.0f));

			for (const SP *p : ps) {
				float R = 2.0f*p->R;

				model = glm::mat4(1.0f);
				model = glm::translate(model, to_glm(p->cur_pos));
				model = glm::scale(model, glm::vec3(R,R,R));
				modelview = view*model;
				normal_matrix = glm::inverseTranspose(glm::mat3(modelview));

				flat_shader.set_mat4("modelview", modelview);
				flat_shader.set_mat3("normal_matrix", normal_matrix);
				wireframe_sphere->render();
			}
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glm::mat4 model(1.0f);
		glm::mat4 modelview = model*view;
		glm::mat3 normal_matrix = glm::inverseTranspose(glm::mat3(modelview));

		flat_shader.set_bool("wireframe", true);
		flat_shader.set_mat4("modelview", modelview);
		flat_shader.set_mat3("normal_matrix", normal_matrix);
		if (draw_box) {
			flat_shader.set_vec4("colour", glm::vec4(1.0f,0.0f,0.0f,1.0f));
			SR.get_box().fast_render();
		}
		if (draw_boxes_octree) {
			flat_shader.set_vec4("colour", glm::vec4(1.0f,1.0f,0.0f,1.0f));
			for (const rgeom *r : SR.get_geometry()) {
				if (r->get_type() == rendered_geometry_type::object) {
					const robject *ro = static_cast<const robject *>(r);
					const vector<box>& bs = ro->get_boxes();
					for (const box& B : bs) {
						B.fast_render();
					}
				}
			}
		}
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		flat_shader.release();
		/* FLAT RELEASE */

		// draw flat geometry
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		SR.apply_projection();

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		SR.apply_view();

		glDisable(GL_LIGHTING);
		SR.render_simulation();

		glDisable(GL_LIGHTING);
		for (const rgeom *r : SR.get_geometry()) {
			if (r->get_model() == nullptr) {
				r->draw_geometry();
			}
		}

		if (not draw_sized_particles_wire and ps.size() > 0) {
			glPointSize(particle_size);
			glColor3f(0.0f,0.0f,1.0f);
			glBegin(GL_POINTS);
			for (const SP *p : ps) {
				glVertex3f(p->cur_pos.x, p->cur_pos.y, p->cur_pos.z);
			}
			glEnd();
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

		glDisable(GL_LIGHTING);
		SR.render_simulation();

		glDisable(GL_LIGHTING);
		for (const rgeom *r : SR.get_geometry()) {
			if (r->get_model() == nullptr) {
				r->draw_geometry();
			}
		}
		glEnable(GL_LIGHTING);
		for (const rgeom *r : SR.get_geometry()) {
			if (r->get_model() != nullptr) {
				r->draw();
			}
		}

		glDisable(GL_LIGHTING);
		if (draw_box) {
			glColor3f(1.0f,0.0f,0.0f);
			SR.get_box().slow_render();
		}
		if (draw_boxes_octree) {
			glColor3f(1.0f,1.0f,0.0f);
			for (const rgeom *r : SR.get_geometry()) {
				if (r->get_type() == rendered_geometry_type::object) {
					const robject *ro = static_cast<const robject *>(r);
					const vector<box>& bs = ro->get_boxes();
					for (const box& B : bs) {
						B.slow_render();
					}
				}
			}
		}

		const vector<SP *>& ps =
			SR.get_simulator().get_sized_particles();

		if (draw_sized_particles_wire) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glColor3f(0.0f,0.0f,1.0f);

			for (const SP *p : ps) {
				float R = 2.0f*p->R;

				glPushMatrix();
					glTranslatef(p->cur_pos.x, p->cur_pos.y, p->cur_pos.z);
					glScalef(R,R,R);
					wireframe_sphere->render();
				glPopMatrix();
			}
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		else if (ps.size() > 0) {
			glEnable(GL_LIGHTING);
			glPointSize(particle_size);
			glBegin(GL_POINTS);
			glColor3f(0.0f,0.0f,1.0f);
			for (const SP *p : ps) {
				glVertex3f(p->cur_pos.x, p->cur_pos.y, p->cur_pos.z);
			}
			glEnd();
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

		for (int i = 0; i < n_iterations; ++i) {
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
