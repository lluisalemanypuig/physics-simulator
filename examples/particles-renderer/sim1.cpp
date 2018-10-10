#include "mainwindow.hpp"

// PRIVATE

void MainWindow::make_sim1(SimulationRenderer *sr) {
	// these parts of the initialiser are scene-dependent

	rect_shower w;
	w.set_flat_source(vec3(-4.5f,10.0f,-1.5f), 3.0f,3.0f);
	w.set_starttime_initialiser(
		[](particle *p) {
			p->set_starttime( p->get_index()/300.0 );
		}
	);

	// the friction,bouncing coefficient, and lifetime
	// are parameter-dependent.
	make_init_with_params(w);

	sr->get_simulator().set_initialiser(&w);

	rrectangle *ramp = new rrectangle();
	ramp->p1 = glm::vec3(-2.0f, 2.0f,  1.0f);
	ramp->p2 = glm::vec3(-2.0f, 2.0f, -1.0f);
	ramp->p3 = glm::vec3(-4.0f, 4.0f, -1.0f);
	ramp->p4 = glm::vec3(-4.0f, 4.0f,  1.0f);
	ramp->rl = new rectangle(ramp->p1,ramp->p2,ramp->p3,ramp->p4);
	ramp->rl->set_position( ramp->rl->get_plane().get_normal()*0.02f );
	ramp->set_color(QVector4D(1.0f, 1.0f, 0.0f, 1.0f));

	rtriangle *bouncer = new rtriangle();
	bouncer->p1 = glm::vec3( 0.0f, 2.0f,  1.0f);
	bouncer->p2 = glm::vec3( 0.0f, 2.0f, -1.0f);
	bouncer->p3 = glm::vec3( 2.0f, 4.0f,  0.0f);
	bouncer->tl = new triangle(bouncer->p1,bouncer->p2,bouncer->p3);
	bouncer->tl->set_position( bouncer->tl->get_plane().get_normal()*0.02f );
	bouncer->set_color(QVector4D(1.0f, 0.0f, 1.0f, 1.0f));

	rplane *floor = new rplane();
	floor->p1 = glm::vec3(-5.0f, -0.05f, -5.0f);
	floor->p2 = glm::vec3(-5.0f, -0.05f,  5.0f);
	floor->p3 = glm::vec3( 5.0f, -0.05f,  5.0f);
	floor->p4 = glm::vec3( 5.0f, -0.05f, -5.0f);
	floor->pl = new plane(vec3(0.0f,1.0f,0.0f), vec3(0.0f,0.0f,0.0f));

	sr->add_rgeom(ramp);
	sr->add_rgeom(bouncer);
	sr->add_rgeom(floor);

	sr->add_particles(300);
}
