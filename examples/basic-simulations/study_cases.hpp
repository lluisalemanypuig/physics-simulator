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
using namespace sim;

// Custom includes
#include "utils.hpp"

namespace study_cases {

	void bouncing_ball(int argc, char *argv[]);

} // -- namespace study_cases
