#pragma once

// C++ includes
#include <vector>
using namespace std;

// Custom includes
#include <simulator/geometry.hpp>

namespace sim {

enum class solver : std::int8_t {
	none = -1,
	EulerOrig, EulerSemi, Verlet
};

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
