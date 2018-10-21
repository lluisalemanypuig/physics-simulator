#pragma once

// C includes
#include <string.h>

// C++ includes
#include <iostream>
#include <fstream>
using namespace std;

// glm includes
#include <glm/glm.hpp>
using namespace glm;

// simulator includes
#include <physim/initialiser/initialiser.hpp>
#include <physim/simulator.hpp>
#include <physim/particle.hpp>
using namespace physim;
using namespace init;

// Custom includes
#include "utils.hpp"

namespace study_cases {

	void list_all_cases();
	void choose_case(int argc, char *argv[]);

	/* -- Floor (1) study case --
	 * Simply a particle bouncing vertically
	 * on a plane (the floor).
	 */
	void bounce_on_floor(int argc, char *argv[]);

	/* -- Floor (2) study case --
	 * Simply a particle bouncing vertically
	 * on a plane (the floor).
	 */
	void roll_on_floor(int argc, char *argv[]);

	/* -- Sphere study case --
	 * A aprticle falling onto a sphere below its
	 * initial position.
	 */
	void bounce_on_sphere(int argc, char *argv[]);

	/* -- Triangle-triangle-floor case --
	 * This scene contains two triangles and a floor.
	 * The trangles are almost facing each other.
	 */
	void tri_tri_floor(int argc, char *argv[]);

	/* -- Roll-plane-sphere case --
	 * This scene contains a plane and a sphere.
	 * The particle starts rolling down the plane
	 * then collides with a sphere at the bottom of
	 * the plane.
	 */
	void roll_plane_sphere(int argc, char *argv[]);

} // -- namespace study_cases
