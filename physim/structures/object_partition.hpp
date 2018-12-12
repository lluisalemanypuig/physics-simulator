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
 * @brief Object partition class.
 *
 * This is basically an implementation of an octree for a set of triangles,
 * forming a connected triangular mesh or not.
 */
class object_partition {
	private:
		/// Nodes of the octree.
		struct node {
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
		 * stored in @e triangles.
		 * @param[in] vertices The full list of non-repeated vertices of the
		 * triangles in @e triangles.
		 * @param[in] triangles The full list of triangles. Every three integer
		 * values we have one triangle.
		 * @param[in] tris_per_vertex The @e i-th position of this vector
		 * contains the indices of the triangles adjacent to the @e i-th
		 * vertex.
		 * @param[in] vertices_idxs The list of vertices to be partitioned.
		 * @param[in] triangle_idxs The list of triangles to incident to the
		 * node to be created. The indexes in this list are all multiples of 3
		 * and point to positions (also multiple of 3) in @e triangles.
		 */
		node *make_tree_at(
			const std::vector<math::vec3>& vertices,
			const std::vector<size_t>& triangles,
			const std::vector<std::vector<size_t> >& tris_per_vertex,
			const std::vector<size_t>& vertices_idxs,
			const std::vector<size_t>& triangle_idxs
		) const;

		/**
		 * @brief Copies a node.
		 *
		 * Recursively copies a node @e n.
		 * @param n Node to be copied.
		 * @return Reference to a node created in this function, a copy of @e n.
		 */
		node *copy_node(const node *n) const;

	public:
		/// Default constructor.
		object_partition();
		/// Destructor.
		~object_partition();

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

		/// Frees the memory occupied by this object.
		void clear();

		/**
		 * @brief Copies the partition passed as parameter.
		 *
		 * The contents of this partition are cleared.
		 * @param part An object partition.
		 */
		void copy(const object_partition& part);

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
		void get_triangles
		(const math::vec3& p, std::vector<size_t>& tris_idxs) const;

		// OTHERS
};

} // -- namespace structures
} // -- namespace physim
