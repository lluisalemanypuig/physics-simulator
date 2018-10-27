#include "simulation_renderer.hpp"

const float rotationFactor = 0.5f;
const float maxRotationCamera = 75.0f;
const float minDistanceCamera = 0.5f;
const float maxDistanceCamera = 50.0f;

// PRIVATE

void SimulationRenderer::draw_hard_geom(rgeom *rg) {
	rendered_geometry_type type = rg->get_type();

	rplane *rp = nullptr;
	rrectangle *rc = nullptr;
	rtriangle *rt = nullptr;

	glDisable(GL_LIGHTING);

	glColor4f(rg->r,rg->b,rg->g,rg->a);

	switch (type) {
		case rendered_geometry_type::plane:
			rp = static_cast<rplane *>(rg);
			glBegin(GL_QUADS);
				glVertex3f(rp->p1.x, rp->p1.y, rp->p1.z);
				glVertex3f(rp->p2.x, rp->p2.y, rp->p2.z);
				glVertex3f(rp->p3.x, rp->p3.y, rp->p3.z);
				glVertex3f(rp->p4.x, rp->p4.y, rp->p4.z);
			glEnd();
			break;

		case rendered_geometry_type::rectangle:
			rc = static_cast<rrectangle *>(rg);
			glBegin(GL_QUADS);
				glVertex3f(rc->p1.x, rc->p1.y, rc->p1.z);
				glVertex3f(rc->p2.x, rc->p2.y, rc->p2.z);
				glVertex3f(rc->p3.x, rc->p3.y, rc->p3.z);
				glVertex3f(rc->p4.x, rc->p4.y, rc->p4.z);
			glEnd();
			break;

		case rendered_geometry_type::triangle:
			rt = static_cast<rtriangle *>(rg);
			glBegin(GL_TRIANGLES);
				glVertex3f(rt->p1.x, rt->p1.y, rt->p1.z);
				glVertex3f(rt->p2.x, rt->p2.y, rt->p2.z);
				glVertex3f(rt->p3.x, rt->p3.y, rt->p3.z);
			glEnd();
			break;

		default:
			cerr << "This type (" << int(type) << ") is not hard geometry" << endl;
	}
	glEnable(GL_LIGHTING);
}

void SimulationRenderer::draw_soft_geom(rgeom *rg) {
	rendered_geometry_type type = rg->get_type();
	rsphere *rs = nullptr;

	switch (type) {
		case rendered_geometry_type::sphere:
			rs = static_cast<rsphere *>(rg);
			glTranslatef(rs->c.x, rs->c.y, rs->c.z);
			glScalef(2.0f*rs->r,2.0f*rs->r,2.0f*rs->r);
			sphere->render();
			break;

		default:
			cerr << "This type (" << int(type) << ") is not soft geometry" << endl;
	}
}

void SimulationRenderer::draw_geom(rgeom *rg) {
	rendered_geometry_type type = rg->get_type();

	switch (type) {
		case rendered_geometry_type::plane:
		case rendered_geometry_type::rectangle:
		case rendered_geometry_type::triangle:
			draw_hard_geom(rg);
			break;

		case rendered_geometry_type::sphere:
			draw_soft_geom(rg);
			break;

		default:
			cerr << "No type for geometry" << endl;
	}
}

void SimulationRenderer::draw_particles() {
	glDisable(GL_LIGHTING);

	glColor3f(1.0f,1.0f,1.0f);
	glBegin(GL_POINTS);
	for (size_t i = 0; i < S.n_particles(); ++i) {
		const physim::math::vec3& pos = S.get_particle(i).get_position();
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
		draw_geom(rg);
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
