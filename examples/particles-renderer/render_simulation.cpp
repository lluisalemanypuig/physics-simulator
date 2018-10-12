#include "simulation_renderer.hpp"

const float rotationFactor = 0.5f;
const float maxRotationCamera = 75.0f;
const float minDistanceCamera = 1.0f;
const float maxDistanceCamera = 20.0f;

// PRIVATE

void SimulationRenderer::draw_hard_geom(rgeom *rg) {
	geom::geometry *g = rg->get_underlying();

	rplane *rp = nullptr;
	rrectangle *rc = nullptr;
	rtriangle *rt = nullptr;

	glDisable(GL_LIGHTING);

	QVector4D col = rg->get_color();
	glColor4f(col.x(),col.y(),col.z(),col.w());

	switch (g->get_geom_type()) {
		case geom_type::Plane:
			rp = static_cast<rplane *>(rg);
			glBegin(GL_QUADS);
				glVertex3f(rp->p1.x, rp->p1.y, rp->p1.z);
				glVertex3f(rp->p2.x, rp->p2.y, rp->p2.z);
				glVertex3f(rp->p3.x, rp->p3.y, rp->p3.z);
				glVertex3f(rp->p4.x, rp->p4.y, rp->p4.z);
			glEnd();
			break;

		case geom_type::Rectangle:
			rc = static_cast<rrectangle *>(rg);
			glBegin(GL_QUADS);
				glVertex3f(rc->p1.x, rc->p1.y, rc->p1.z);
				glVertex3f(rc->p2.x, rc->p2.y, rc->p2.z);
				glVertex3f(rc->p3.x, rc->p3.y, rc->p3.z);
				glVertex3f(rc->p4.x, rc->p4.y, rc->p4.z);
			glEnd();
			break;

		case geom_type::Triangle:
			rt = static_cast<rtriangle *>(rg);
			glBegin(GL_TRIANGLES);
				glVertex3f(rt->p1.x, rt->p1.y, rt->p1.z);
				glVertex3f(rt->p2.x, rt->p2.y, rt->p2.z);
				glVertex3f(rt->p3.x, rt->p3.y, rt->p3.z);
			glEnd();
			break;

		default:
			cerr << "This is not hard geometry" << endl;
			rg->get_underlying()->display();
	}
	glEnable(GL_LIGHTING);
}

void SimulationRenderer::draw_soft_geom(rgeom *rg) {
	geom::geometry *g = rg->get_underlying();
	geom_type gt = g->get_geom_type();

	rsphere *rs = nullptr;

	switch (gt) {
		case geom_type::Sphere:
			rs = static_cast<rsphere *>(rg);
			glTranslatef(rs->c.x, rs->c.y, rs->c.z);
			glScalef(rs->r,rs->r,rs->r);
			sphere->render();
			break;

		default:
			cerr << "This is not soft geometry" << endl;
			rg->get_underlying()->display();
	}
}

void SimulationRenderer::draw_geom(rgeom *rg) {
	const geom::geometry *g = rg->get_underlying();

	switch (g->get_geom_type()) {
		case geom_type::Plane:
		case geom_type::Rectangle:
		case geom_type::Triangle:
			draw_hard_geom(rg);
			break;

		case geom_type::Sphere:
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
		const vec3& pos = S.get_particle(i).get_position();
		glVertex3f(pos.x, pos.y, pos.z);
	}
	glEnd();

	glEnable(GL_LIGHTING);
}

void SimulationRenderer::paintGL() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glLoadIdentity();

	glTranslatef(0.0f, -5.0f, -15.0f);
	glTranslatef(0.0f, 0.0f, -distance);
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
	gluPerspective(60.0f, (1.0f*w)/(1.0f*h), 0.01f, 100.0f);
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
	float col[] = {1.0, 1.0, 1.0, 1.0};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, col);
	float pos[] = {0.0, 0.0, 0.0, 1.0};
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	float amb[] = {0.2, 0.2, 0.2, 1.0};
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
	// Zoom
	if (event->buttons() & Qt::RightButton) {
		distance += 0.01f*(event->y() - mouse_last_pos.y());
		distance = std::max(minDistanceCamera, std::min(distance, maxDistanceCamera));
	}

	mouse_last_pos = event->pos();

	makeCurrent();
	//set_modelview();
	doneCurrent();
	update();
}

// PROTECTED

// PUBLIC
