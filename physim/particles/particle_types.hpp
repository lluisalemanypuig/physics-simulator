#pragma once

// C++ includes
#include <cstdint>

namespace physim {
namespace particles {

/// The different types of particles.
enum class particle_type : int8_t {
	/// Base particle, see @ref base_particle.
	base_particle = 0,
	/// Free particle, see @ref free_particle.
	free_particle,
	/// Sized particle, see @ref sized_particle.
	sized_particle,
	/// Mesh particle, see @ref mesh_particle.
	mesh_particle,
	/// Fluid particle, see @ref fluid_particle.
	fluid_particle,
	/// Agent particle, see @ref agent_particle.
	agent_particle
};

} // -- namespace particles
} // -- namespace physim
