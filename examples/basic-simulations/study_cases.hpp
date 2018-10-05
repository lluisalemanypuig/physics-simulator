#pragma once

// C includes
#include <string.h>

// C++ includes
#include <iostream>
using namespace std;

// glm includes
#include <glm/glm.hpp>
using namespace glm;

// simulator includes
#include <simulator/simulator.hpp>
#include <simulator/particle.hpp>
using namespace physim;

// Custom includes
#include "utils.hpp"

namespace study_cases {

	void list_all_cases();
	void choose_case(int argc, char *argv[]);

	/**
	 * @brief Floor study case.
	 *
	 * Simply a particle bouncing vertically
	 * on a plane (the floor).
	 */
	void floor(int argc, char *argv[]);

	/**
	 * @brief Triangle-triangle-floor case
	 *
	 * This scene contains two triangles and a floor.
	 * The trangles are almost facing each other.
	 */
	void tri_tri_floor(int argc, char *argv[]);

} // -- namespace study_cases
