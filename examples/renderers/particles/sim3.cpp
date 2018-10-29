#include "mainwindow.hpp"

// physim includes
#include <physim/initialiser/initialiser.hpp>
#include <physim/initialiser/rect_shower.hpp>
#include <physim/geometry/triangle.hpp>
#include <physim/geometry/rectangle.hpp>
#include <physim/geometry/sphere.hpp>
#include <physim/geometry/plane.hpp>
#include <physim/math/math.hpp>
using namespace physim;
using namespace math;
using namespace init;
using namespace geom;

// PRIVATE

void MainWindow::make_sim2(SimulationRenderer *sr) {

	initialiser i;
	i.set_pos_initialiser(
		[&](particle *p) {
			p->set_previous_position(vec3(0.0f,0.0f,0.0f));

			size_t idx = p->get_index();
			float iz = 0.0;
			if (idx == 0)		{ iz = -1.00f; }
			else if (idx == 1)	{ iz = -0.75f; }
			else if (idx == 2)	{ iz = -0.50f; }
			else if (idx == 3)	{ iz = -0.25f; }
			else if (idx == 4)	{ iz =  0.00f; }
			else if (idx == 5)	{ iz =  0.25f; }
			else if (idx == 6)	{ iz =  0.50f; }
			else if (idx == 7)	{ iz =  0.75f; }
			else if (idx == 8)	{ iz =  1.00f; }
			p->set_position(-2.0f,4.5f,iz);
		}
	);
	i.set_vel_initialiser(
		[](particle *p) {
			p->set_velocity(0.0f,0.0f,0.0f);
		}
	);

	make_init_with_params(i);
	sr->get_simulator().set_initialiser(&i);

	rplane *floor = new rplane();
	floor->p1 = vec3(-5.0f, -0.05f, -5.0f);
	floor->p2 = vec3(-5.0f, -0.05f,  5.0f);
	floor->p3 = vec3( 5.0f, -0.05f,  5.0f);
	floor->p4 = vec3( 5.0f, -0.05f, -5.0f);

	rsphere *ball = new rsphere();
	ball->c = vec3(0.0f,2.0f,0.0f);
	ball->r = 1.0f;

	rrectangle *ramp = new rrectangle();
	ramp->p1 = vec3( 0.0f, 2.0f,  1.0f);
	ramp->p2 = vec3( 0.0f, 2.0f, -1.0f);
	ramp->p3 = vec3(-3.0f, 5.0f, -1.0f);
	ramp->p4 = vec3(-3.0f, 5.0f,  1.0f);
	ramp->set_color(0.0f,0.3f,0.0f,1.0f);

	sr->add_rgeom(floor);
	sr->add_rgeom(ball);
	sr->add_rgeom(ramp);

	plane *pl = new plane(
		vec3(0.0f,1.0f,0.0f),
		vec3(0.0f,0.0f,0.0f)
	);
	sphere *s = new sphere(ball->c, 1.0f);
	rectangle *rl = new rectangle(ramp->p1,ramp->p2,ramp->p3,ramp->p4);

	sr->get_simulator().add_geometry(pl);
	sr->get_simulator().add_geometry(s);
	sr->get_simulator().add_geometry(rl);

	sr->get_simulator().set_gravity_acceleration(vec3(0.0f,-9.81f,0.0f));

	sr->add_particles(9);
}

