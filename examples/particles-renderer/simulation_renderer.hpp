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
#include <QMouseEvent>
#include <QMatrix4x4>

// Simulator includes
#include <simulator/simulator.hpp>
using namespace physim;
using namespace geom;

// Custom includes
#include "render_geometry.hpp"
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

		float FPS;		// fps of the simulations
		bool show_fps;	// show fps on screen
		bool exe_sim;	// when false, stop the running simulation
		bool running;	// is true iff the simulation is running

		vector<rgeom *> G;	// wrapped geometrical objects of the scene
		simulator S;		// simulator object
		bool scene_cleared;	// if true then the simulation was cleared
							// the scene needs to be remade
		float dt;			// time step
		float tt;			// simulation total time

	private:
		void set_projection(float aspect);
		void set_modelview();
		void draw_rgeom(rgeom *rg);

	protected:
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
		// Sets the initialiser function to the simulator
		void set_initialiser(const function<void (particle *)>& f);
		// Sets the solver
		void set_solver(const solver_type& s = solver_type::EulerOrig);

		// -- GETTERS

		bool is_scene_cleared() const;

		// -- SETTERS

		// sets 'scene_cleared' to false, so that
		// we can run the application
		void set_scene_made();

		void set_fps(float fps);
		void set_show_fps(bool show);
		void set_time_step(float dt);
		void set_total_time(float T);

};

