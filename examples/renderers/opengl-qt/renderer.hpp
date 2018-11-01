#pragma once

// Qt includes
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QMouseEvent>

// render includes
#include <render/mesh/rendered_mesh.hpp>

class ObjRenderer : public QOpenGLWidget, protected QOpenGLFunctions {
	private:
		float angleX, angleY, distance;
		QPoint lastMousePos;

		rendered_mesh sphere;

	private:
		float get_aspect() const;

	protected:
		void paintGL();
		void resizeGL(int w, int h);
		void initializeGL();

		void mousePressEvent(QMouseEvent *event);
		void mouseMoveEvent(QMouseEvent *event);
		void wheelEvent(QWheelEvent *event);

	public:
		ObjRenderer(QWidget *parent = nullptr);
		~ObjRenderer();

		void load_mesh(const QString& file);
};


