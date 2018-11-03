#pragma once

// C++ inlcudes
#include <vector>

// base includes
#include <base/model/rendered_model.hpp>
#include <base/cameras/perspective.hpp>
#include <base/cameras/orthogonal.hpp>
#include <base/box.hpp>

// physim includes
#include <physim/math/vec3.hpp>
typedef physim::math::vec3 vec3;

/**
 * @brief Class used for rendering and navigating
 * a scene.
 *
 * This class does not contain the definition of
 * the functions GLUT needs.
 */
class simple_renderer {
	private:
		/**
		 * @brief Make the bounding box of the model @e m.
		 *
		 * The box has its faces parallel to the axis.
		 * @param m Non-null pointer to model.
		 */
		void make_model_box(model *m);

	protected:
		/// Bounding box of the models.
		box B;

		// Used only when flying.

		/// Position of the camera.
		vec3 cam_pos;
		/// Camera pitch.
		float pitch;
		/// Camera yaw.
		float yaw;

		// Used only when inspecting

		/// Bounding box's diagonal length.
		float diag_length;
		/// View Reference Point.
		vec3 VRP;
		/// Rotation angle theta.
		float theta;
		/// Rotation angle psi.
		float psi;

		/// Perspective camera.
		perspective pers_cam;
		/// Orthogonal camera.
		orthogonal orth_cam;

		/// Use perspective camera.
		bool use_perspective;
		/// Use orthogonal camera.
		bool use_orthogonal;

		/// Fix the camera so that we see the whole scene.
		bool inspect;
		/// The camera flies free in the scene.
		bool fly;

		/// Viewport width.
		int vp_width;
		/// Viewport height.
		int vp_height;

		/// Models in the scene.
		std::vector<rendered_model *> ms;

	protected:
		float get_aspect_ratio() const;

	public:
		simple_renderer(int w = 640, int h = 480);
		~simple_renderer();

		// MODIFIERS

		/// Clears the models vector (see @ref ms).
		void clear();

		/**
		 * @brief Adds a model to the simulation.
		 *
		 * After every addition the bounding box is
		 * enlarged to fit this model.
		 * @param m Non-null pointer to a model.
		 */
		void add_model(rendered_model *m);

		/**
		 * @brief Set window dimensions.
		 * @param w Width of the window.
		 * @param h Height of the window.
		 */
		void set_window_dims(int w, int h);

		/**
		 * @brief Initialise the perspective and orthogonal
		 * cameras.
		 *
		 * Needs:
		 * - models in simulation.
		 * - window size
		 */
		void init_cameras();

		void increment_theta(float i);
		void increment_psi(float i);

		void increment_pitch(float i);
		void increment_yaw(float i);

		void increment_zoom(float i);

		void move_camera(float vel, float dir);
		void tilt_camera_up(float vel, float dir);

		void camera_forwards(float vel);
		void camera_backwards(float vel);
		void camera_sideways_left(float vel);
		void camera_sideways_right(float vel);

		void switch_to_perspective();
		void switch_to_orthogonal();

		void switch_to_inspection();
		void switch_to_flight();

		// GETTERS

		/// Returns a reference to the perspective camera.
		perspective& get_perspective_camera();

		/// Returns a reference to the orthogonal camera.
		orthogonal& get_orthogonal_camera();

		/// Returns the position of the viewer.
		const vec3& get_viewer_pos() const;

		/// Returns the bounding box of the scene.
		const box& get_box() const;

		/// Returns the width of the window.
		int window_width() const;
		/// Returns the height of the window.
		int window_height() const;

		/// Returns whether the camera is flying.
		bool is_flying() const;
		/// Returns whether the camera is inspecting.
		bool is_inspecting() const;

		float get_yaw() const;
		float get_pitch() const;

		// OpenGL

		/**
		 * @brief Makes the corresponding calls to render the
		 * scene using the internal camera.
		 *
		 * Calls either gluPerspective, or glOrtho.
		 */
		void apply_view_mode() const;

		/**
		 * @brief Move the scene according to the internal state.
		 *
		 * Either inspecting or flying.
		 */
		void apply_camera() const;

		/// Renders the models
		void render_models() const;
};

