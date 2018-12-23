#pragma once

// C includes
#include <stddef.h>

// physim includes
#include <physim/particles/fluid_particle.hpp>
#include <physim/fluids/kernel_pair.hpp>
#include <physim/structures/octree.hpp>

namespace physim {
namespace fluids {

/**
 * @brief Class implementing a fluid.
 *
 * A fluid is characterised by its @ref volume, @ref density and @ref viscosity.
 *
 * It is also characterised by several kernel functions for
 * - pressure: @ref kernel_pressure
 * - viscosity: @ref kernel_viscosity
 */
class fluid {
	protected:
		/// Number of particles of the fluid
		size_t N;
		/// Total volume of the fluid [m^3].
		float volume;
		/// Density of the fluid [Kg/m^3].
		float density;
		/**
		 * @brief Density of the particles at rest.
		 *
		 * Used to calculate every particle's initial density.
		 */
		float rest_density;
		/// Viscosity of the fluid.
		float viscosity;

		/// Particles of this fluid.
		particles::fluid_particle **ps;

		/// Kernel function for pressure.
		kernel_pair kernel_pressure;
		/// Kernel function for viscosity.
		kernel_pair kernel_viscosity;

		/**
		 * @brief Space partition of this fluid's particles.
		 *
		 * Used for fast neighbourhood queries.
		 */
		structures::octree *tree;

	public:
		/// Default onstructor.
		fluid();
		/// Destructor.
		virtual ~fluid();

		// OPERATORS

		/// Returns a reference to the @e i-th particle.
		particles::fluid_particle *operator[] (size_t i);
		/// Returns a constant reference to the @e i-th particle.
		const particles::fluid_particle *operator[] (size_t i) const;

		// MODIFIERS

		/**
		 * @brief Allocates memoery for @e n particles.
		 *
		 * Allocates memory for @e n fluid particles and sets their initial
		 * mass.
		 *
		 * The memory currently allocated is cleared only if @e n differs
		 * from the current number of paricles @ref N.
		 *
		 * It also initialises the mass of the particles and their index
		 * within the fluid.
		 * @param n Number of particles.
		 * @param vol Volume of the fluid (see @ref volume).
		 * @param dens Density of the fluid (see @ref density).
		 * @param visc Viscosity of the fluid (see @ref viscosity).
		 */
		void allocate(size_t n, float vol, float dens, float visc);

		/// Deallocates the memory occupied by this fluid.
		virtual void clear();

		/**
		 * @brief Builds the initial state of the fluid.
		 *
		 * After assigning each of this fluid's particles a position,
		 * the initial state needed for proper simulation is computed.
		 *
		 * This may include the computation of the initial density
		 * for each of this fluid's particles:
		 *
		 * \f$\rho_i = \sum_j m_j \cdot W(r_{ij})\f$
		 *
		 * and the initial pressure:
		 *
		 * \f$p_i = c_s^2(\rho_i - \rho_0)\f$
		 *
		 * where \f$c_s\f$ is the speed of sound, \f$\rho_0\f$ is the fluid's
		 * density at rest (see @ref rest_density), and \f$r_{ij}\f$ is the
		 * distance between @e i-th and @e j-th particles (hence the need of
		 * the initialisation of every particle's position).
		 *
		 * @pre In general, all this mesh's particles must have
		 * been initialised following the description above.
		 */
		virtual void make_initial_state();

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
		 * @brief Builds the space partition of this fluid's particles.
		 *
		 * Constructs the partition using @ref tree. The contents of the
		 * tree are first cleared and then rebuilt again.
		 */
		void make_partition();

		// SETTERS

		/**
		 * @brief Sets the pressure kernel function.
		 *
		 * This is evaluated for the approximation of
		 *
		 * \f$
		 * \left\langle -\frac{1}{\rho}\nabla \right\rangle_i
		 * \approx
		 * \sum_j P_{ij} \cdot \nabla W(r_{ij})
		 * \f$
		 *
		 * where
		 *
		 * \f$
		 * P_{ij} = -m_j
		 *		\left(
		 *			\frac{p_i}{\rho_i^2} + \frac{p_j}{\rho_j^2}
		 *		\right)
		 * \f$
		 *
		 * and \f$W(r_{ij})\f$ denotes the kernel function evaluated at the
		 * distance between the @e i-th and the @e j-th particles \f$r_{ij}\f$,
		 * \f$m_j\f$ is the @e j-th particle's mass, \f$p_i, \rho_i\f$ denote
		 * the @e i-th particle's pressure and density.
		 *
		 * @param kp Kernel function for pressure.
		 */
		void set_pressure_kernel(const kernel_pair& kp);
		/**
		 * @brief Sets the viscosity kernel function.
		 *
		 * This is evaluated for the approximation of
		 *
		 * \f$
		 * \left\langle \frac{\mu}{\rho}\nabla^2 v\right\rangle_i
		 * \approx
		 * \sum_j V_{ij} \cdot \nabla^2 W(r_{ij})
		 * \f$
		 *
		 * where
		 *
		 * \f$
		 * V_{ij} = \mu \cdot m_j
		 *		\left(
		 *			\frac{v_j - v_i}{\rho_i \rho_j}
		 *		\right)
		 * \f$
		 *
		 * and \f$W(r_{ij})\f$ denotes the kernel function evaluated at the
		 * distance between the @e i-th and the @e j-th particles \f$r_{ij}\f$,
		 * \f$m_j\f$ is the @e j-th particle's mass, \f$\mu\f$ is the fluid's
		 * viscosity (see @ref viscosity).
		 * @param kp Kernel function for pressure.
		 */
		void set_viscosity_kernel(const kernel_pair& kp);

		// GETTERS

		/// Returns the number of particles.
		size_t size() const;
		/// Returns the volume of the fluid.
		float get_volume() const;
		/// Returns the density of the fluid.
		float get_density() const;
		/// Returns the viscosity of the fluid.
		float get_viscosity() const;

		/// Returns a reference to this fluid's particles.
		particles::fluid_particle **get_particles();
		/// Returns a constant reference to this fluid's particles.
		particles::fluid_particle *const *get_particles() const;
};

} // -- namespace fluids
} // -- namespace physim
