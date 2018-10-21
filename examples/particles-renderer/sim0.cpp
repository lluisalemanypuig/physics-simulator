#include "mainwindow.hpp"

// PRIVATE

void MainWindow::make_sim0(SimulationRenderer *sr) {
	// these parts of the initialiser are scene-dependent

	rect_shower w;
	w.set_straight_source(vec3(-4.5f,10.0f,-4.5f), 9.5f,9.5f);
	w.set_starttime_initialiser(
		[](particle *p) {
			p->set_starttime( 0.0f );
		}
	);

	// the friction,bouncing coefficient, and lifetime
	// are parameter-dependent.
	make_init_with_params(w);

	sr->get_simulator().set_initialiser(&w);

	rplane *floor = new rplane();
	floor->p1 = vec3(-5.0f, -0.05f, -5.0f);
	floor->p2 = vec3(-5.0f, -0.05f,  5.0f);
	floor->p3 = vec3( 5.0f, -0.05f,  5.0f);
	floor->p4 = vec3( 5.0f, -0.05f, -5.0f);
	floor->pl = new plane(vec3(0.0f,1.0f,0.0f), vec3(0.0f,0.0f,0.0f));

	sr->add_rgeom(floor);

	sr->add_particles(1000);
}
