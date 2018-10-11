#include "mainwindow.hpp"

// PRIVATE

void MainWindow::make_sim3(SimulationRenderer *sr) {

	const glm::vec3 A(-3.0f, 0.0f,-3.0f);
	const glm::vec3 D(-5.0f,-0.5f,-5.0f);
	const glm::vec3 E(-5.0f,-0.5f, 5.0f);
	const glm::vec3 F( 5.0f,-0.5f,-5.0f);
	const glm::vec3 G( 5.0f,-0.5f, 5.0f);
	const glm::vec3 H(-5.0f, 5.0f,-5.0f);
	const glm::vec3 I(-5.0f, 5.0f, 5.0f);
	const glm::vec3 J( 5.0f, 5.0f,-5.0f);
	const glm::vec3 K( 5.0f, 5.0f, 5.0f);
	const glm::vec3 L( 3.0f, 5.0f, 3.0f);
	const glm::vec3 M( 0.0f, 2.0f, 0.0f);
	//const glm::vec3 N( 0.0f, 3.0f, 0.0f);
	const glm::vec3 O( 0.5f, 3.0f,-1.0f);
	const glm::vec3 P(-1.0f, 3.0f, 0.5f);
	const glm::vec3 S(-1.5f, 5.0f,-1.5f);

	const glm::vec3 hK = K + vec3(0.0f,0.5f,0.0f);
	const glm::vec3 hL = L + vec3(0.0f,0.5f,0.0f);

	hose h;
	h.set_hose_source(hK, glm::normalize(hL-hK), 0.5f, glm::length(hL-hK));
	h.set_starttime_initialiser(
		[](particle *p) {
			p->set_starttime( p->get_index()/1000.0f );
		}
	);

	make_init_with_params(h);
	sr->get_simulator().set_initialiser(&h);

	rrectangle *floor = new rrectangle();
	floor->p1 = E;
	floor->p2 = G;
	floor->p3 = F;
	floor->p4 = D;
	floor->rl = new rectangle(floor->p1,floor->p2,floor->p3, D);
	sr->add_rgeom(floor);

	rrectangle *wall1 = new rrectangle();
	wall1->p1 = K;
	wall1->p2 = I;
	wall1->p3 = E;
	wall1->p4 = G;
	wall1->rl = new rectangle(wall1->p1,wall1->p2,wall1->p3,wall1->p4);
	wall1->set_color(QVector4D(1.0f,1.0f,0.0f,1.0f));
	rrectangle *wall2 = new rrectangle();
	wall2->p1 = K;
	wall2->p2 = J;
	wall2->p3 = F;
	wall2->p4 = G;
	wall2->rl = new rectangle(wall2->p1,wall2->p2,wall2->p3,wall2->p4);
	wall2->set_color(QVector4D(1.0f,0.0f,1.0f,1.0f));
	rrectangle *wall3 = new rrectangle();
	wall3->p1 = J;
	wall3->p2 = H;
	wall3->p3 = D;
	wall3->p4 = F;
	wall3->rl = new rectangle(wall3->p1,wall3->p2,wall3->p3,wall3->p4);
	wall3->set_color(QVector4D(0.0f,1.0f,1.0f,1.0f));
	rrectangle *wall4 = new rrectangle();
	wall4->p1 = H;
	wall4->p2 = I;
	wall4->p3 = E;
	wall4->p4 = D;
	wall4->rl = new rectangle(wall4->p1,wall4->p2,wall4->p3,wall4->p4);
	wall4->set_color(QVector4D(0.0f,0.0f,1.0f,1.0f));
	sr->add_rgeom(wall1);
	sr->add_rgeom(wall2);
	sr->add_rgeom(wall3);
	sr->add_rgeom(wall4);

	rsphere *ball1 = new rsphere();
	ball1->c = M;
	ball1->r = 1.0f;
	ball1->s = new sphere(ball1->c, ball1->r);
	ball1->set_color(QVector4D(0.0f,0.0f,1.0f,1.0f));
	rsphere *ball2 = new rsphere();
	ball2->c = A;
	ball2->r = 1.0f;
	ball2->s = new sphere(ball2->c, ball2->r);
	ball2->set_color(QVector4D(0.0f,0.0f,1.0f,1.0f));
	sr->add_rgeom(ball1);
	sr->add_rgeom(ball2);

	rtriangle *tri = new rtriangle();
	tri->p1 = S;
	tri->p2 = O;
	tri->p3 = P;
	tri->tl = new triangle(S,O,P);
	tri->tl->set_position( tri->tl->get_plane().get_normal()*0.02f );
	tri->set_color(QVector4D(0.0f,0.3f,0.0f,1.0f));
	sr->add_rgeom(tri);

	sr->add_particles(3000);
}


