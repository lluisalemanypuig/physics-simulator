#pragma once

// C++ includes
#include <vector>

// Qt includes
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QProgressBar>
#include <QMouseEvent>
#include <QLabel>

// physim includes
#include <physim/simulator.hpp>

// Custom includes
#include <base/rgeom/rendered_geometry.hpp>
#include <base/model/rendered_model.hpp>
#include "utils.hpp"

class SimulationRenderer : public QOpenGLWidget, protected QOpenGLFunctions {
	Q_OBJECT

	private:
		// camera pitch and yaw, and object's
		// distance from the camera
		float angleX, angleY, distance;
		// track mouse last position
		QPoint mouse_last_pos;

		float particle_size;// size of the particle

		bool limit_fps;		// limit or not the frames per second
		int fps_count;		// measured amount of fps
		double FPS;			// fps of the simulations
		bool allow_run;		// when false, stop the running simulation

		std::vector<rgeom *> G;	// wrapped geometrical objects of the scene
		physim::simulator S;// simulator object
		bool scene_cleared;	// if true then the simulation was cleared
							// the scene needs to be remade
		float dt;			// time step
		float tt;			// simulation total time
		int sim_steps;		// number of steps of the simulation

		rendered_model *sphere;

		QProgressBar *p_bar;// the progress bar of the simulation
		QLabel *label_fps;

	private:
		// hard geometry are those geometrical objects with
		// no material/textures. Soft geometry has materials
		// and textures, so lighting has to be enabled.
		void draw_particles();

		// scene-rendering functions
		void paintGL();
		void resizeGL(int w, int h);
		void initializeGL();

		void mousePressEvent(QMouseEvent *event);
		void mouseMoveEvent(QMouseEvent *event);
		void wheelEvent(QWheelEvent *event);

	signals:
		void simulation_completed();

	public:
		SimulationRenderer(QWidget *parent);
		~SimulationRenderer();

		// -- SIMULATION CONTROL

		void run_simulation();		// executes the simulation
		void pause_simulation();	// pauses the simulation
		void reset_simulation();	// resets the simulation
		void clear_simulation();	// clears the simulation

		// Adds the wrapped geometrical objects to the vector 'G'.
		// Adds the underlying geometrical objects to the simulator 'S'.
		void add_rgeom(rgeom *rg);
		// Adds n particles to the simulator
		void add_particles(size_t n = 1);
		// Sets the solver
		void set_solver(const physim::solver_type& s);

		// -- GETTERS

		physim::simulator& get_simulator();
		bool is_scene_cleared() const;

		float get_time_step() const;
		float get_total_time() const;

		// -- SETTERS

		void allow_to_run();

		void set_particle_size(float s);

		void set_progress_bar(QProgressBar *pbar);
		void set_label_show_fps(QLabel *show_fps);

		void set_limit_fps(bool l);

		void set_sphere(rendered_model *s);

		// sets 'scene_cleared' to false, so that
		// we can run the application
		void set_scene_made();

		void set_fps(double fps);
		void set_time_step(float dt);
		void set_total_time(float T);

		void set_bounce_all_particles(float b);
		void set_friction_all_particles(float f);
};
