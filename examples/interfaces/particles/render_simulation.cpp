#include "simulation_renderer.hpp"

// C++ includes
#include <iostream>
using namespace std;

// OpenGL includes
#include <GL/glu.h>

const float rotationFactor = 0.5f;
const float maxRotationCamera = 75.0f;
const float minDistanceCamera = 0.5f;
const float maxDistanceCamera = 50.0f;

// PRIVATE

void SimulationRenderer::draw_particles() {
	glDisable(GL_LIGHTING);

	glBegin(GL_POINTS);
	for (size_t i = 0; i < S.n_particles(); ++i) {
		const physim::math::vec3& pos = S.get_particle(i).cur_pos;
		if (S.get_particle(i).lifetime < 0.2f) {
			glColor3f(0.0f,0.0f,0.0f);
		}
		else {
			glColor3f(1.0f,1.0f,1.0f);
		}
		glVertex3f(pos.x, pos.y, pos.z);
	}
	glEnd();

	glEnable(GL_LIGHTING);
}

void SimulationRenderer::paintGL() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	// without this, scaling will affect the normals!
	glEnable(GL_NORMALIZE);

	glLoadIdentity();

	glTranslatef(0.0f, -5.0f, -15.0f - distance);
	glRotatef(angleX, 1.0f, 0.0f, 0.0f);
	glRotatef(angleY, 0.0f, 1.0f, 0.0f);

	// update screen with the geometry
	for (rgeom *rg : G) {
		if (not rg->should_render()) {
			continue;
		}
		glPushMatrix();
		rg->draw();
		glPopMatrix();
	}

	// update screen with the particles
	glPointSize(particle_size);
	draw_particles();
}

void SimulationRenderer::resizeGL(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (1.0*w)/(1.0*h), 0.01, 100.0);
	glMatrixMode(GL_MODELVIEW);
}

void SimulationRenderer::initializeGL() {
	initializeOpenGLFunctions();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, width()/height(), 0.01, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	float col[] = {1.0f, 1.0f, 1.0f, 1.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, col);
	float pos[] = {0.0f, 0.0f, 0.0f, 1.0f};
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	float amb[] = {0.2f, 0.2f, 0.2f, 1.0f};
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
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

	mouse_last_pos = event->pos();

	update();
}

void SimulationRenderer::wheelEvent(QWheelEvent *event) {
	// Zoom
	float angle = event->angleDelta().y();

	// negative values mean 'zoom in'
	// positive values mean 'zoom out'

	distance -= 0.01f*angle;
	distance = std::max(minDistanceCamera, std::min(distance, maxDistanceCamera));

	update();
}

// PROTECTED

// PUBLIC
