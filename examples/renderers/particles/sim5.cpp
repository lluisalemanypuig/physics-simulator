#include "mainwindow.hpp"

// physim includes
#include <physim/initialiser/initialiser.hpp>
#include <physim/initialiser/rect_shower.hpp>
#include <physim/math/math.hpp>
using namespace physim;
using namespace math;
using namespace init;
using namespace geom;

// PRIVATE

void MainWindow::make_sim4(SimulationRenderer *sr) {

	initialiser *I = sr->get_simulator().get_initialiser();
	I->set_pos_initialiser(
		[](particle *p) {
			float z = -4.0f + p->get_index()*0.8f;
			p->set_position(-5.0f,1.0f,z);
		}
	);
	I->set_vel_initialiser(
		[](particle *p) {
			p->set_velocity(5.0f,0.0f,0.0f);
		}
	);
	I->set_friction_initialiser(
		[](particle *p) {
			p->set_friction( p->get_index()/10.0f );
		}
	);

	vec3 A( -5.0f, -0.25f,-5.0f);
	vec3 B( -5.0f, -0.25f, 5.0f);
	vec3 C( 20.0f, -0.25f, 5.0f);
	vec3 D( 20.0f, -0.25f,-5.0f);

	rplane *floor = new rplane();
	floor->p1 = A;
	floor->p2 = B;
	floor->p3 = C;
	floor->p4 = D;
	sr->add_rgeom(floor);

	plane *pl = new plane(
		vec3(0.0f,1.0f,0.0f),
		vec3(0.0f,0.0f,0.0f)
	);
	sr->get_simulator().add_geometry(pl);

	sr->add_particles(11);
}


