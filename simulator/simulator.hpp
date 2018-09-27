#pragma once

// C++ includes
#include <vector>
using namespace std;

// Custom includes
#include "geometry.hpp"

namespace sim {

/**
 * @brief Simulator class.
 *
 * This class contains the algorihm used to simulate the
 * movement of particles and systems of particles (like
 * springs systems).
 */
class simulator {
	private:
		/**
		 * @brief The collection of objects whose position
		 * does not depend on the interaction with other objects.
		 */
		vector<geom::geometry *> ignored;

		// vector<particle *> ps;

	public:
		/// Default constructor
		simulator();
		/// Destructor
		~simulator();
};

} // -- namespace sim
