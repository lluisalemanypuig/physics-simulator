#include <physim/simulator.hpp>

// physim includes
#include <physim/math/private/math3.hpp>
#include <physim/geometry/sphere.hpp>

namespace physim {

static geom::sphere Sj;

// ------------------------------------------------------

/* The main idea implemented in the following loop
 * is the following:
 *	-> Run the solver to predict a new position and
 *	   velocity.
 *
 *  -> We now know a prediction of the next position
 *     of the particle thanks to the solver. This is
 *     the "predicted position", stored in variable
 *     'pred_pos'.
 *
 *  -> Now we have a segment ('prev_pos','pred_pos')
 *     with which we can check collision with other
 *     geometry.
 *
 *  -> However, we should not update the particle with
 *     the first collision that has been detected.
 *     Instead, a prediction of the result of the
 *     collision is obtained and stored in 'pred_particle'.
 *     This particle is called the "predicted particle".
 *     Notice that this predicted particle also contains
 *     the predicted velocity, and may have other attributes
 *     modified after the collision (as part of the result
 *	   of the collision).
 *
 *  -> Then, checking the collision is done with the
 *     new segment from prev_pos to the position of
 *     the predicted particle. The position of the
 *     predicted particle, when there is any collision,
 *     is stored in variable 'pred_pos'.
 *
 *  -> Therefore, after the first collision, we have
 *     a prediction of the result of that collision
 *     of the particle with some geometry. Then, the
 *     second collision is detected with the segment
 *     from the previous position ('prev_pos') and
 *     the position of the predicted particle ('pred_pos').
 *     However, be aware now that the result of the
 *     second collision is NOT done with the state
 *     of the predicted particle. Instead, it is done
 *     with the original state of the particle.
 *
 *  -> The state that the particle takes is the state
 *     of the predicted particle at the end of the loop
 *     (only if there had been any collision).
 *
 */

// -----------------------------------------------
// FREE PARTICLES

bool simulator::find_update_geom_collision_free
(
	const particles::free_particle *p,
	math::vec3& pred_pos, math::vec3& pred_vel,
	particles::free_particle& coll_pred
)
{
	// has there been any collision?
	bool collision = false;

	// Check collision between the particle and
	// every fixed geometrical object in the scene.

	for (size_t i = 0; i < scene_fixed.size(); ++i) {
		const geom::geometry *g = scene_fixed[i];

		// if the particle collides with some geometry
		// then the geometry is in charge of updating
		// this particle's position, velocity, ...

		if (g->intersec_segment(p->cur_pos, pred_pos)) {
			collision = true;

			coll_pred = *p;

			// the geometry updates the predicted particle
			g->update_particle(pred_pos, pred_vel, &coll_pred);

			if (solver == solver_type::Verlet) {
				// this solver needs a correct position
				// for the 'previous' position of the
				// particle after a collision with geometry

				__pm3_sub_v_vs(coll_pred.prev_pos, coll_pred.cur_pos, coll_pred.cur_vel, dt);
			}

			// keep track of the predicted particle's position
			__pm3_assign_v(pred_pos, coll_pred.cur_pos);
			__pm3_assign_v(pred_vel, coll_pred.cur_vel);
		}
	}

	return collision;
}

bool simulator::find_update_particle_collision_free
(
	const particles::free_particle *p,
	math::vec3& pred_pos, math::vec3& pred_vel,
	particles::free_particle& coll_pred
)
{
	bool collision = false;

	for (size_t j = 0; j < sps.size(); ++j) {

		// if segment joining the particle's current position
		// and the predicted position intersects a sized
		// particle then we have to update the particle's
		// prediction

		if (__pm3_dist2(pred_pos, sps[j]->cur_pos) < (sps[j]->R)*(sps[j]->R)) {
			Sj.set_position(sps[j]->cur_pos);
			Sj.set_radius(sps[j]->R);

			collision = true;

			coll_pred = *p;

			// the geometry updates the predicted particle
			Sj.update_particle(pred_pos, pred_vel, &coll_pred);

			if (solver == solver_type::Verlet) {
				// this solver needs a correct position
				// for the 'previous' position of the
				// particle after a collision with geometry

				__pm3_sub_v_vs(coll_pred.prev_pos, coll_pred.cur_pos, coll_pred.cur_vel, dt);
			}

			// keep track of the predicted particle's position
			__pm3_assign_v(pred_pos, coll_pred.cur_pos);
			__pm3_assign_v(pred_vel, coll_pred.cur_vel);
		}
	}

	return collision;
}

// -----------------------------------------------
// SIZED PARTICLES

static inline
void update_with_geometry
(
	const geom::geometry *g, const solver_type& solver,
	float dt, const particles::sized_particle *p,
	math::vec3& pred_pos, math::vec3& pred_vel,
	particles::sized_particle& coll_pred,
	bool& collision
)
{
	// if the particle collides with some geometry
	// then the geometry is in charge of updating
	// this particle's position, velocity, ...

	bool inter = false;
	inter = inter or g->intersec_segment(p->cur_pos, pred_pos);
	inter = inter or g->intersec_sphere(pred_pos, p->R);
	if (inter) {
		collision = true;

		coll_pred = *p;

		// the geometry updates the predicted particle
		g->update_particle(pred_pos, pred_vel, &coll_pred);

		if (solver == solver_type::Verlet) {
			// this solver needs a correct position
			// for the 'previous' position of the
			// particle after a collision with geometry

			__pm3_sub_v_vs(coll_pred.prev_pos, coll_pred.cur_pos, coll_pred.cur_vel, dt);
		}

		// keep track of the predicted particle's position
		__pm3_assign_v(pred_pos, coll_pred.cur_pos);
		__pm3_assign_v(pred_vel, coll_pred.cur_vel);
	}
}

bool simulator::find_update_geom_collision_sized
(
	const particles::sized_particle *p,
	math::vec3& pred_pos, math::vec3& pred_vel,
	particles::sized_particle& coll_pred
)
{
	// has there been any collision?
	bool collision = false;

	// Check collision between the particle and
	// every fixed geometrical object in the scene.

	for (size_t i = 0; i < scene_fixed.size(); ++i) {
		const geom::geometry *g = scene_fixed[i];

		update_with_geometry(g, solver, dt, p, pred_pos, pred_vel, coll_pred, collision);
	}

	return collision;
}

bool simulator::find_update_particle_collision_sized
(
	const particles::sized_particle *p, size_t i,
	math::vec3& pred_pos, math::vec3& pred_vel,
	particles::sized_particle& coll_pred
)
{
	bool collision = false;

	size_t j;
	for (j = 0; j < i; ++j) {
		Sj.set_position(sps[j]->cur_pos);
		Sj.set_radius(sps[j]->R);

		update_with_geometry(&Sj, solver, dt, p, pred_pos, pred_vel, coll_pred, collision);
	}
	for (j = i + 1; j < sps.size(); ++j) {
		Sj.set_position(sps[j]->cur_pos);
		Sj.set_radius(sps[j]->R);

		update_with_geometry(&Sj, solver, dt, p, pred_pos, pred_vel, coll_pred, collision);
	}

	return collision;
}

} // -- namespace physim
