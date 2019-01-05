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
 * @ref init(const std::vector<math::vec3>&, const std::vector<size_t>&, size_t)
 * ),
 *
 * and a set of vertices
 * (see function
 * @ref init(const std::vector<math::vec3>&, size_t) ).
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

			/// Indices to the objects' indices.
			size_t *idxs;
			/// Amount of indices stored.
			size_t count;
			/// Is this node a leaf?
			bool leaf;

			/// Pointers to the children of this node.
			node *children[8];

			// Functions

			/// Default constructor. Initialises pointers to null.
			node();
			/// Destructor.
			~node();

			/// Returns a pointer to the beginning of @ref idxs.
			size_t *begin_idxs();
			/// Returns a constant pointer to the beginning of @ref idxs.
			const size_t *begin_idxs() const;
			/// Returns a pointer to the ending of @ref idxs.
			size_t *end_idxs();
			/// Returns a constant pointer to the ending of @ref idxs.
			const size_t *end_idxs() const;
		};

		/// Root of the octree.
		node *root;

	private:

		/**
		 * @brief Builds a tree rooted at a node that partitions the triangles
		 * stored in @e triangles pointed by @e triangle_idxs.
		 * @param lod Threshold value for vertex partition. Below this amount,
		 * vertices will not be partitioned anymore.
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
			size_t lod,
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
		 * @param lod Threshold value for vertex partition. Below this amount,
		 * vertices will not be partitioned anymore.
		 * @param vmin Point with the minimum value coordinates of the points
		 * in @e vertices.
		 * @param vmax Point with the maximum value coordinates of the points
		 * in @e vertices.
		 * @param it Pointer to the first component of the first vertex.
		 * @param offset Size in bytes between consecutive vertices' first
		 * component.
		 * @param vertices_idxs The list of vertices to be partitioned.
		 * @return Returns the root of an octree that partitions the points
		 * in @e vertices_idxs.
		 */
		node *make_octree_vertices(
			const void *it, size_t offset,
			const math::vec3& vmin, const math::vec3& vmax,
			const std::vector<size_t>& vertices_idxs,
			size_t lod
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

		/**
		 * @brief Retrieves the indices stored at those cells intersecting
		 * a sphere of radius @e R centered at @e p.
		 * @param[in] p Center of sphere.
		 * @param[in] R Radius of sphere.
		 * @param[in] n Node of the tree
		 * @param[out] idxs List of indices (need not be unique).
		 */
		void get_indices_node(
			const math::vec3& p, float R,
			const node *n, std::vector<size_t>& idxs
		) const;

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
		 * @param lod Level Of Detail: minimum number of vertices needed to
		 * subdivide a cell.
		 */
		void init(
			const std::vector<math::vec3>& vertices,
			const std::vector<size_t>& tris_indices,
			size_t lod = 8
		);

		/**
		 * @brief Builds the partition of a cloud of points.
		 * @param vertices The vertices, without repetitions, of the cloud.
		 * @param lod Level Of Detail: minimum number of vertices per cell.
		 */
		void init(
			const std::vector<math::vec3>& vertices,
			size_t lod = 8
		);

		/**
		 * @brief Builds the partition of a cloud of points.
		 *
		 * For example, we may have a struct like this:
		 *
		 \verbatim
		 struct big {
			float a,b,c;
			char k;
			math::vec3 p1, p2, p3;
			char q;
			math::vec3 p4;
		 };
		 \endverbatim
		 *
		 * If one wanted to construct an octree using the math::vec3's
		 * p1, in the array
		 *
		 \verbatim
		 big b[512]
		 \endverbatim
		 *
		 * one would have to call this function as follows:
		 *
		 * init(&b[0].b1.x, 512, sizeof(big))
		 *
		 * @param p Pointer to the first element in which the math::vec3 is allocated.
		 * @param n Number of math::vec3.
		 * @param offset Byte offset between math::vec3.
		 * @param lod Level Of Detail: minimum number of vertices per cell.
		 */
		void init(const void *p, size_t n, size_t offset, size_t lod = 8);

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
		 * @brief Retrieves the indices of the objects incident to
		 * the cell of a point.
		 *
		 * Point @e p is located inside the octree at some cell. In
		 * parameter @e tris_idxs are stored the indices of those objects
		 * incident to that cell.
		 *
		 * @param[in] p Point to be located.
		 * @param[out] idxs The unique indices of the objectes incident to the
		 * cell where @e p is located at.
		 */
		void get_indices
		(const math::vec3& p, std::vector<size_t>& idxs) const;

		/**
		 * @brief Retrieves the indices of the objects incident to
		 * the cells intersecting a sphere.
		 *
		 * Sphere has center at @e p and radius @e R, Parameter @e tris_idxs
		 * stores the indices of those objects incident to the cells
		 *
		 * @param[in] p Center of the sphere.
		 * @param[in] R Radious of the sphere.
		 * @param[out] idxs The unique indices of the objectes incident to the
		 * cell that intersect the sphere.
		 */
		void get_indices
		(const math::vec3& p, float R, std::vector<size_t>& idxs) const;

		/**
		 * @brief Returns the bounding boxes of the cells in this octree.
		 * @param[out] boxes The vector contains pairs of elements with points
		 * the minimum and maximum coordinate values.
		 */
		void get_boxes
		(std::vector<std::pair<math::vec3, math::vec3> >& boxes) const;

		// OTHERS
};

} // -- namespace structures
} // -- namespace physim
