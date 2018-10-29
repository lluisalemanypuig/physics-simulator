#include "mainwindow.hpp"

// physim includes
#include <physim/initialiser/initialiser.hpp>
#include <physim/initialiser/rect_shower.hpp>
#include <physim/geometry/plane.hpp>
#include <physim/math/math.hpp>
using namespace physim;
using namespace math;
using namespace init;
using namespace geom;

// PRIVATE

void MainWindow::make_sim0(SimulationRenderer *sr) {
	// these parts of the initialiser are scene-dependent

	rect_shower w;
	w.set_straight_source(math::vec3(-3.0f,10.0f,-3.0f), 6.0f,6.0f);
	w.set_starttime_initialiser(
		[](particle *p) {
			p->set_starttime( p->get_index()/1000.0f );
		}
	);

	// the friction,bouncing coefficient, and lifetime
	// are parameter-dependent.
	make_init_with_params(w);

	sr->get_simulator().set_initialiser(&w);

	rplane *floor = new rplane();
	floor->p1 = physim::math::vec3(-5.0f, -0.05f, -5.0f);
	floor->p2 = physim::math::vec3(-5.0f, -0.05f,  5.0f);
	floor->p3 = physim::math::vec3( 5.0f, -0.05f,  5.0f);
	floor->p4 = physim::math::vec3( 5.0f, -0.05f, -5.0f);
	sr->add_rgeom(floor);

	physim::geom::plane *pl = new physim::geom::plane(
		physim::math::vec3(0.0f,1.0f,0.0f),
		physim::math::vec3(0.0f,0.0f,0.0f)
	);
	sr->get_simulator().add_geometry(pl);

	sr->add_particles(1000);
}
