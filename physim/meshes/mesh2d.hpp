#pragma once

// C++ includes
#include <vector>

// physim includes
#include <physim/particles/mesh_particle.hpp>
#include <physim/meshes/mesh.hpp>

namespace physim {
namespace meshes {

/**
 * @brief 2-Dimensional spring mesh.
 *
 * A 2-dimensional mesh does not differ much from a 3-dimensional
 * mesh. However, these are used mainly for cloth simulation.
 */
class mesh2d : public mesh {
	private:

	public:
		/// Default constructor.
		mesh2d();
		/**
		 * @brief Constructor with parameters.
		 * @param ke Elasticity parameter.
		 * @param kd Damping factor.
		 */
		mesh2d(float ke, float kd);
		/// Destructor.
		virtual ~mesh2d();

};

} // -- namespace meshes
} // -- namespace physim
