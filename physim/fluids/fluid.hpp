#pragma once

// C includes
#include <stddef.h>

// physim includes
#include <physim/particles/fluid_particle.hpp>
#include <physim/fluids/kernel_function.hpp>
#include <physim/structures/octree.hpp>

namespace physim {
namespace fluids {

/**
 * @brief Class implementing a fluid.
 *
 * A fluid is characterised by its @ref volume, @ref density and @ref viscosity.
 *
 * It is also characterised by a kernel function and its gradients (see
 * @ref kernel, @ref kernel_gr, @ref kernel_gr2).
 */
class fluid {
	protected:
		/// Number of particles of the fluid.
		size_t N;
		/// Total volume of the fluid. [m^3]
		float volume;
		/// Fluid reference density \f$\rho_0\f$. [Kg/m^3]
		float density;
		/// Viscosity of the fluid. [Kg/(m*s)].
		float viscosity;
		/// The speed of sound through this fluid. [m/s]
		float speed_sound;

		/// Particles of this fluid.
		particles::fluid_particle *ps;

		/// Neighbourhood size.
		float R;
		/**
		 * @brief Kernel density function. \f$W\f$
		 *
		 * This function is used to compute the initial density of the
		 * particles:
		 *
		 * \f$
		 * \rho_i = \sum_j m_j \cdot W(r_{ij})
		 * \f$,
		 *
		 * for all particles \f$1 \le i \le N\f$,
		 * - \f$W(r_{ij})\f$ denotes the kernel function evaluated at the distance
		 * between the @e i-th and the @e j-th particle,
		 * - \f$m_j\f$ is the mass of the @e j-th particle.
		 *
		 * By default, this function returns 1;
		 */
		kernel_scalar_function kernel_density;
		/**
		 * @brief Kernel pressure function. \f$\nabla W\f$
		 *
		 * This function is used to evaluate the new pressure values:
		 *
		 * \f$
		 * \left\langle -\frac{1}{\rho}\nabla \right\rangle_i
		 * \approx
		 * \sum_j P_{ij} \cdot \nabla W_\rho(r_{ij})
		 * \f$
		 *
		 * where
		 *
		 * \f$
		 * P_{ij} = -m_j
		 *		\left(
		 *			\frac{p_i}{\rho_i^2} + \frac{p_j}{\rho_j^2}
		 *		\right)
		 * \f$,
		 *
		 * for all particles \f$1 \le i,j \le N\f$,
		 * - \f$\nabla W(r_{ij})\f$ denotes the gradient of the kernel function
		 * evaluated at the distance between the @e i-th and the @e j-th particles
		 * \f$r_{ij}\f$,
		 * - \f$m_j\f$ is the mass of the @e j-th particle,
		 * - \f$p_i, p_j\f$ denote the pressure,
		 * - \f$\rho_i, \rho_j\f$ denote the densities at particles @e i and @e j.
		 *
		 * By default, this function returns 1;
		 */
		kernel_vectorial_function kernel_pressure;
		/**
		 * @brief Kernel viscosity function. \f$\nabla^2 W\f$
		 *
		 * This is evaluated for the approximation of
		 *
		 * \f$
		 * \left\langle \frac{\mu}{\rho}\nabla^2 v\right\rangle_i
		 * \approx
		 * \sum_j V_{ij} \cdot \nabla^2 W_\mu(r_{ij})
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
		 * for all particles \f$1 \le i,j \le N\f$, where
		 * - \f$\nabla^2 W(r_{ij})\f$ denotes the gradient of the kernel function
		 * evaluated at the distance between the @e i-th and the @e j-th particles
		 * \f$r_{ij}\f$,
		 * - \f$\mu\f$ is the fluid's reference viscosity (see @ref viscosity).
		 * - \f$m_j\f$ is the mass of the @e j-th particle,
		 * - \f$p_i, p_j\f$ denote the pressures,
		 * - \f$\rho_i, \rho_j\f$ denote the densities,
		 * - \f$v_i, v_j\f$ denote the velocities of particles @e i and @e j.
		 *
		 * By default, this function returns 1;
		 */
		kernel_scalar_function kernel_viscosity;

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
		 * @param r Neighbourhood size. Radius of the largest sphere around
		 * a particle. This sphere defines the neighbourhood's size of a
		 * particle (see @ref R).
		 * @param cs Speed of sound in this fluid (see @ref speed_sound).
		 */
		void allocate(size_t n, float vol, float dens, float visc, float r, float cs);

		/// Deallocates the memory occupied by the particles of this fluid.
		virtual void clear();

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
		virtual void update_forces() = 0;
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
		virtual void update_forces(size_t n) = 0;

		/**
		 * @brief Builds the space partition of this fluid's particles.
		 *
		 * Constructs the partition using @ref tree. The contents of the
		 * tree are first cleared and then rebuilt again.
		 */
		void make_partition();

		// SETTERS

		/**
		 * @brief Sets the kernel density functions.
		 * @param W \f$W\f$, see @ref kernel_density.
		 */
		void set_kernel_density(const kernel_scalar_function& W_d);
		/**
		 * @brief Sets the kernel pressure functions.
		 * @param W_p \f$\nabla W\f$, see @ref kernel_pressure.
		 */
		void set_kernel_pressure(const kernel_vectorial_function& W_p);
		/**
		 * @brief Sets the kernel viscosity functions.
		 * @param W_v \f$\nabla^2 W\f$, see @ref kernel_viscosity.
		 */
		void set_kernel_viscosity(const kernel_scalar_function& W_v);

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
		particles::fluid_particle *get_particles();
		/// Returns a constant reference to this fluid's particles.
		const particles::fluid_particle *get_particles() const;
};

} // -- namespace fluids
} // -- namespace physim
