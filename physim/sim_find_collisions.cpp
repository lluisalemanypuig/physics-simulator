#include <physim/simulator.hpp>

// physim includes
#include <physim/math/private/math3.hpp>
#include <physim/geometry/sphere.hpp>

#include <iostream>
using namespace std;

namespace physim {

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

static geom::sphere Sj;
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

bool simulator::find_update_geom_collision_sized
(
	const particles::sized_particle *in,
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

		// if the particle collides with some geometry
		// then the geometry is in charge of updating
		// this particle's position, velocity, ...

		bool inter = false;
		inter = inter or g->intersec_segment(in->cur_pos, pred_pos);
		inter = inter or g->intersec_sphere(pred_pos, in->R);
		if (inter) {
			collision = true;

			coll_pred = *in;

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

static inline bool spart_spart_collision
(const particles::sized_particle *in, const particles::sized_particle *out)
{
	float center_d2 = __pm3_dist2(in->cur_pos, out->cur_pos);
	float sum_r = in->R + out->R;
	return center_d2 <= sum_r*sum_r;
}

static inline void spart_spart_update
(particles::sized_particle *in, particles::sized_particle *out)
{
	// following the steps in
	// https://www.atmos.illinois.edu/courses/atmos100/userdocs/3Dcollisions.html

	// 1. angles between the two colliders
	math::vec3 D;
	__pm3_sub_v_v(D, in->cur_pos, out->cur_pos);

	float xyz1 = math::angle_xyz(D, in->cur_vel);
	float xyz2 = math::angle_xyz(D, out->cur_vel);

	// 2. force vectors towards each other
	math::vec3 c1, c2;
	__pm3_assign_vs(c1, in->cur_vel,xyz1);
	__pm3_assign_vs(c2, out->cur_vel,xyz2);

	float a_xy1 = math::angle_xy(in->cur_vel, in->cur_vel);
	float a_xz1 = math::angle_xz(in->cur_vel, in->cur_vel);
	float a_xy2 = math::angle_xy(out->cur_vel, out->cur_vel);
	float a_xz2 = math::angle_xz(out->cur_vel, out->cur_vel);

	c1.x = c1.x*std::sin(a_xz1)*std::cos(a_xy1);
	c1.y = c1.y*std::sin(a_xz1)*std::sin(a_xy1);
	c1.z = c1.z*std::cos(a_xz1);

	c2.x = c2.x*std::sin(a_xz2)*std::cos(a_xy2);
	c2.y = c2.y*std::sin(a_xz2)*std::sin(a_xy2);
	c2.z = c2.z*std::cos(a_xz2);

	math::vec3 n1, n2;
	__pm3_sub_v_v(n1, in->cur_vel, c1);
	__pm3_sub_v_v(n2, out->cur_vel, c2);

	__pm3_add_v_v(in->cur_vel, c2, n1);
	__pm3_add_v_v(out->cur_vel, c1, n2);
}

// particle 'in' has index 'i'
void simulator::find_update_particle_collision_sized
(particles::sized_particle *in, size_t i)
{

	for (size_t j = i + 1; j < sps.size(); ++j) {

		if (spart_spart_collision(in, sps[j])) {
			// collision between particle 'in' and particle 'j'
			spart_spart_update(in, sps[j]);
		}
	}
}

} // -- namespace physim
