#include "mainwindow.hpp"

// PRIVATE

void MainWindow::make_scene_0(SimulationRenderer *sr) {
	// these parts of the initialiser are scene-dependent
	initialiser& init = sr->get_simulator().get_initialiser();
	init.set_pos_initialiser(
		[this](particle *p) {
			p->set_previous_position(vec3(0.0f,0.0f,0.0f));
			float x = this->U010(this->eng) - 4.5f;
			float z = this->U010(this->eng) - 4.5f;
			p->set_position(vec3(x,10.0f,z));
		}
	);
	init.set_vel_initialiser(
		[](particle *p) {
			p->set_previous_velocity(vec3(0.0f,0.0f,0.0f));
			p->set_velocity(vec3(0.0f,0.0f,0.0f));
		}
	);
	init.set_starttime_initialiser(
		[this](particle *p) {
			float s = this->U01(this->eng);
			p->set_starttime( 2.0f*s );
		}
	);

	// the friction,bouncing coefficient, and lifetime
	// are parameter-dependent.
	make_init_with_params(init);

	rplane *floor = new rplane();
	floor->p1 = glm::vec3(-5.0f, 0.0f, -5.0f);
	floor->p2 = glm::vec3(-5.0f, 0.0f,  5.0f);
	floor->p3 = glm::vec3( 5.0f, 0.0f,  5.0f);
	floor->p4 = glm::vec3( 5.0f, 0.0f, -5.0f);
	floor->pl = new plane(vec3(0.0f,1.0f,0.0f), floor->p1);

	sr->add_rgeom(floor);

	sr->add_particles(25);
}
