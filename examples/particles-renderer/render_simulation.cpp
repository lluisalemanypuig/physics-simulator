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

	modelviewMatrix.translate(0.0f, 0.0f, -20.0f);
	modelviewMatrix.translate(0.0f, 0.0f, -distance);
	modelviewMatrix.rotate(angleX, 1.0f, 0.0f, 0.0f);
	modelviewMatrix.rotate(angleY, 0.0f, 1.0f, 0.0f);

	program->bind();
	program->setUniformValue("modelview", modelviewMatrix);
	program->setUniformValue("normalMatrix", modelviewMatrix.normalMatrix());
	program->release();
}

void SimulationRenderer::draw_geom(rgeom *rg) {
	geom::geometry *g = rg->get_underlying();
	geom_type gt = g->get_geom_type();

	rplane *rp = nullptr;

	switch (gt) {
		case geom_type::Plane:
			program->bind();
			program->setUniformValue("color", QVector4D(1.0f, 0.0f, 0.0f, 1.0));

			rp = static_cast<rplane *>(rg);
			glBegin(GL_QUADS);
				glVertex3f(rp->p1.x, rp->p1.y, rp->p1.z);
				glVertex3f(rp->p2.x, rp->p2.y, rp->p2.z);
				glVertex3f(rp->p3.x, rp->p3.y, rp->p3.z);
				glVertex3f(rp->p4.x, rp->p4.y, rp->p4.z);
			glEnd();
			program->release();
			break;

		case geom_type::Triangle:

		case geom_type::Sphere:

		case geom_type::none:
		default:
			cerr << "No type for geometry" << endl;
	}
}

void SimulationRenderer::draw_particles() {
	program->bind();
	program->setUniformValue("color", QVector4D(1.0f, 1.0f, 1.0f, 1.0));

	for (size_t i = 0; i < S.n_particles(); ++i) {
		const vec3& pos = S.get_particle(i).get_position();

		glBegin(GL_POINTS);
		glVertex3f(pos.x, pos.y, pos.z);
		glEnd();
	}

	program->release();
}

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

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
}

void SimulationRenderer::resizeGL(int w, int h) {
	glViewport(0, 0, w, h);
	set_projection((float)w/h);
	set_modelview();
}

void SimulationRenderer::paintGL() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// update screen with the geometry
	for (rgeom *rg : G) {
		draw_geom(rg);
	}

	// update screen with the particles
	draw_particles();
}

void SimulationRenderer::mousePressEvent(QMouseEvent *event) {
	mouse_last_pos = event->pos();
}

void SimulationRenderer::mouseMoveEvent(QMouseEvent *event) {
	// Rotation
	if (event->buttons() & Qt::LeftButton) {
		angleX += rotationFactor*(event->y() - mouse_last_pos.y());
		angleX = std::max(-maxRotationCamera, std::min(angleX, maxRotationCamera));
		angleY += rotationFactor*(event->x() - mouse_last_pos.x());
	}
	// Zoom
	if (event->buttons() & Qt::RightButton) {
		distance += 0.01f*(event->y() - mouse_last_pos.y());
		distance = std::max(minDistanceCamera, std::min(distance, maxDistanceCamera));
	}

	mouse_last_pos = event->pos();

	makeCurrent();
	set_modelview();
	doneCurrent();
	update();
}

// PROTECTED

// PUBLIC
