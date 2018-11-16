#include "mainwindow.hpp"

// C includes
#include <assert.h>

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
using namespace math;
using namespace init;
using namespace geom;

// PRIVATE

void MainWindow::make_sim3(SimulationRenderer *sr) {

	vec3 A(-3.0f, 0.0f,-3.0f);
	vec3 D(-5.0f,-0.5f,-5.0f);
	vec3 E(-5.0f,-0.5f, 5.0f);
	vec3 F( 5.0f,-0.5f,-5.0f);
	vec3 G( 5.0f,-0.5f, 5.0f);
	vec3 H(-5.0f, 5.0f,-5.0f);
	vec3 I(-5.0f, 5.0f, 5.0f);
	vec3 J( 5.0f, 5.0f,-5.0f);
	vec3 K( 5.0f, 5.0f, 5.0f);
	vec3 L( 3.0f, 5.0f, 3.0f);
	vec3 M( 0.0f, 2.0f, 0.0f);
	//vec3 N( 0.0f, 3.0f, 0.0f);
	vec3 O( 0.5f, 3.0f,-1.0f);
	vec3 P(-1.0f, 3.0f, 0.5f);
	vec3 S(-1.5f, 5.0f,-1.5f);

	vec3 scene_top_center = (H + I + J + K)/4.0f;

	vec3 dir1 = normalise(scene_top_center - H);
	vec3 dir2 = normalise(scene_top_center - I);
	vec3 dir3 = normalise(scene_top_center - J);
	vec3 dir4 = normalise(scene_top_center - K);

	vec3 source1 = H + dir1*0.25f;
	vec3 source2 = I + dir2*0.25f;
	vec3 source3 = J + dir3*0.25f;
	vec3 source4 = K + dir4*0.25f;

	multisource<hose> ms;
	ms.allocate(16000, 4);

	std::vector<hose>& hoses = ms.get_sources();
	hoses[0].set_hose_source(source1, dir1, 1.0f, 16.0f);
	hoses[1].set_hose_source(source2, dir2, 8.0f, 12.0f);
	hoses[2].set_hose_source(source3, dir3, 12.0f, 8.0f);
	hoses[3].set_hose_source(source4, dir4, 16.0f, 1.0f);

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
	floor->p1 = E;
	floor->p2 = G;
	floor->p3 = F;
	floor->p4 = D;

	rplane *wall1 = new rplane();
	wall1->p1 = K;
	wall1->p2 = I;
	wall1->p3 = E;
	wall1->p4 = G;
	wall1->set_color(1.0f,1.0f,0.0f,1.0f);
	rplane *wall2 = new rplane();
	wall2->p1 = K;
	wall2->p2 = J;
	wall2->p3 = F;
	wall2->p4 = G;
	wall2->set_color(1.0f,0.0f,1.0f,1.0f);
	rplane *wall3 = new rplane();
	wall3->p1 = J;
	wall3->p2 = H;
	wall3->p3 = D;
	wall3->p4 = F;
	wall3->set_color(0.0f,1.0f,1.0f,1.0f);
	rplane *wall4 = new rplane();
	wall4->p1 = H;
	wall4->p2 = I;
	wall4->p3 = E;
	wall4->p4 = D;
	wall4->set_color(0.0f,0.0f,1.0f,1.0f);

	sim_ball->compile();

	rsphere *ball1 = new rsphere();
	ball1->c = M;
	ball1->r = 1.0f;
	ball1->set_model(sim_ball);

	rsphere *ball2 = new rsphere();
	ball2->c = A;
	ball2->r = 1.0f;
	ball2->set_model(sim_ball);

	rtriangle *ramp = new rtriangle();
	ramp->p1 = S;
	ramp->p2 = O;
	ramp->p3 = P;
	ramp->set_color(0.0f,1.0f,0.0f,1.0f);

	sr->add_rgeom(floor);
	sr->add_rgeom(wall1);
	sr->add_rgeom(wall2);
	sr->add_rgeom(wall3);
	sr->add_rgeom(wall4);
	sr->add_rgeom(ball1);
	sr->add_rgeom(ball2);
	sr->add_rgeom(ramp);

	plane *floor_pl = new plane(vec3( 0.0f,1.0f, 0.0f), floor->p1);
	plane *wall1_pl = new plane(vec3( 0.0f,0.0f,-1.0f), wall1->p1);
	plane *wall2_pl = new plane(vec3(-1.0f,0.0f, 0.0f), wall2->p1);
	plane *wall3_pl = new plane(vec3( 0.0f,0.0f, 1.0f), wall3->p1);
	plane *wall4_pl = new plane(vec3( 1.0f,0.0f, 0.0f), wall4->p1);
	sphere *ball1_pl = new sphere(ball1->c, ball1->r);
	sphere *ball2_pl = new sphere(ball2->c, ball2->r);
	triangle *tri_ramp = new triangle(S,O,P);

	sr->get_simulator().add_geometry(floor_pl);
	sr->get_simulator().add_geometry(wall1_pl);
	sr->get_simulator().add_geometry(wall2_pl);
	sr->get_simulator().add_geometry(wall3_pl);
	sr->get_simulator().add_geometry(wall4_pl);
	sr->get_simulator().add_geometry(ball1_pl);
	sr->get_simulator().add_geometry(ball2_pl);
	sr->get_simulator().add_geometry(tri_ramp);

	sr->get_simulator().add_gravity_acceleration(vec3(0.0f,-9.81f,0.0f));

	sr->add_particles(16000);
}


