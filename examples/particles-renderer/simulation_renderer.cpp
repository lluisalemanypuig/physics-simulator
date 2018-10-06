#include "simulation_renderer.hpp"

const float rotationFactor = 0.5f;
const float maxRotationCamera = 75.0f;
const float minDistanceCamera = 1.0f;
const float maxDistanceCamera = 3.0f;

// PRIVATE

void SimulationRenderer::set_projection(float aspect) {
	QMatrix4x4 projectionMatrix;

	projectionMatrix.perspective(60, aspect, 0.01, 100.0);

	program->bind();
	program->setUniformValue("projection", projectionMatrix);
	program->release();
}

void SimulationRenderer::set_modelview() {
	QMatrix4x4 modelviewMatrix;

	modelviewMatrix.translate(0, 0, -distance);
	modelviewMatrix.rotate(angleX, 1.0f, 0.0f, 0.0f);
	modelviewMatrix.rotate(angleY, 0.0f, 1.0f, 0.0f);
	program->bind();
	program->setUniformValue("modelview", modelviewMatrix);
	program->setUniformValue("normalMatrix", modelviewMatrix.normalMatrix());
	program->release();
}

// PROTECTED

void SimulationRenderer::initializeGL() {
	initializeOpenGLFunctions();

	program = new QOpenGLShaderProgram();
	program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/simpleshader.vert");
	program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/simpleshader.frag");
	program->link();
	if (!program->isLinked()) {
		cerr << "SimulationRenderer::initializeGL - Error:" << endl;
		cerr << "Shader program has not linked" << endl;
		cerr << endl;
		cerr << "Log: " << endl;
		cerr << endl;
		cerr << program->log().toStdString();
		QApplication::quit();
	}
	program->bind();

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
}

void SimulationRenderer::resizeGL(int w, int h) {
	glViewport(0, 0, w, h);
	set_projection((float)w/h);
	set_modelview();
}

void SimulationRenderer::paintGL() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	program->bind();
	program->setUniformValue("color", QVector4D(1.0f, 0.0f, 0.0f, 1.0));

	glBegin(GL_TRIANGLES);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 1.0f, 0.0f);
		glVertex3f(1.0f, 1.0f, 0.0f);
	glEnd();

	program->release();
}

void SimulationRenderer::mousePressEvent(QMouseEvent *event) {
	lastMousePos = event->pos();
}

void SimulationRenderer::mouseMoveEvent(QMouseEvent *event) {
	// Rotation
	if (event->buttons() & Qt::LeftButton) {
		angleX += rotationFactor*(event->y() - lastMousePos.y());
		angleX = max(-maxRotationCamera, min(angleX, maxRotationCamera));
		angleY += rotationFactor*(event->x() - lastMousePos.x());
	}
	// Zoom
	if (event->buttons() & Qt::RightButton) {
		distance += 0.01f*(event->y() - lastMousePos.y());
		distance = max(minDistanceCamera, min(distance, maxDistanceCamera));
	}

	lastMousePos = event->pos();

	makeCurrent();
	set_modelview();
	doneCurrent();
	update();
}

// PUBLIC

SimulationRenderer::SimulationRenderer(QWidget *parent) : QOpenGLWidget(parent) {
	angleX = 0.0f;
	angleY = 0.0f;
	distance = 2.0f;

	program = nullptr;
}

SimulationRenderer::~SimulationRenderer() {
	if (program != nullptr) {
		delete program;
	}
}

void SimulationRenderer::display_simulation() {
	for (int i = 0; i < 100; ++i) {
		paintGL();
	}
}
