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

static size_t counter = 0;

/* Write a png file */
void write_png(const string& name, unsigned char *data, uint w, uint h) {
	FILE *fp;
	png_structp png_ptr;
	png_infop info_ptr;

	/* Open the file */
	fp = fopen(name.c_str(), "wb");
	if (fp == nullptr) {
		return;
	}

	/* Create and initialize the png_struct with the desired error handler
	* functions.  If you want to use the default stderr and longjump method,
	* you can supply NULL for the last three parameters.  We also check that
	* the library version is compatible with the one used at compile time,
	* in case we are using dynamically linked libraries.  REQUIRED.
	*/
	png_ptr = png_create_write_struct
	(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);

	if (png_ptr == nullptr) {
		fclose(fp);
		return;
	}

	/* Allocate/initialize the image information data.  REQUIRED */
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == nullptr) {
		fclose(fp);
		png_destroy_write_struct(&png_ptr,  png_infopp_NULL);
		return;
	}

	/* Set error handling.  REQUIRED if you aren't supplying your own
	* error handling functions in the png_create_write_struct() call.
	*/
	if (setjmp(png_jmpbuf(png_ptr))) {
		/* If we get here, we had a problem writing the file */
		fclose(fp);
		png_destroy_write_struct(&png_ptr, &info_ptr);
		return;
	}

	/* Set up the output control if you are using standard C streams */
	png_init_io(png_ptr, fp);

	/* Set the image information here.  Width and height are up to 2^31,
	 * bit_depth is one of 1, 2, 4, 8, or 16, but valid values also depend on
	 * the color_type selected. color_type is one of PNG_COLOR_TYPE_GRAY,
	 * PNG_COLOR_TYPE_GRAY_ALPHA, PNG_COLOR_TYPE_PALETTE, PNG_COLOR_TYPE_RGB,
	 * or PNG_COLOR_TYPE_RGB_ALPHA.  interlace is either PNG_INTERLACE_NONE or
	 * PNG_INTERLACE_ADAM7, and the compression_type and filter_type MUST
	 * currently be PNG_COMPRESSION_TYPE_BASE and PNG_FILTER_TYPE_BASE. REQUIRED
	 */
	png_set_IHDR
	(png_ptr, info_ptr, w, h, 8,
		PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_ADAM7,
		PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	png_uint_32 width, height;
	int bit_depth, color_type, interlace_type, compression_type, filter_method;

	png_get_IHDR(png_ptr, info_ptr, &width, &height,
		   &bit_depth, &color_type, &interlace_type,
		   &compression_type, &filter_method);

	/* Optional significant bit (sBIT) chunk */
	png_color_8 sig_bit;
	/* If we are dealing with a grayscale image then */
	sig_bit.gray = 0;
	/* Otherwise, if we are dealing with a color image then */
	sig_bit.red = 1;
	sig_bit.green = 1;
	sig_bit.blue = 1;
	/* If the image has an alpha channel then */
	sig_bit.alpha = 1;
	png_set_sBIT(png_ptr, info_ptr, &sig_bit);

	png_write_info(png_ptr, info_ptr);

	png_byte **row_pointers =
		static_cast<png_byte **>(malloc(h*sizeof(png_byte *)));

	for (uint i = 0; i < w*h; i += w) {
		row_pointers[i/w] = &data[4*i];
	}
	png_write_image(png_ptr, row_pointers);

	/* Similarly, if you png_malloced any data that you passed in with
	* png_set_something(), such as a hist or trans array, free it here,
	* when you can be sure that libpng is through with it.
	*/
	png_free(png_ptr, nullptr);

	/* Whenever you use png_free() it is a good idea to set the pointer to
	* NULL in case your application inadvertently tries to png_free() it
	* again.  When png_free() sees a NULL it returns without action, thus
	* avoiding the double-free security problem.
	*/

	/* Clean up after the write, and free any memory allocated */
	png_destroy_write_struct(&png_ptr, &info_ptr);

	/* Close the file */
	fclose(fp);
}

string int_to_string(size_t v) {
	if (v < 10) {
		return "00000" + std::to_string(v);
	}
	if (v < 100) {
		return "0000" + std::to_string(v);
	}
	if (v < 1000) {
		return "000" + std::to_string(v);
	}
	if (v < 10000) {
		return "00" + std::to_string(v);
	}
	if (v < 100000) {
		return "0" + std::to_string(v);
	}
	return std::to_string(v);
}

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

	const vector<SP>& ps =
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

		for (const SP& p : ps) {
			float R = 2.0f*p.R;

			model = glm::mat4(1.0f);
			model = glm::translate(model, to_glm(p.cur_pos));
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
		for (const SP& p : ps) {
			glVertex3f(p.cur_pos.x, p.cur_pos.y, p.cur_pos.z);
		}
		glEnd();
	}
}

void refresh() {
	glClearColor(bgd_color.x, bgd_color.y, bgd_color.z, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader_render();

	if (run) {
		for (int i = 0; i < n_iterations; ++i) {
			SR.get_simulator().apply_time_step(num_threads);
		}
	}

	if (record) {
		unsigned char *screen =
		static_cast<unsigned char *>(
			malloc(window_width*window_height*4*sizeof(unsigned char))
		);

		glReadPixels
		(0,0, window_width, window_height, GL_RGBA, GL_UNSIGNED_BYTE, screen);

		string name = "image-" + int_to_string(counter) + ".png";
		write_png(name, screen, window_width, window_height);
		free(screen);
		++counter;
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
