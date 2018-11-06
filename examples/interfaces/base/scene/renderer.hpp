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
class renderer {
	private:

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

		/// Window  width.
		int win_width;
		/// Window height.
		int win_height;

		/// Models in the scene.
		std::vector<rendered_model *> loaded_models;

	protected:
		/// Returns the ratio window width / window height.
		float get_aspect_ratio() const;

	public:
		/// Default constructor.
		renderer();
		/// Destructor.
		virtual ~renderer();

		// MODIFIERS

		/// Clears the models vector (see @ref ms).
		virtual void clear();

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

		/// Increments angle @ref theta.
		void increment_theta(float i);
		/// Increments angle @ref psi.
		void increment_psi(float i);

		/// Increments the pitch of the camera.
		void increment_pitch(float i);
		/// Increments the yaw of the camera.
		void increment_yaw(float i);

		/**
		 * @brief Increments the zoom of both cameras.
		 *
		 * In order to have the same 'zoom' for both cameras,
		 * parameter @e i is scaled with a factor of 0.142
		 * for the orthogonal camera.
		 */
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

		// SETTERS

		void set_perspective(const perspective& p);
		void set_orthogonal(const orthogonal& o);

		void set_VRP(const vec3& VRP);
		void set_theta(float t);
		void set_psi(float p);

		void set_viewer_pos(const vec3& pos);
		void set_yaw(float y);
		void set_pitch(float p);

		// GETTERS

		/// Returns a reference to the perspective camera.
		perspective& get_perspective_camera();

		/// Returns a reference to the orthogonal camera.
		orthogonal& get_orthogonal_camera();

		/// Returns the position of the viewer.
		const vec3& get_viewer_pos() const;
		/// Returns the yaw of the camera.
		float get_yaw() const;
		/// Returns the pitch of the camera.
		float get_pitch() const;

		const vec3& get_VRP() const;
		float get_theta() const;
		float get_psi() const;

		/// Returns the bounding box of the scene.
		box& get_box();
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

