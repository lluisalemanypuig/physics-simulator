#include "mainwindow.hpp"

// PRIVATE

void MainWindow::make_scene_1(SimulationRenderer *sr) {
	// these parts of the initialiser are scene-dependent

	rect_waterfall w;
	w.set_rectangle_source(-4.5f,10.0f,-1.5f, 3.0f,3.0f);
	w.set_starttime_initialiser(
		[](particle *p) {
			p->set_starttime( p->get_index()/300.0 );
		}
	);

	// the friction,bouncing coefficient, and lifetime
	// are parameter-dependent.
	make_init_with_params(w);

	sr->get_simulator().set_initialiser(&w);

	rrectangle *bouncer = new rrectangle();
	bouncer->p1 = glm::vec3(-2.0f, 2.0f,  1.0f);
	bouncer->p2 = glm::vec3(-2.0f, 2.0f, -1.0f);
	bouncer->p3 = glm::vec3(-4.0f, 4.0f, -1.0f);
	bouncer->p4 = glm::vec3(-4.0f, 4.0f,  1.0f);
	bouncer->rl = new rectangle(bouncer->p1,bouncer->p2,bouncer->p3,bouncer->p4);
	bouncer->rl->set_position( bouncer->rl->get_plane().get_normal()*0.02f );

	rplane *floor = new rplane();
	floor->p1 = glm::vec3(-5.0f, -0.05f, -5.0f);
	floor->p2 = glm::vec3(-5.0f, -0.05f,  5.0f);
	floor->p3 = glm::vec3( 5.0f, -0.05f,  5.0f);
	floor->p4 = glm::vec3( 5.0f, -0.05f, -5.0f);
	floor->pl = new plane(vec3(0.0f,1.0f,0.0f), vec3(0.0f,0.0f,0.0f));

	sr->add_rgeom(bouncer);
	sr->add_rgeom(floor);

	sr->add_particles(300);
}
