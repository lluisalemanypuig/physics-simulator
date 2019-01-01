#pragma once

// C++ includes
#include <vector>

// render includes
#include <render/scene/viewer.hpp>
#include <render/geometry/rgeometry.hpp>

// physim includes
#include <physim/simulator.hpp>

/**
 * @brief Simulation renderer.
 */
class sim_renderer : public viewer {
	private:
		/// Renderer's simulator.
		physim::simulator S;
		/// Geometry in the simulation.
		std::vector<rgeom *> geometry;
		/// Indices of the particles to be rendered
		std::vector<uint> free_indices;
		/// Indices of the particles to be rendered
		std::vector<std::vector<uint> > per_fluid_indices;

	public:
		/// Default constructor.
		sim_renderer();
		/// Destructor.
		~sim_renderer();

		void clear();

		void set_particle_size(float s);
		void set_spring_width(float w);
		void add_geometry(rgeom *r);

		void make_free_particle_indices();
		void make_fluid_particle_indices();

		const std::vector<rgeom *>& get_geometry() const;

		physim::simulator& get_simulator();

		void render_simulation() const;
};
