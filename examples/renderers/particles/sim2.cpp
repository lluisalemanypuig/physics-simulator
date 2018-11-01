#include "mainwindow.hpp"

// physim includes
#include <physim/initialiser/initialiser.hpp>
#include <physim/initialiser/rect_shower.hpp>
#include <physim/geometry/triangle.hpp>
#include <physim/geometry/rectangle.hpp>
#include <physim/geometry/plane.hpp>
#include <physim/math/math.hpp>
using namespace physim;
using namespace math;
using namespace init;
using namespace geom;

// PRIVATE

void MainWindow::make_sim1(SimulationRenderer *sr) {
	// these parts of the initialiser are scene-dependent

	rect_shower w;
	w.set_straight_source(physim::math::vec3(-4.5f,10.0f,-1.5f), 3.0f,3.0f);
	w.set_starttime_initialiser(
		[](particle *p) {
			p->set_starttime( p->get_index()/300.0f );
		}
	);

	// the friction,bouncing coefficient, and lifetime
	// are parameter-dependent.
	make_init_with_params(w);

	sr->get_simulator().set_initialiser(&w);

	rrectangle *ramp = new rrectangle();
	ramp->p1 = physim::math::vec3(-2.0f, 2.0f,  1.0f);
	ramp->p2 = physim::math::vec3(-2.0f, 2.0f, -1.0f);
	ramp->p3 = physim::math::vec3(-4.0f, 4.0f, -1.0f);
	ramp->p4 = physim::math::vec3(-4.0f, 4.0f,  1.0f);
	ramp->set_color(1.0f, 1.0f, 0.0f, 1.0f);

	rtriangle *bouncer = new rtriangle();
	bouncer->p1 = physim::math::vec3( 0.0f, 2.0f,  1.0f);
	bouncer->p2 = physim::math::vec3( 0.0f, 2.0f, -1.0f);
	bouncer->p3 = physim::math::vec3( 2.0f, 4.0f,  0.0f);
	bouncer->set_color(1.0f, 0.0f, 1.0f, 1.0f);

	rplane *floor = new rplane();
	floor->p1 = physim::math::vec3(-5.0f, -0.05f, -5.0f);
	floor->p2 = physim::math::vec3(-5.0f, -0.05f,  5.0f);
	floor->p3 = physim::math::vec3( 5.0f, -0.05f,  5.0f);
	floor->p4 = physim::math::vec3( 5.0f, -0.05f, -5.0f);

	sr->add_rgeom(ramp);
	sr->add_rgeom(bouncer);
	sr->add_rgeom(floor);

	physim::geom::rectangle *rl = new physim::geom::rectangle(ramp->p1,ramp->p2,ramp->p3,ramp->p4);
	physim::geom::triangle *tl = new physim::geom::triangle(bouncer->p1,bouncer->p2,bouncer->p3);
	physim::geom::plane *pl = new physim::geom::plane(
		physim::math::vec3(0.0f,1.0f,0.0f),
		physim::math::vec3(0.0f,0.0f,0.0f)
	);

	sr->get_simulator().add_geometry(rl);
	sr->get_simulator().add_geometry(tl);
	sr->get_simulator().add_geometry(pl);

	sr->get_simulator().add_gravity_acceleration(vec3(0.0f,-9.81f,0.0f));

	sr->add_particles(300);
}
