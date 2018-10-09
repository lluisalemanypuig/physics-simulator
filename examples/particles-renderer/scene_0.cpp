#include "mainwindow.hpp"

// PRIVATE

void MainWindow::make_scene_0(SimulationRenderer *sr) {
	// these parts of the initialiser are scene-dependent

	rect_waterfall w;
	w.set_rectangle_source(-4.5f,10.0f,-4.5f, 9.0f,9.0f);
	w.set_starttime_initialiser(
		[this](particle *p) {
			float s = this->U01(this->eng);
			p->set_starttime( 2.0f*s );
		}
	);

	// the friction,bouncing coefficient, and lifetime
	// are parameter-dependent.
	make_init_with_params(w);

	sr->get_simulator().set_initialiser(&w);

	rplane *floor = new rplane();
	floor->p1 = glm::vec3(-5.0f, 0.0f, -5.0f);
	floor->p2 = glm::vec3(-5.0f, 0.0f,  5.0f);
	floor->p3 = glm::vec3( 5.0f, 0.0f,  5.0f);
	floor->p4 = glm::vec3( 5.0f, 0.0f, -5.0f);
	floor->pl = new plane(vec3(0.0f,1.0f,0.0f), floor->p1);

	sr->add_rgeom(floor);

	sr->add_particles(300);
}
