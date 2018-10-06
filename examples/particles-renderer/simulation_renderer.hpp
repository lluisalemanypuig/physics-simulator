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

class SimulationRenderer : public QOpenGLWidget, protected QOpenGLFunctions {
	private:
		float angleX, angleY, distance;
		QPoint lastMousePos;

		QOpenGLShaderProgram *program;

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

		void display_simulation();

};

