#include "mainwindow.hpp"

// C includes
#include <assert.h>

// glm includes
#include <glm/glm.hpp>

// base includes
#include <base/geometry/rrectangle.hpp>
#include <base/geometry/rtriangle.hpp>
#include <base/geometry/rsphere.hpp>
#include <base/geometry/rplane.hpp>

// physim includes
#include <physim/initialiser/initialiser.hpp>
#include <physim/initialiser/multisource.hpp>
#include <physim/initialiser/hose.hpp>
#include <physim/geometry/triangle.hpp>
#include <physim/geometry/sphere.hpp>
#include <physim/geometry/plane.hpp>
#include <physim/math/vec3.hpp>
using namespace physim;
using namespace particles;
using namespace init;
using namespace geom;

// custom includes
#include "conversion_helper.hpp"

// PRIVATE

void MainWindow::make_sim3(SimulationRenderer *sr) {

	glm::vec3 A(-3.0f, 0.0f,-3.0f);
	glm::vec3 D(-5.0f,-0.5f,-5.0f);
	glm::vec3 E(-5.0f,-0.5f, 5.0f);
	glm::vec3 F( 5.0f,-0.5f,-5.0f);
	glm::vec3 G( 5.0f,-0.5f, 5.0f);
	glm::vec3 H(-5.0f, 5.0f,-5.0f);
	glm::vec3 I(-5.0f, 5.0f, 5.0f);
	glm::vec3 J( 5.0f, 5.0f,-5.0f);
	glm::vec3 K( 5.0f, 5.0f, 5.0f);
	glm::vec3 L( 3.0f, 5.0f, 3.0f);
	glm::vec3 M( 0.0f, 2.0f, 0.0f);
	//glm::vec3 N( 0.0f, 3.0f, 0.0f);
	glm::vec3 O( 0.5f, 3.0f,-1.0f);
	glm::vec3 P(-1.0f, 3.0f, 0.5f);
	glm::vec3 S(-1.5f, 5.0f,-1.5f);

	glm::vec3 scene_top_center = (H + I + J + K)/4.0f;

	glm::vec3 dir1 = glm::normalize(scene_top_center - H);
	glm::vec3 dir2 = glm::normalize(scene_top_center - I);
	glm::vec3 dir3 = glm::normalize(scene_top_center - J);
	glm::vec3 dir4 = glm::normalize(scene_top_center - K);

	glm::vec3 source1 = H + dir1*0.25f;
	glm::vec3 source2 = I + dir2*0.25f;
	glm::vec3 source3 = J + dir3*0.25f;
	glm::vec3 source4 = K + dir4*0.25f;

	multisource<hose> ms;
	ms.allocate(16000, 4);

	std::vector<hose>& hoses = ms.get_sources();
	hoses[0].set_hose_source(to_physim(source1), to_physim(dir1), 1.0f, 16.0f);
	hoses[1].set_hose_source(to_physim(source2), to_physim(dir2), 8.0f, 12.0f);
	hoses[2].set_hose_source(to_physim(source3), to_physim(dir3), 12.0f, 8.0f);
	hoses[3].set_hose_source(to_physim(source4), to_physim(dir4), 16.0f, 1.0f);

	ms.make_position_init();
	ms.use_position_init();
	ms.make_velocity_init();
	ms.use_velocity_init();
	ms.set_starttime_initialiser(
		[](particles::free_particle *p) {
			p->starttime = p->index/4000.0f;
		}
	);

	make_init_with_params(ms);
	sr->get_simulator().set_initialiser(&ms);

	rplane *floor = new rplane();
	floor->set_points(E, G, F, D);

	rplane *wall1 = new rplane();
	wall1->set_points(K, I, E, G);
	wall1->set_color(1.0f,1.0f,0.0f,1.0f);
	rplane *wall2 = new rplane();
	wall2->set_points(K, J, F, G);
	wall2->set_color(1.0f,0.0f,1.0f,1.0f);
	rplane *wall3 = new rplane();
	wall3->set_points(J, H, D, F);
	wall3->set_color(0.0f,1.0f,1.0f,1.0f);
	rplane *wall4 = new rplane();
	wall4->set_points(H, I, E, D);
	wall4->set_color(0.0f,0.0f,1.0f,1.0f);

	sim_ball->compile();

	rsphere *ball1 = new rsphere();
	ball1->set_center(M);
	ball1->set_radius(1.0f);
	ball1->set_model(sim_ball);

	rsphere *ball2 = new rsphere();
	ball2->set_center(A);
	ball2->set_radius(1.0f);
	ball2->set_model(sim_ball);

	rtriangle *ramp = new rtriangle();
	ramp->set_points(S, O, P);
	ramp->set_color(0.0f,1.0f,0.0f,1.0f);

	sr->add_rgeom(floor);
	sr->add_rgeom(wall1);
	sr->add_rgeom(wall2);
	sr->add_rgeom(wall3);
	sr->add_rgeom(wall4);
	sr->add_rgeom(ball1);
	sr->add_rgeom(ball2);
	sr->add_rgeom(ramp);

	plane *floor_pl = new plane(math::vec3( 0.0f,1.0f, 0.0f), to_physim(floor->p1()));
	plane *wall1_pl = new plane(math::vec3( 0.0f,0.0f,-1.0f), to_physim(wall1->p1()));
	plane *wall2_pl = new plane(math::vec3(-1.0f,0.0f, 0.0f), to_physim(wall2->p1()));
	plane *wall3_pl = new plane(math::vec3( 0.0f,0.0f, 1.0f), to_physim(wall3->p1()));
	plane *wall4_pl = new plane(math::vec3( 1.0f,0.0f, 0.0f), to_physim(wall4->p1()));
	sphere *ball1_pl = new sphere(to_physim(ball1->center()), ball1->radius());
	sphere *ball2_pl = new sphere(to_physim(ball2->center()), ball2->radius());
	triangle *tri_ramp = new triangle(to_physim(S),to_physim(O),to_physim(P));

	sr->get_simulator().add_geometry(floor_pl);
	sr->get_simulator().add_geometry(wall1_pl);
	sr->get_simulator().add_geometry(wall2_pl);
	sr->get_simulator().add_geometry(wall3_pl);
	sr->get_simulator().add_geometry(wall4_pl);
	sr->get_simulator().add_geometry(ball1_pl);
	sr->get_simulator().add_geometry(ball2_pl);
	sr->get_simulator().add_geometry(tri_ramp);

	sr->get_simulator().add_gravity_acceleration(math::vec3(0.0f,-9.81f,0.0f));

	sr->add_particles(16000);
}


