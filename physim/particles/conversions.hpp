#pragma once

// physim includes
#include <physim/particles/free_particle.hpp>
#include <physim/particles/mesh_particle.hpp>

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

} // -- namespace particles
} // -- namespace physim
