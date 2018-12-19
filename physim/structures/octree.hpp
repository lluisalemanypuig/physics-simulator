#pragma once

// C++ includes
#include <string>
#include <vector>

// physim includes
#include <physim/math/vec3.hpp>
#include <physim/geometry/triangle.hpp>

namespace physim {
namespace structures {

/**
 * @brief Octree class.
 *
 * This is an implementation of a basic octree for both set of triangles
 * (see function
 * @ref init(const std::vector<math::vec3>&, const std::vector<size_t>&)),
 * and a set of vertices
 * (see functions
 * @ref init(const std::vector<math::vec3>&),
 */
class octree {
	private:
		/// Octree's node definition.
		struct node {
			/// Points with the minimum coordinate values of the points within.
			math::vec3 vmin;
			/// Points with the maximum coordinate values of the points within.
			math::vec3 vmax;
			/// Point at the center of the region this node partitions.
			math::vec3 center;
			/// Indices to an object's triangles.
			std::vector<size_t> *tris_idxs;
			/// Pointers to the children of this node.
			node *children[8];

			/// Default constructor. Initialises pointers to null.
			node();
			/// Destructor.
			~node();

			/**
			 * @brief Is this node a leaf
			 *
			 * A node is a leaf if it stores triangle indices.
			 * @return Returns true if this node is a leaf. Returns false
			 * otherwise.
			 */
			bool is_leaf() const;
		};

		/// Root of the octree.
		node *root;

	private:

		/**
		 * @brief Builds a tree rooted at a node that partitions the triangles
		 * stored in @e triangles pointed by @e triangle_idxs.
		 * @param vmin Point with the minimum value coordinates of the
		 * points in @e vertices.
		 * @param vmax Point with the maximum value coordinates of the
		 * points in @e vertices.
		 * @param vertices The full list of non-repeated vertices of the
		 * triangles in @e triangles.
		 * @param triangles The full list of triangles. Every three integer
		 * values we have one triangle.
		 * @param tris_per_vertex The @e i-th position of this vector
		 * contains the indices of the triangles adjacent to the @e i-th
		 * vertex. This is an auxiliary vector for faster tree building.
		 * @param vertices_idxs The list of vertices to be partitioned.
		 * @param triangle_idxs The list of triangles to incident to the
		 * node to be created. The indexes in this list are all multiples of 3
		 * and point to positions (also multiple of 3) in @e triangles.
		 * @return Returns the root of an octree that partitions the triangles
		 * in @e triangle_idxs and the points in @e vertices_idxs.
		 */
		node *make_octree_triangles(
			const math::vec3& vmin, const math::vec3& vmax,
			const std::vector<math::vec3>& vertices,
			const std::vector<size_t>& triangles,
			const std::vector<std::vector<size_t> >& tris_per_vertex,
			const std::vector<size_t>& vertices_idxs,
			const std::vector<size_t>& triangle_idxs
		) const;

		/**
		 * @brief Builds a tree rooted at a node that partitions the vertices
		 * stored in @e vertices pointed by @e vertices_idxs.
		 * @param vmin Point with the minimum value coordinates of the points
		 * in @e vertices.
		 * @param vmax Point with the maximum value coordinates of the points
		 * in @e vertices.
		 * @param vertices The full list of vertices.
		 * @param vertices_idxs The list of vertices to be partitioned.
		 * @return Returns the root of an octree that partitions the points
		 * in @e vertices_idxs.
		 */
		node *make_octree_vertices(
			const math::vec3& vmin, const math::vec3& vmax,
			const std::vector<math::vec3>& vertices,
			const std::vector<size_t>& vertices_idxs
		) const;

		/**
		 * @brief Copies a node.
		 *
		 * Recursively copies a node @e n.
		 * @param n Node to be copied.
		 * @return Reference to a node created in this function, a copy of @e n.
		 */
		node *copy_node(const node *n) const;

		/**
		 * @brief Returns the bounding boxes of the cells in this octree.
		 * @param[in] n Node to obtain the boxes from.
		 * @param[out] boxes The vector contains pairs of elements with points
		 * the minimum and maximum coordinate values.
		 */
		void get_boxes_node
		(const node *n, std::vector<std::pair<math::vec3, math::vec3> >& boxes)
		const;

	public:
		/// Default constructor.
		octree();
		/// Destructor.
		~octree();

		// MEMORY

		/**
		 * @brief Builds the partition of an object.
		 *
		 * The object is made up of the triangles in @e triangles.
		 * @param vertices The vertices, without repetitions, of the object.
		 * @param tris_indices The vertices indices of each triangle. Every
		 * three integer values we have a triangle.
		 */
		void init(
			const std::vector<math::vec3>& vertices,
			const std::vector<size_t>& tris_indices
		);

		/**
		 * @brief Builds the partition of a cloud of points.
		 * @param vertices The vertices, without repetitions, of the cloud.
		 */
		void init(const std::vector<math::vec3>& vertices);

		/// Frees the memory occupied by this object.
		void clear();

		/**
		 * @brief Copies the partition passed as parameter.
		 *
		 * The contents of this partition are cleared.
		 * @param part An object partition.
		 */
		void copy(const octree& part);

		// SETTERS

		// GETTERS

		/**
		 * @brief Finds the indices of the triangles in the subspace of a point.
		 *
		 * Point @e p is located inside the octree at some subspace. In
		 * parameter @e tris_idxs are stored the indices of those triangles
		 *
		 * @param[in] p Point to be located.
		 * @param[out] tris_idxs The triangles incident to the subspace where
		 * @e p is located at.
		 */
		void get_indices
		(const math::vec3& p, std::vector<size_t>& tris_idxs) const;

		/**
		 * @brief Returns the bounding boxes of the cells in this octree.
		 * @param[out] boxes The vector contains pairs of elements with points
		 * the minimum and maximum coordinate values.
		 */
		void get_boxes(std::vector<std::pair<math::vec3, math::vec3> >& boxes) const;

		// OTHERS
};

} // -- namespace structures
} // -- namespace physim
