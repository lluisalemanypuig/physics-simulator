#pragma once

// C++ includes
#include <functional>
#include <iostream>
using namespace std;

// Qt includes
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QApplication>
#include <QProgressBar>
#include <QMouseEvent>
#include <QMatrix4x4>
#include <QLabel>

// Simulator includes
#include <simulator/simulator.hpp>
#include <simulator/initialiser/initialiser.hpp>
using namespace physim;
using namespace geom;
using namespace init;

// Custom includes
#include "render_geometry.hpp"
#include "OBJ_reader.hpp"
#include "utils.hpp"

class SimulationRenderer : public QOpenGLWidget, protected QOpenGLFunctions {
	private:
		// camera pitch and yaw, and object's
		// distance from the camera
		float angleX, angleY, distance;
		// track mouse last position
		QPoint mouse_last_pos;

		// shader for rendering objects
		QOpenGLShaderProgram *program;

		float particle_size;// size of the particle

		bool limit_fps;		// limit or not the frames per second
		int fps_count;		// measured amount of fps
		double FPS;			// fps of the simulations
		bool exe_sim;		// when false, stop the running simulation

		vector<rgeom *> G;	// wrapped geometrical objects of the scene
		simulator S;		// simulator object
		bool scene_cleared;	// if true then the simulation was cleared
							// the scene needs to be remade
		float dt;			// time step
		float tt;			// simulation total time
		int sim_steps;		// number of steps of the simulation

		GLUquadricObj *sphere;
		GLuint sphere_idx;
		OBJ_reader obj;

		QProgressBar *p_bar;// the progress bar of the simulation
		QLabel *showFPS;

	private:
		// scene-rendering functions
		void set_projection(float aspect);
		void set_modelview();
		void draw_geom(rgeom *rg);
		void draw_particles();

		void initializeGL();
		void resizeGL(int w, int h);
		void paintGL();

		void mousePressEvent(QMouseEvent *event);
		void mouseMoveEvent(QMouseEvent *event);

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
		void set_solver(const solver_type& s = solver_type::EulerOrig);

		// -- GETTERS

		simulator& get_simulator();
		bool is_scene_cleared() const;

		float get_time_step() const;
		float get_total_time() const;

		// -- SETTERS

		void set_particle_size(float s);

		void set_progress_bar(QProgressBar *pbar);
		void set_label_show_fps(QLabel *show_fps);

		void set_limit_fps(bool l);

		// sets 'scene_cleared' to false, so that
		// we can run the application
		void set_scene_made();

		void set_fps(double fps);
		void set_time_step(float dt);
		void set_total_time(float T);

		void set_bounce_all_particles(float b);
		void set_friction_all_particles(float f);
};

