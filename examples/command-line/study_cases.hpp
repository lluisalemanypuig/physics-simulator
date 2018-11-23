#pragma once

// C includes
#include <string.h>

// C++ includes
#include <iostream>
#include <fstream>
using namespace std;

// Custom includes
#include "utils.hpp"

namespace study_cases {

	void choose_case(int argc, char *argv[]);

	/* -- Floor (1) study case --
	 * Simply a particle bouncing vertically
	 * on a plane (the floor).
	 */
	void bounce_on_floor(int argc, char *argv[]);
	/* -- Floor (1') study case --
	 * Simply a sized particle bouncing vertically
	 * on a plane (the floor).
	 */
	void bounce_on_floor_sized(int argc, char *argv[]);

	/* -- Floor (2) study case --
	 * Simply a particle bouncing vertically
	 * on a plane (the floor).
	 */
	void roll_on_floor(int argc, char *argv[]);
	/* -- Floor (2') study case --
	 * Simply a particle bouncing vertically
	 * on a plane (the floor).
	 */
	void roll_on_floor_sized(int argc, char *argv[]);

	/* -- Sphere study case --
	 * A particle falling onto a sphere below its
	 * initial position.
	 */
	void bounce_on_sphere(int argc, char *argv[]);
	/* -- Sphere study case --
	 * A sized particle falling onto a sphere below
	 * its initial position.
	 */
	void bounce_on_sphere_sized(int argc, char *argv[]);

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

	/* -- Hose case --
	 * This scene contains several geometrical objects
	 * (planes, triangles, spheres). This test is meant
	 * for efficiency measuring only.
	 */
	void hose_case(int argc, char *argv[]);

	/* -- 1-dimensional mesh case --
	 * This scene contains a plane (floor) and a
	 * simple 1-dimensional mesh.
	 */
	void mesh1d_case(int argc, char *argv[]);

	/* -- 2-dimensional regular mesh case --
	 * This scene contains a plane (floor) and a
	 * 2-dimensional regular mesh.
	 */
	void mesh2d_regular_case(int argc, char *argv[]);

} // -- namespace study_cases
