#include "simulation_renderer.hpp"

// PRIVATE

// PROTECTED

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

	// scene_cleared: set to true so that it can
	// be built when launching the application
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

	running = true;
	timing::time_point begin, end;

	while (S.get_current_time() <= tt and exe_sim) {
		begin = timing::now();

		//cout << "time step: " << dt << endl;
		S.apply_time_step(dt);

		makeCurrent();
		set_modelview();
		doneCurrent();
		update();

		end = timing::now();
		double frame = timing::elapsed_seconds(begin, end);

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

	makeCurrent();
	set_modelview();
	doneCurrent();
	update();
}

void SimulationRenderer::add_rgeom(rgeom *rg) {
	G.push_back(rg);
	S.add_geometry( rg->get_underlying() );
}

void SimulationRenderer::add_particles(size_t n) {
	S.add_particles(n);
}

void SimulationRenderer::set_solver(const solver_type& s) {
	S.set_solver(s);
}

// GETTERS

simulator& SimulationRenderer::get_simulator() {
	return S;
}

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

void SimulationRenderer::set_bounce_all_particles(float b) {
	for (particle *p : S.get_particles()) {
		p->set_bouncing(b);
	}
}

void SimulationRenderer::set_friction_all_particles(float f) {
	for (particle *p : S.get_particles()) {
		p->set_friction(f);
	}
}
