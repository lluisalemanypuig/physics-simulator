#include "mainwindow.hpp"

// PRIVATE

void MainWindow::make_scene_0(SimulationRenderer *sr) {
	rplane *floor = new rplane();
	floor->p1 = glm::vec3(-5.0f, 0.0f, -5.0f);
	floor->p2 = glm::vec3(-5.0f, 0.0f,  5.0f);
	floor->p3 = glm::vec3( 5.0f, 0.0f,  5.0f);
	floor->p4 = glm::vec3( 5.0f, 0.0f, -5.0f);
	floor->pl = new plane(vec3(0.0f,1.0f,0.0f), floor->p1);

	sr->add_rgeom(floor);
}
