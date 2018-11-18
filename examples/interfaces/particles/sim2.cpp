#include "mainwindow.hpp"

// base includes
#include <base/geometry/rrectangle.hpp>
#include <base/geometry/rtriangle.hpp>
#include <base/geometry/rplane.hpp>

// physim includes
#include <physim/initialiser/initialiser.hpp>
#include <physim/initialiser/rect_shower.hpp>
#include <physim/geometry/triangle.hpp>
#include <physim/geometry/rectangle.hpp>
#include <physim/geometry/plane.hpp>
#include <physim/math/math.hpp>
using namespace physim;
using namespace particles;
using namespace math;
using namespace init;
using namespace geom;

// PRIVATE

void MainWindow::make_sim1(SimulationRenderer *sr) {
	// these parts of the initialiser are scene-dependent

	rect_shower w;
	w.set_straight_source(vec3(-4.5f,10.0f,-1.5f), 3.0f,3.0f);
	w.set_starttime_initialiser(
		[](free_particle *p) {
			p->starttime = p->index/300.0f;
		}
	);

	// the friction,bouncing coefficient, and lifetime
	// are parameter-dependent.
	make_init_with_params(w);

	sr->get_simulator().set_initialiser(&w);

	rrectangle *ramp = new rrectangle();
	ramp->set_points(
		vec3(-2.0f, 2.0f,  1.0f), vec3(-2.0f, 2.0f, -1.0f),
		vec3(-4.0f, 4.0f, -1.0f), vec3(-4.0f, 4.0f,  1.0f)
	);
	ramp->set_color(1.0f, 1.0f, 0.0f, 1.0f);

	rtriangle *bouncer = new rtriangle();
	bouncer->set_points(
		vec3( 0.0f, 2.0f,  1.0f), vec3( 0.0f, 2.0f, -1.0f),
		vec3( 2.0f, 4.0f,  0.0f)
	);
	bouncer->set_color(1.0f, 0.0f, 1.0f, 1.0f);

	rplane *floor = new rplane();
	floor->set_points(
		vec3(-5.0f, -0.05f, -5.0f), vec3(-5.0f, -0.05f,  5.0f),
		vec3( 5.0f, -0.05f,  5.0f), vec3( 5.0f, -0.05f, -5.0f)
	);

	sr->add_rgeom(ramp);
	sr->add_rgeom(bouncer);
	sr->add_rgeom(floor);

	rectangle *rl = new rectangle(ramp->p1(),ramp->p2(),ramp->p3(),ramp->p4());
	triangle *tl = new triangle(bouncer->p1(),bouncer->p2(),bouncer->p3());
	plane *pl = new plane(
		vec3(0.0f,1.0f,0.0f),
		vec3(0.0f,0.0f,0.0f)
	);

	sr->get_simulator().add_geometry(rl);
	sr->get_simulator().add_geometry(tl);
	sr->get_simulator().add_geometry(pl);

	sr->get_simulator().add_gravity_acceleration(vec3(0.0f,-9.81f,0.0f));

	sr->add_particles(300);
}
