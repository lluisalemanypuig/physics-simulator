#pragma once

// physim includes
#include <physim/particles/free_particle.hpp>
#include <physim/particles/mesh_particle.hpp>

namespace physim {
namespace particles {

	void from_mesh_to_free(const mesh_particle& m, free_particle& f);
	void from_free_to_mesh(const free_particle& f, mesh_particle& m);

} // -- namespace particles
} // -- namespace physim
