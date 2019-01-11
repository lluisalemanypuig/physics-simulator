#pragma once

// C includes
#include <stddef.h>

// physim includes
#include <physim/particles/fluid_particle.hpp>
#include <physim/fluids/fluid.hpp>
#include <physim/fluids/kernel_function.hpp>
#include <physim/structures/octree.hpp>

namespace physim {
namespace fluids {

/**
 * @brief Class implementing a newtonian fluid.
 */
class newtonian : public fluid {
	protected:

		void make_neighbours_lists
		(size_t i, std::vector<size_t>& neihgs, std::vector<float>& d2s) const;

		void make_neighbours_lists_tree
		(size_t i, std::vector<size_t>& neihgs, std::vector<float>& d2s) const;

		void initialise_density_pressure
		(size_t i, const std::vector<size_t>& neighs, const std::vector<float>& d2s);

		void update_force
		(size_t i, const std::vector<size_t>& neighs, const std::vector<float>& d2s);

	public:
		/// Default onstructor.
		newtonian();
		/// Destructor.
		virtual ~newtonian();

		// OPERATORS

		/**
		 * @brief Update the forces generated within the fluid.
		 *
		 * This method updates the forces acting on all particles.
		 * This update depends on the neighbouring information
		 * determined by the kernel functions.
		 *
		 * This method does not update positions or velocities.
		 *
		 * @pre The modification of a particles' force may assume
		 * that all particles start with null force (force
		 * equal to 0 in the three axes).
		 * @pre This method is called before computing the forces
		 * acting on the particles due to force fields.
		 */
		virtual void update_forces();
		/**
		 * @brief Update the forces generated within the fluid.
		 *
		 * This method updates the forces acting on all particles.
		 * This update depends on the neighbouring information
		 * determined by the kernel functions.
		 *
		 * This method does not update positions or velocities.
		 *
		 * @param n Number of threads
		 * @pre The modification of a particles' force may assume
		 * that all particles start with null force (force
		 * equal to 0 in the three axes).
		 * @pre This method is called before computing the forces
		 * acting on the particles due to force fields.
		 */
		virtual void update_forces(size_t n);
};

} // -- namespace fluids
} // -- namespace physim
