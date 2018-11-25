#include "mainwindow.hpp"

// glm includes
#include <glm/vec3.hpp>

// render includes
#include <render/geometry/rrectangle.hpp>
#include <render/geometry/rtriangle.hpp>
#include <render/geometry/rplane.hpp>

// physim includes
#include <physim/initialiser/initialiser.hpp>
#include <physim/initialiser/rect_shower.hpp>
#include <physim/geometry/triangle.hpp>
#include <physim/geometry/rectangle.hpp>
#include <physim/geometry/plane.hpp>
#include <physim/math/math.hpp>
using namespace physim;
using namespace particles;
using namespace init;
using namespace geom;

// custom includes
#include "conversion_helper.hpp"

// PRIVATE

void MainWindow::make_sim1(SimulationRenderer *sr) {
	// these parts of the initialiser are scene-dependent

	rect_shower w;
	w.set_straight_source(math::vec3(-4.5f,10.0f,-1.5f), 3.0f,3.0f);
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
		glm::vec3(-2.0f, 2.0f,  1.0f), glm::vec3(-2.0f, 2.0f, -1.0f),
		glm::vec3(-4.0f, 4.0f, -1.0f), glm::vec3(-4.0f, 4.0f,  1.0f)
	);
	ramp->set_color(1.0f, 1.0f, 0.0f, 1.0f);

	rtriangle *bouncer = new rtriangle();
	bouncer->set_points(
		glm::vec3( 0.0f, 2.0f,  1.0f), glm::vec3( 0.0f, 2.0f, -1.0f),
		glm::vec3( 2.0f, 4.0f,  0.0f)
	);
	bouncer->set_color(1.0f, 0.0f, 1.0f, 1.0f);

	rplane *floor = new rplane();
	floor->set_points(
		glm::vec3(-5.0f, -0.05f, -5.0f), glm::vec3(-5.0f, -0.05f,  5.0f),
		glm::vec3( 5.0f, -0.05f,  5.0f), glm::vec3( 5.0f, -0.05f, -5.0f)
	);

	sr->add_rgeom(ramp);
	sr->add_rgeom(bouncer);
	sr->add_rgeom(floor);

	rectangle *rl = new rectangle(
		to_physim(ramp->p1()), to_physim(ramp->p2()),
		to_physim(ramp->p3()), to_physim(ramp->p4())
	);
	triangle *tl = new triangle(
		to_physim(bouncer->p1()), to_physim(bouncer->p2()),
		to_physim(bouncer->p3())
	);
	plane *pl = new plane(
		math::vec3(0.0f,1.0f,0.0f),
		math::vec3(0.0f,0.0f,0.0f)
	);

	sr->get_simulator().add_geometry(rl);
	sr->get_simulator().add_geometry(tl);
	sr->get_simulator().add_geometry(pl);

	sr->get_simulator().add_gravity_acceleration(math::vec3(0.0f,-9.81f,0.0f));

	sr->add_free_particles(300);
}
