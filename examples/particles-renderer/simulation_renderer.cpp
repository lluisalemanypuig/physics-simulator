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

void SimulationRenderer::draw_rgeom(rgeom *rg) {
	geom::geometry *g = rg->get_underlying();
	geom_type gt = g->get_geom_type();

	switch (gt) {
		case geom_type::Plane:
			program->bind();
			program->setUniformValue("color", QVector4D(1.0f, 0.0f, 0.0f, 1.0));

			rplane *rp = static_cast<rplane *>(rg);
			glBegin(GL_QUADS);
				glVertex3f(rp->p1.x, rp->p1.y, rp->p1.z);
				glVertex3f(rp->p2.x, rp->p2.y, rp->p2.z);
				glVertex3f(rp->p3.x, rp->p3.y, rp->p3.z);
				glVertex3f(rp->p4.x, rp->p4.y, rp->p4.z);
			glEnd();
			program->release();
			break;

		default:
			cerr << "No type for geometry" << endl;
	}
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

// PUBLIC

SimulationRenderer::SimulationRenderer(QWidget *parent) : QOpenGLWidget(parent) {
	angleX = 0.0f;
	angleY = 0.0f;
	distance = 2.0f;

	program = nullptr;

	FPS = 60.0f;
	show_fps = true;
	exe_sim = true;
	running = false;

	// set to true so that it can be made
	// when launching the application
	scene_cleared = true;
	dt = 0.01f;
	tt = 10.0f;
}

SimulationRenderer::~SimulationRenderer() {
	if (program != nullptr) {
		delete program;
	}
}

void SimulationRenderer::run_simulation() {
	assert(not scene_cleared);

	/*
	running = true;

	timing::time_point begin, end;

	for (; i < 1000 and exe_sim; ++i) {
		begin = timing::now();

		cout << "Apply time step " << i << endl;

		angleX += 0.1;

		makeCurrent();
		set_modelview();
		doneCurrent();
		update();

		end = timing::now();

		double step_time = timing::elapsed_seconds(begin, end);

		cout << "Elapsed time: " << step_time << endl;
		cout << "    -> wait: " << 1.0f/FPS - step_time << " seconds" << endl;

		QCoreApplication::processEvents();
		timing::sleep_seconds( 1.0/FPS - step_time );
	}

	running = false;
	exe_sim = true;

	if (i == 1000) {
		i = 0;
	}
	*/

	running = true;
	timing::time_point begin, end;

	while (S.get_current_time() <= tt and exe_sim) {
		begin = timing::now();

		S.apply_time_step(dt);

		makeCurrent();

		// update screen with the particles
		// ....
		for (rgeom *rg : G) {
			draw_rgeom(rg);
		}

		doneCurrent();
		update();

		end = timing::now();
		double frame = timing::elapsed_seconds(begin, end);

		cout << "Elapsed time: " << frame << endl;
		cout << "    -> wait: " << 1.0f/FPS - frame << " seconds" << endl;

		QCoreApplication::processEvents();
		timing::sleep_seconds( 1.0/FPS - frame );
	}

	running = false;
	exe_sim = true;

	// the loop may have stopped due to the
	// 'pause' button -> reset simulation
	// only if necessary
	if (S.get_current_time() > tt) {
		S.reset_simulation();
	}
}

void SimulationRenderer::pause_simulation() {
	exe_sim = false;
}

void SimulationRenderer::reset_simulation() {
	S.reset_simulation();
}

void SimulationRenderer::clear_simulation() {
	S.clear_simulation();
	for (rgeom *rg : G) {
		delete rg;
	}
	G.clear();
	scene_cleared = true;
}

void SimulationRenderer::add_rgeom(rgeom *rg) {
	G.push_back(rg);
	S.add_geometry( rg->get_underlying() );
}

void SimulationRenderer::set_initialiser(const function<void (particle *)>& f) {
	S.set_initialiser(f);
}

void SimulationRenderer::add_particles(size_t n) {
	S.add_particles(n);
}

void SimulationRenderer::set_solver(const solver_type& s) {
	S.set_solver(s);
}

// GETTERS

bool SimulationRenderer::is_scene_cleared() const {
	return scene_cleared;
}

// SETTERS

void SimulationRenderer::set_scene_made() {
	scene_cleared = false;
}

void SimulationRenderer::set_fps(float fps) {
	FPS = fps;
}

void SimulationRenderer::set_show_fps(bool show) {
	show_fps = show;
}

void SimulationRenderer::set_time_step(float _dt) {
	dt  = _dt;
}

void SimulationRenderer::set_total_time(float T) {
	tt = T;
}
