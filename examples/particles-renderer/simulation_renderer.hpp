#pragma once

// C++ includes
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

// Custom includes
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

		int i;

		simulator S;	// simulator object
		float dt;		// time step
		float tt;		// simulation total time

	private:
		void set_projection(float aspect);
		void set_modelview();

	protected:
		void initializeGL();
		void resizeGL(int w, int h);
		void paintGL();

		void mousePressEvent(QMouseEvent *event);
		void mouseMoveEvent(QMouseEvent *event);

	public:
		SimulationRenderer(QWidget *parent);
		~SimulationRenderer();

		// SIMULATION CONTROL

		void run_simulation();		// executes the simulation
		void pause_simulation();	// pauses the simulation
		void reset_simulation();	// resets the simulation
		void clear_simulation();	// clears the simulation

		// GETTERS

		simulator& get_simulator();

		// SETTERS

		void set_fps(float fps);
		void set_show_fps(bool show);
		void set_time_step(float dt);
		void set_total_time(float T);

};

