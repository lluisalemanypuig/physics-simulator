#pragma once

// C includes
#include <stddef.h>

// physim includes
#include <physim/particles/fluid_particle.hpp>
#include <physim/fluids/kernel_pair.hpp>

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
		 * Used to calculate the particle's initial density.
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

	public:
		/// Default onstructor.
		fluid();
		/// Destructor.
		virtual ~fluid();

		/**
		 * @brief Initialises this fluid.
		 *
		 * Allocates memory for @e n fluid particles and sets their initial
		 * mass.
		 *
		 * The memory currently allocated is cleared only if @e n differs
		 * from the current number of paricles @ref N.
		 *
		 * It also initialises the mass of the particles, their index
		 * in the fluid, and their
		 * @param n Number of particles.
		 * @param vol Volume of the fluid (see @ref volume).
		 * @param dens Density of the fluid (see @ref density).
		 * @param visc Viscosity of the fluid (see @ref viscosity).
		 */
		void init(size_t n, float vol, float dens, float visc);

		/// Deallocates the memory occupied by this fluid.
		void clear();

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
};

} // -- namespace fluids
} // -- namespace physim
