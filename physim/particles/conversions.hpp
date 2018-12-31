#pragma once

// physim includes
#include <physim/particles/free_particle.hpp>
#include <physim/particles/mesh_particle.hpp>
#include <physim/particles/fluid_particle.hpp>

namespace physim {
namespace particles {

	/**
	 * @brief Conversion between types of particles.
	 *
	 * Only the common attributes are copied.
	 * @param[in] m From a mesh particle.
	 * @param[out] f To a free particle.
	 */
	void from_mesh_to_free(const mesh_particle& m, free_particle& f);
	/**
	 * @brief Conversion between types of particles.
	 *
	 * Only the common attributes are copied.
	 * @param[in] f From a free particle.
	 * @param[out] m To a mesh particle.
	 */
	void from_free_to_mesh(const free_particle& f, mesh_particle& m);

	/**
	 * @brief Conversion between types of particles.
	 *
	 * Only the common attributes are copied.
	 * @param[in] m From a mesh particle.
	 * @param[out] f To a free particle.
	 */
	void from_fluid_to_free(const fluid_particle& fl, free_particle& fr);
	/**
	 * @brief Conversion between types of particles.
	 *
	 * Only the common attributes are copied.
	 * @param[in] f From a free particle.
	 * @param[out] m To a fluid particle.
	 */
	void from_free_to_fluid(const free_particle& fr, fluid_particle& fl);

} // -- namespace particles
} // -- namespace physim
