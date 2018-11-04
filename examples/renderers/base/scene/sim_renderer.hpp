#pragma once

// C++ includes
#include <vector>

// base includes
#include <base/scene/renderer.hpp>
#include <base/rgeom/rendered_geometry.hpp>

// physim includes
#include <physim/simulator.hpp>

/**
 * @brief Simulation renderer.
 */
class sim_renderer : public renderer {
	private:
		/// Renderer's simulator.
		physim::simulator S;
		/// Geometry in the simulation.
		std::vector<rgeom *> geometry;

	public:
		/// Default constructor.
		sim_renderer();
		/// Destructor.
		~sim_renderer();

		void set_particle_size(float s);
		void set_spring_width(float w);
		void add_geometry(rgeom *r);

		/// Returns a reference to the renderer's simulator.
		physim::simulator& get_simulator();

		void apply_time_step();
		void render_simulation() const;
};