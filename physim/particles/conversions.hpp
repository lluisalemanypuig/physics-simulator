#pragma once

// physim includes
#include <physim/particles/free_particle.hpp>
#include <physim/particles/mesh_particle.hpp>
#include <physim/particles/sized_particle.hpp>
#include <physim/particles/agent_particle.hpp>

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
	 * @param[in] f From a sized particle.
	 * @param[out] m To an agent particle.
	 */
	void from_sized_to_agent(const sized_particle& f, agent_particle& m);
	/**
	 * @brief Conversion between types of particles.
	 *
	 * Only the common attributes are copied.
	 * @param[in] f From an agent particle.
	 * @param[out] m To a sized particle.
	 */
	void from_agent_to_sized(const agent_particle& f, sized_particle& m);

} // -- namespace particles
} // -- namespace physim
