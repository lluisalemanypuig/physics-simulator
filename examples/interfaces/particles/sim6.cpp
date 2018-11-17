#include "mainwindow.hpp"

// base includes
#include <base/render_geometry/rplane.hpp>

// physim includes
#include <physim/initialiser/initialiser.hpp>
#include <physim/initialiser/rect_shower.hpp>
#include <physim/geometry/plane.hpp>
#include <physim/math/math.hpp>
using namespace physim;
using namespace particles;
using namespace math;
using namespace init;
using namespace geom;

// PRIVATE

void MainWindow::make_sim5(SimulationRenderer *sr) {

	initialiser *I = sr->get_simulator().get_initialiser();
	I->set_pos_initialiser(
		[](free_particle *p) {
			float z = -4.0f + p->index*0.8f;
			p->cur_pos = vec3(-5.0f,1.0f,z);
		}
	);
	I->set_vel_initialiser(
		[](free_particle *p) {
			p->cur_vel = vec3(5.0f,0.0f,0.0f);
		}
	);
	I->set_bounce_initialiser(
		[](free_particle *p) {
			p->bouncing = p->index/10.0f;
		}
	);

	vec3 A( -5.0f, -0.25f,-5.0f);
	vec3 B( -5.0f, -0.25f, 5.0f);
	vec3 C( 20.0f, -0.25f, 5.0f);
	vec3 D( 20.0f, -0.25f,-5.0f);

	rplane *floor = new rplane();
	floor->set_points(A, B, C, D);
	sr->add_rgeom(floor);

	plane *pl = new plane(
		vec3(0.0f,1.0f,0.0f),
		vec3(0.0f,0.0f,0.0f)
	);
	sr->get_simulator().add_geometry(pl);

	sr->get_simulator().add_gravity_acceleration(vec3(0.0f,-9.81f,0.0f));

	sr->add_particles(11);
}


