#include <physim/simulator.hpp>

// C++ includes
#include <iostream>
using namespace std;

// physim includes
#include <physim/math/private/math3.hpp>
#include <physim/math/private/numeric.hpp>
#include <physim/geometry/sphere.hpp>
#include <physim/geometry/object.hpp>

namespace physim {
using namespace math;
using namespace particles;

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

bool simulator::find_update_geomcoll_free
(
	const free_particle& p,
	vec3& pred_pos, vec3& pred_vel,
	free_particle& coll_pred
)
{
	// has there been any collision?
	bool collision = false;

	// Check collision between the particle and
	// every fixed geometrical object in the scene.

	for (size_t i = 0; i < scene_fixed.size(); ++i) {
		const geometric::geometry *g = scene_fixed[i];

		// if the particle collides with some geometry
		// then the geometry is in charge of updating
		// this particle's position, velocity, ...

		if (g->get_geom_type() != geometric::geometry_type::Object) {
			if (g->intersec_segment(p.cur_pos, pred_pos)) {
				collision = true;

				coll_pred = p;

				// the geometry updates the predicted particle
				g->update_particle(pred_pos, pred_vel, coll_pred);

				if (solver == solver_type::Verlet) {
					// this solver needs a correct position
					// for the 'previous' position of the
					// particle after a collision with geometry

					__pm3_sub_v_vs(coll_pred.prev_pos,
								   coll_pred.cur_pos,
								   coll_pred.cur_vel, dt);
				}

				// keep track of the predicted particle's position
				__pm3_assign_v(pred_pos, coll_pred.cur_pos);
				__pm3_assign_v(pred_vel, coll_pred.cur_vel);
			}
		}
		else {
			// faster test with geometrical objects
			const geometric::object *o =
				static_cast<const geometric::object *>(g);

			bool updated =
				o->update_particle(pred_pos, pred_vel, p, coll_pred);

			if (updated) {
				collision = true;

				if (solver == solver_type::Verlet) {
					// this solver needs a correct position
					// for the 'previous' position of the
					// particle after a collision with geometry

					__pm3_sub_v_vs(coll_pred.prev_pos,
								   coll_pred.cur_pos,
								   coll_pred.cur_vel, dt);
				}

				// keep track of the predicted particle's position
				__pm3_assign_v(pred_pos, coll_pred.cur_pos);
				__pm3_assign_v(pred_vel, coll_pred.cur_vel);
			}
		}

	}

	return collision;
}

// Correct the position of a free particle
// after colliding with a sized particle
#define free_pos_correction													\
	if (__physim_Sj.is_inside(coll_pred.cur_pos)) {							\
		__pm3_sub_v_v(centre_particle, coll_pred.cur_pos, sps[j].cur_pos);	\
		float l = __pm3_norm(centre_particle);								\
		__pm3_add_acc_vs													\
		(coll_pred.cur_pos, centre_particle, sps[j].R*__pm_inv(l) + 0.001f);\
	}

// s1, s2: speed before collision
static inline void update_particles_velocity
(free_particle& in, free_particle& out, float s1, float s2)
{
	// following the steps in
	// https://www.atmos.illinois.edu/courses/atmos100/userdocs/3Dcollisions.html

	// 1. angles between the two colliders
	vec3 D;
	__pm3_sub_v_v(D, in.cur_pos, out.cur_pos);

	float xyz1 = angle_xyz(D, in.cur_vel);
	float xyz2 = angle_xyz(D, out.cur_vel);

	// 2. force vectors towards each other
	vec3 c1, c2;
	__pm3_assign_vs(c1, in.cur_vel,xyz1);
	__pm3_assign_vs(c2, out.cur_vel,xyz2);

	// 3. decompose this vector into x'-y'-z' components,
	// where x' is aligned with the center-line
	float a_xy1, a_xz1, a_xy2, a_xz2;
	a_xy1 = (__pm_is_zero(in.cur_vel.x, 1e-4f) and __pm_is_zero(in.cur_vel.y, 1e-4f) ?
			 0.0f : angle_xy(in.cur_vel, in.cur_vel));
	a_xz1 = (__pm_is_zero(in.cur_vel.x, 1e-4f) and __pm_is_zero(in.cur_vel.y, 1e-4f) ?
			 0.0f : angle_xz(in.cur_vel, in.cur_vel));

	a_xy2 = (__pm_is_zero(out.cur_vel.x, 1e-4f) and __pm_is_zero(out.cur_vel.y, 1e-4f) ?
			 0.0f : angle_xy(out.cur_vel, out.cur_vel));
	a_xz2 = (__pm_is_zero(out.cur_vel.x, 1e-4f) and __pm_is_zero(out.cur_vel.y, 1e-4f) ?
			 0.0f : angle_xz(out.cur_vel, out.cur_vel));

	c1.x = c1.x*std::sin(a_xz1)*std::cos(a_xy1);
	c1.y = c1.y*std::sin(a_xz1)*std::sin(a_xy1);
	c1.z = c1.z*std::cos(a_xz1);

	c2.x = c2.x*std::sin(a_xz2)*std::cos(a_xy2);
	c2.y = c2.y*std::sin(a_xz2)*std::sin(a_xy2);
	c2.z = c2.z*std::cos(a_xz2);

	// 4. determine the force vector normal to the center-line
	vec3 n1, n2;
	__pm3_sub_v_v(n1, in.cur_vel, c1);
	__pm3_sub_v_v(n2, out.cur_vel, c2);

	// 5. compose the new vectors into a new velocity
	__pm3_add_v_v(in.cur_vel, c2, n1);
	__pm3_add_v_v(out.cur_vel, c1, n2);

	// 6. The new velocies must have
	// the same norm as before
	normalise(in.cur_vel, in.cur_vel);
	normalise(out.cur_vel, out.cur_vel);

	// velocity correction (2/2)
	__pm3_assign_vs(in.cur_vel, in.cur_vel,s1);
	__pm3_assign_vs(out.cur_vel, out.cur_vel,s2);
}

bool simulator::find_update_partcoll_free
(
	const free_particle& p,
	vec3& pred_pos, vec3& pred_vel,
	free_particle& coll_pred
)
{
	geometric::sphere __physim_Sj;
	vec3 centre_particle;
	bool collision = false;

	// old speeds
	float s1,s2;

	for (size_t j = 0; j < sps.size(); ++j) {

		// if segment joining the particle's current position
		// and the predicted position intersects a sized
		// particle then we have to update the particle's
		// prediction

		float r2 = (sps[j].R)*(sps[j].R);
		if (__pm3_dist2(pred_pos, sps[j].cur_pos) < r2) {
			__physim_Sj.set_position(sps[j].cur_pos);
			__physim_Sj.set_radius(sps[j].R);

			collision = true;

			// update position of particle using collision method

			coll_pred = p;

			// First, check whether the current position is inside
			// the sphere. If so, change it to the closest point
			// on the surface of the surface, plus some epsillon
			free_pos_correction

			// velocity correction (1/2)
			s1 = __pm3_norm(coll_pred.cur_vel);
			s2 = __pm3_norm(sps[j].cur_vel);

			// the geometry updates the predicted particle
			__physim_Sj.update_particle(pred_pos, pred_vel, coll_pred);

			// velocity correction (2/2)
			update_particles_velocity(coll_pred, sps[j], s1, s2);

			if (solver == solver_type::Verlet) {
				// this solver needs a correct position
				// for the 'previous' position of the
				// particle after a collision with geometry

				__pm3_sub_v_vs(coll_pred.prev_pos,
							   coll_pred.cur_pos,
							   coll_pred.cur_vel, dt);
			}

			// keep track of the predicted particle's position
			__pm3_assign_v(pred_pos, coll_pred.cur_pos);
			__pm3_assign_v(pred_vel, coll_pred.cur_vel);
		}
	}

	// do the same for agent particles
	for (size_t j = 0; j < aps.size(); ++j) {

		// if segment joining the particle's current position
		// and the predicted position intersects a sized
		// particle then we have to update the particle's
		// prediction

		float r2 = (aps[j].R)*(aps[j].R);
		if (__pm3_dist2(pred_pos, aps[j].cur_pos) < r2) {
			__physim_Sj.set_position(aps[j].cur_pos);
			__physim_Sj.set_radius(aps[j].R);

			collision = true;

			coll_pred = p;

			// First, check whether the current position is inside
			// the sphere. If so, change it to the closest point
			// on the surface of the surface, plus some epsillon
			free_pos_correction

			// velocity correction (1/2)
			s1 = __pm3_norm(coll_pred.cur_vel);
			s2 = __pm3_norm(aps[j].cur_vel);

			// the geometry updates the predicted particle
			__physim_Sj.update_particle(pred_pos, pred_vel, coll_pred);

			// velocity correction (2/2)
			update_particles_velocity(coll_pred, sps[j], s1, s2);

			if (solver == solver_type::Verlet) {
				// this solver needs a correct position
				// for the 'previous' position of the
				// particle after a collision with geometry

				__pm3_sub_v_vs(coll_pred.prev_pos,
							   coll_pred.cur_pos,
							   coll_pred.cur_vel, dt);
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

bool simulator::find_update_geomcoll_sized
(
	const sized_particle& in,
	vec3& pred_pos, vec3& pred_vel,
	sized_particle& coll_pred
)
{
	// has there been any collision?
	bool collision = false;

	// Check collision between the particle and
	// every fixed geometrical object in the scene.

	for (size_t i = 0; i < scene_fixed.size(); ++i) {
		const geometric::geometry *g = scene_fixed[i];

		// if the particle collides with some geometry
		// then the geometry is in charge of updating
		// this particle's position, velocity, ...

		if (g->get_geom_type() != geometric::geometry_type::Object) {
			bool inter = false;
			inter = inter or g->intersec_segment(in.cur_pos, pred_pos);
			inter = inter or g->intersec_sphere(pred_pos, in.R);
			if (inter) {
				collision = true;

				coll_pred = in;

				// the geometry updates the predicted particle
				g->update_particle(pred_pos, pred_vel, coll_pred);

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
		else {
			// faster test with geometrical objects
			const geometric::object *o =
				static_cast<const geometric::object *>(g);

			bool updated =
				o->update_particle(pred_pos, pred_vel, in, coll_pred);

			if (updated) {
				collision = true;

				if (solver == solver_type::Verlet) {
					// this solver needs a correct position
					// for the 'previous' position of the
					// particle after a collision with geometry

					__pm3_sub_v_vs(coll_pred.prev_pos,
								   coll_pred.cur_pos,
								   coll_pred.cur_vel, dt);
				}

				// keep track of the predicted particle's position
				__pm3_assign_v(pred_pos, coll_pred.cur_pos);
				__pm3_assign_v(pred_vel, coll_pred.cur_vel);
			}
		}

	}

	return collision;
}

static inline bool spart_spart_collision
(const sized_particle& in, const sized_particle& out)
{
	float center_d2 = __pm3_dist2(in.cur_pos, out.cur_pos);
	float sum_r = in.R + out.R;
	return center_d2 <= sum_r*sum_r;
}

static inline void update_particles_position
(sized_particle& in, sized_particle& out)
{	
	// distances: from 'in' to 'out', for the others see
	// http://mathworld.wolfram.com/Circle-CircleIntersection.html
	const float D = __pm3_dist(in.cur_pos, out.cur_pos);
	const float d1 = (D*D + in.R*in.R - out.R*out.R)/(2*D);
	//const float d2 = (D*D - in.R*in.R + out.R*out.R)/(2*D);

	// unit vector from 'in' to 'out'
	vec3 u;
	__pm3_sub_v_v(u, out.cur_pos, in.cur_pos);
	normalise(u, u);

	/* This position correction is somewhat wrong
	 * since the direction the centers are moved
	 * need not be parallel to the velocity vectors.
	 * And I think they should be.
	 *
	 * However, with small time steps, this is good
	 * enough... but I don't like it.
	 */

	vec3 M,A1,A2;
	__pm3_add_v_vs(M,   in.cur_pos, u,  d1);		//  M := c1 + d1*u
	__pm3_add_v_vs(A1,  in.cur_pos, u,  in.R);	// A1 := c1 + R1*u
	__pm3_add_v_vs(A2, out.cur_pos, u,-out.R);	// A2 := c2 - R2*u

	// distances to move particle 'in' and 'out'
	// to correct positions
	float m_in  = __pm3_dist(M, A1);
	float m_out = __pm3_dist(M, A2);

	__pm3_add_v_vs( in.cur_pos,  in.cur_pos, u, -m_in);
	__pm3_add_v_vs(out.cur_pos, out.cur_pos, u, m_out);
}

// particle 'in' has index 'i'
void simulator::find_update_partcoll_sized
(sized_particle& in, size_t i)
{
	float s1, s2;

	for (size_t j = i + 1; j < sps.size(); ++j) {

		if (spart_spart_collision(in, sps[j])) {
			// update the particle's position before
			// updating velocities
			update_particles_position(in, sps[j]);

			s1 = __pm3_norm(in.cur_vel);
			s2 = __pm3_norm(sps[j].cur_vel);

			// collision between particle 'in' and particle 'j'
			update_particles_velocity(in, sps[j], s1, s2);

			// compute valid previous positions
			// for Verlet solver
			if (solver == solver_type::Verlet) {
				// this solver needs a correct position
				// for the 'previous' position of the
				// particle after a collision with geometry

				__pm3_sub_v_vs(in.prev_pos, in.cur_pos, in.cur_vel, dt);
				__pm3_sub_v_vs(sps[j].prev_pos, sps[j].cur_pos, sps[j].cur_vel, dt);
			}
		}
	}

	// check collisions with other agent particles
	for (size_t j = 0; j < aps.size(); ++j) {

		if (spart_spart_collision(in, aps[j])) {
			// update the particle's position before
			// updating velocities
			update_particles_position(in, aps[j]);

			s1 = __pm3_norm(in.cur_vel);
			s2 = __pm3_norm(aps[j].cur_vel);

			// collision between particle 'in' and particle 'j'
			update_particles_velocity(in, aps[j], s1, s2);

			// compute valid previous positions
			// for Verlet solver
			if (solver == solver_type::Verlet) {
				// this solver needs a correct position
				// for the 'previous' position of the
				// particle after a collision with geometry

				__pm3_sub_v_vs(in.prev_pos, in.cur_pos, in.cur_vel, dt);
				__pm3_sub_v_vs(aps[j].prev_pos, aps[j].cur_pos, aps[j].cur_vel, dt);
			}
		}
	}
}

// particle 'in' has index 'i'
void simulator::find_update_partcoll_agent
(agent_particle& in, size_t i)
{
	float s1,s2;

	// check collisions with sized particles
	for (size_t j = 0; j < sps.size(); ++j) {

		if (spart_spart_collision(in, sps[j])) {
			// update the particle's position before
			// updating velocities
			update_particles_position(in, sps[j]);

			s1 = __pm3_norm(in.cur_vel);
			s2 = __pm3_norm(sps[j].cur_vel);

			// collision between particle 'in' and particle 'j'
			update_particles_velocity(in, sps[j], s1, s2);

			// compute valid previous positions
			// for Verlet solver
			if (solver == solver_type::Verlet) {
				// this solver needs a correct position
				// for the 'previous' position of the
				// particle after a collision with geometry

				__pm3_sub_v_vs(in.prev_pos, in.cur_pos, in.cur_vel, dt);
				__pm3_sub_v_vs(sps[j].prev_pos, sps[j].cur_pos, sps[j].cur_vel, dt);
			}
		}
	}

	// check collisions with other agent particles
	for (size_t j = i + 1; j < aps.size(); ++j) {

		if (spart_spart_collision(in, aps[j])) {
			// update the particle's position before
			// updating velocities
			update_particles_position(in, aps[j]);

			s1 = __pm3_norm(in.cur_vel);
			s2 = __pm3_norm(aps[j].cur_vel);

			// collision between particle 'in' and particle 'j'
			update_particles_velocity(in, aps[j], s1, s2);

			// compute valid previous positions
			// for Verlet solver
			if (solver == solver_type::Verlet) {
				// this solver needs a correct position
				// for the 'previous' position of the
				// particle after a collision with geometry

				__pm3_sub_v_vs(in.prev_pos, in.cur_pos, in.cur_vel, dt);
				__pm3_sub_v_vs(aps[j].prev_pos, aps[j].cur_pos, aps[j].cur_vel, dt);
			}
		}
	}
}

} // -- namespace physim
