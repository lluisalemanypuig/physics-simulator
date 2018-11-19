#include <base/scene/renderer.hpp>

// C incldues
#include <assert.h>

// C++ includes
#include <iostream>
#include <cmath>
using namespace std;

// glm includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// base includes
#include <base/include_gl.hpp>

// PRIVATE

void renderer::make_model_box(model *m) {
	if (loaded_models.size() == 1) {
		m->make_box(B);
	}
	else {
		box b;
		m->make_box(b);
		B.enlarge_box(b);
	}
}

// PROTECTED

float renderer::get_aspect_ratio() const {
	return float(win_width)/float(win_height);
}

// PUBLIC

renderer::renderer() {
	cam_pos = glm::vec3(0.0f,0.0f,0.0f);
	pitch = 0.0f;
	yaw = 0.0f;

	pitch = 0.0f;
	yaw = 0.0f;

	use_perspective = true;
	use_orthogonal = false;

	inspect = true;
	fly = false;
}

renderer::~renderer() {
	clear();
}

void renderer::clear() {
	for (size_t i = 0; i < loaded_models.size(); ++i) {
		loaded_models[i]->clear();
		delete loaded_models[i];
	}
	loaded_models.clear();
}

bool renderer::init_shader
(const string& dir, const string& vert, const string& frag)
{
	return shader_program.init(dir, vert, frag);
}

void renderer::add_model(rendered_model *m) {
	assert(m != nullptr);
	loaded_models.push_back(m);
	make_model_box(m);
}

void renderer::set_window_dims(int w, int h) {
	win_width = w;
	win_height = h;
}

void renderer::init_cameras() {
	orth_cam.init_camera(B);
	pers_cam.init_camera(B);
	orth_cam.adapt_camera_to_viewport(B, win_width, win_height);
	pers_cam.adapt_camera_to_viewport(win_width, win_height);

	orth_cam.set_znear(0.1f);
	orth_cam.set_zfar(1.5f*orth_cam.get_zfar());

	pers_cam.set_znear(0.1f);
	pers_cam.set_zfar(1.5f*pers_cam.get_zfar());

	VRP = B.get_middle_point();
	diag_length = B.get_diag_length();

	cam_pos = VRP;
	cam_pos.z += 5.0f;
}

void renderer::increment_theta(float i)	{ theta += i; }
void renderer::increment_psi(float i)	{ psi += i; }
void renderer::increment_pitch(float i)	{ pitch += i; }
void renderer::increment_yaw(float i)	{ yaw += i; }

void renderer::increment_zoom(float i) {
	pers_cam.zoom(i);
	orth_cam.zoom(0.142f*i);
}

void renderer::move_camera(float vel, float dir) {
	float rad = (yaw + dir)*M_PI/180.0;
	cam_pos.x -= sin(rad)*vel;
	cam_pos.z -= cos(rad)*vel;
}

void renderer::tilt_camera_up(float vel, float dir) {
	float rad = (pitch + dir)*M_PI/180.0;
	cam_pos.y += sin(rad)*vel;
}

void renderer::camera_forwards(float vel) {
	if (pitch != 90.0f and pitch != -90.0f) {
		move_camera(vel, 0.0f);
	}
	tilt_camera_up(vel, 0.0f);
}

void renderer::camera_backwards(float vel) {
	if (pitch != 90.0f and pitch != -90.0f) {
		move_camera(vel, 180.0f);
	}
	tilt_camera_up(vel, 180.0f);
}

void renderer::camera_sideways_left(float vel) {
	move_camera(vel, 90.0);
}

void renderer::camera_sideways_right(float vel) {
	move_camera(vel, 270.0);
}

void renderer::switch_to_perspective()	{ use_perspective = true; use_orthogonal = false; }
void renderer::switch_to_orthogonal()	{ use_perspective = false; use_orthogonal = true; }

void renderer::switch_to_inspection()	{ inspect = true; fly = false; }
void renderer::switch_to_flight()		{ inspect = false; fly = true; }

// SETTERS

void renderer::set_perspective(const perspective& p) { pers_cam = p; }
void renderer::set_orthogonal(const orthogonal& o) { orth_cam = o; }
void renderer::set_VRP(const glm::vec3& _VRP) { VRP = _VRP; }
void renderer::set_theta(float t) { theta = t; }
void renderer::set_psi(float p) { psi = p; }
void renderer::set_viewer_pos(const glm::vec3& pos) { cam_pos = pos; }
void renderer::set_yaw(float y) { yaw = y; }
void renderer::set_pitch(float p) { pitch = p; }

// GETTERS

shader& renderer::get_shader() {
	return shader_program;
}
const shader& renderer::get_shader() const {
	return shader_program;
}

perspective& renderer::get_perspective_camera() {
	return pers_cam;
}

orthogonal& renderer::get_orthogonal_camera() {
	return orth_cam;
}

const glm::vec3& renderer::get_viewer_pos() const {
	return cam_pos;
}

box& renderer::get_box() {
	return B;
}

const box& renderer::get_box() const {
	return B;
}

int renderer::window_width() const {
	return win_width;
}
int renderer::window_height() const {
	return win_height;
}

bool renderer::is_flying() const {
	return fly;
}
bool renderer::is_inspecting() const {
	return inspect;
}

float renderer::get_yaw() const	{ return yaw; }
float renderer::get_pitch() const{ return pitch; }

const glm::vec3& renderer::get_VRP() const { return VRP; }
float renderer::get_theta() const { return theta; }
float renderer::get_psi() const { return psi; }

// OpenGL

void renderer::apply_projection(bool use_shader) const {
	bool error = false;
	if (use_shader) {
		glm::mat4 projection;
		if (use_perspective) {
			projection = glm::perspective(
				pers_cam.get_FOV(), pers_cam.getRAw(),
				pers_cam.get_znear(), pers_cam.get_zfar()
			);
		}
		else if (use_orthogonal) {
			projection = glm::ortho(
				orth_cam.get_left(), orth_cam.get_right(),
				orth_cam.get_bottom(), orth_cam.get_top(),
				orth_cam.get_znear(), orth_cam.get_zfar()
			);
		}
		else {
			error = true;
		}

		if (not error) {
			shader_program.bind();
			shader_program.set_mat4("projection", projection);
			shader_program.release();
		}
	}
	else {
		if (use_perspective) {
			gluPerspective(
				pers_cam.get_FOV(), pers_cam.getRAw(),
				pers_cam.get_znear(), pers_cam.get_zfar()
			);
		}
		else if (use_orthogonal) {
			glOrtho(
				orth_cam.get_left(), orth_cam.get_right(),
				orth_cam.get_bottom(), orth_cam.get_top(),
				orth_cam.get_znear(), orth_cam.get_zfar()
			);
		}
		else {
			error = true;
		}
	}

	if (error) {
		cerr << "void simulation_renderer::apply_view_mode() - Error!" << endl;
		cerr << "    Something went wrong with the cameras!" << endl;
		cerr << "    No perspective or orthogonal camera activated" << endl;
		assert(false);
	}
}

void renderer::apply_modelview(bool use_shader) const {
	bool error = false;
	if (use_shader) {
		glm::mat4 modelview;
		glm::mat3 normal_matrix;
		if (inspect) {
			modelview = glm::translate(modelview, glm::vec3(0.0f, 0.0f, -diag_length));
			modelview = glm::rotate(modelview, theta, glm::vec3(1.0f, 0.0f, 0.0f));
			modelview = glm::rotate(modelview, -psi, glm::vec3(0.0f, 1.0f, 0.0f));
			modelview = glm::translate(modelview, glm::vec3(-VRP.x, -VRP.y, -VRP.z));

			normal_matrix =
				glm::transpose(glm::inverse(glm::mat3(modelview)));
		}
		else if (fly) {
			modelview = glm::rotate(modelview, -pitch, glm::vec3(1.0f, 0.0f, 0.0f));
			modelview = glm::rotate(modelview, -yaw, glm::vec3(0.0f, 1.0f, 0.0f));
			modelview = glm::translate(modelview, glm::vec3(-cam_pos.x, -cam_pos.y, -cam_pos.z));

			normal_matrix =
				glm::transpose(glm::inverse(glm::mat3(modelview)));
		}
		else {
			error = true;
		}

		if (not error) {
			shader_program.bind();
			shader_program.set_mat4("modelview", modelview);
			shader_program.set_mat3("normal_matrix", normal_matrix);
			shader_program.release();
		}
	}
	else {
		if (inspect) {
			glTranslatef(0.0f, 0.0f, -diag_length);
			glRotatef(theta, 1.0f, 0.0f, 0.0f);
			glRotatef(-psi, 0.0f, 1.0f, 0.0f);
			glTranslatef(-VRP.x, -VRP.y, -VRP.z);
		}
		else if (fly) {
			glRotatef(-pitch, 1.0f, 0.0f, 0.0f);
			glRotatef(-yaw, 0.0f, 1.0f, 0.0f);
			glTranslatef(-cam_pos.x, -cam_pos.y, -cam_pos.z);
		}
		else {
			error = true;
		}
	}

	if (error) {
		cerr << "void simulation_renderer::apply_camera() - Error!" << endl;
		cerr << "    Something went wrong with the cameras!" << endl;
		cerr << "    No inspect or fly mode activated" << endl;
		assert(false);
	}
}

void renderer::render_models(bool use_shader) const {
	if (use_shader) {
		shader_program.bind();
	}
	for (rendered_model *m : loaded_models) {
		m->render();
	}
	if (use_shader) {
		shader_program.release();
	}
}
