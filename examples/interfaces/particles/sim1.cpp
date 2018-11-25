#include "mainwindow.hpp"

// glm includes
#include <glm/vec3.hpp>

// render includes
#include <render/geometry/rplane.hpp>

// physim includes
#include <physim/initialiser/initialiser.hpp>
#include <physim/initialiser/rect_shower.hpp>
#include <physim/fields/gravitational.hpp>
#include <physim/geometry/plane.hpp>
#include <physim/math/math.hpp>
using namespace physim;
using namespace particles;
using namespace fields;
using namespace init;
using namespace geom;

// PRIVATE

void MainWindow::make_sim0(SimulationRenderer *sr) {
	// these parts of the initialiser are scene-dependent

	rect_shower w;
	w.set_straight_source(math::vec3(-3.0f,10.0f,-3.0f), 6.0f,6.0f);
	w.set_starttime_initialiser(
		[](free_particle *p) {
			p->starttime = p->index/1000.0f;
		}
	);
	w.set_charge_initialiser(
		[](free_particle *p) { p->charge = 1.0f; }
	);
	w.set_mass_initialiser(
		[](free_particle *p) { p->mass = 1.0f; }
	);

	// the friction,bouncing coefficient, and lifetime
	// are parameter-dependent.
	make_init_with_params(w);

	sr->get_simulator().set_initialiser(&w);

	rplane *floor = new rplane();
	floor->set_points(
		glm::vec3(-5.0f, -0.05f, -5.0f), glm::vec3(-5.0f, -0.05f,  5.0f),
		glm::vec3( 5.0f, -0.05f,  5.0f), glm::vec3( 5.0f, -0.05f, -5.0f)
	);
	sr->add_rgeom(floor);

	plane *pl = new plane(
		math::vec3(0.0f,1.0f,0.0f),
		math::vec3(0.0f,0.0f,0.0f)
	);
	sr->get_simulator().add_geometry(pl);

	sr->get_simulator().add_gravity_acceleration(math::vec3(0.0f,-9.81f,0.0f));

	sr->add_free_particles(1000);
}
