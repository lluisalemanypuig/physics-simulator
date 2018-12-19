#include <physim/structures/octree.hpp>

// C includes
#include <assert.h>

// C++ includes
#include <algorithm>
#include <iostream>
#include <numeric>
#include <limits>
using namespace std;

// physim includes
#include <physim/math/private/math3/add.hpp>
#include <physim/math/private/math3/div.hpp>
#include <physim/math/private/math3/comparison.hpp>
#include <physim/geometry/rectangle.hpp>

namespace physim {
using namespace math;
using namespace geometric;

namespace structures {

// PRIVATE

// -- node

octree::node::node() {
	tris_idxs = nullptr;
	for (unsigned char i = 0; i < 8; ++i) {
		children[i] = nullptr;
	}
}

octree::node::~node() {
	for (unsigned char i = 0; i < 8; ++i) {
		if (children[i] != nullptr) {
			delete children[i];
			children[i] = nullptr;
		}
	}

	if (tris_idxs != nullptr) {
		tris_idxs->clear();
		delete tris_idxs;
		tris_idxs = nullptr;
	}
}

bool octree::node::is_leaf() const {
	return tris_idxs != nullptr;
}

// PRIVATE

// -- octree

octree::node *octree::make_octree_triangles(
	const vec3& vmin, const vec3& vmax,
	const vector<vec3>& vertices,
	const vector<size_t>& triangles,
	const vector<vector<size_t> >& tris_per_vertex,
	const vector<size_t>& vertices_idxs,
	const vector<size_t>& triangle_idxs
)
const
{
	node *n = new node();

	// set minimum and maximum points
	__pm3_assign_v(n->vmin, vmin);
	__pm3_assign_v(n->vmax, vmax);
	// make center point
	__pm3_add_v_v(n->center, n->vmin, n->vmax);
	__pm3_div_acc_s(n->center, 2.0f);

	if (vertices_idxs.size() <= 8) {
		// If there are less than 8 vertices to be partitioned
		// then we store the triangle indices and stop here.
		// Note that in the event when there are no vertices
		// we can't possibly assign a value to the center point.
		// Let's hope that this will not be an issue.

		n->tris_idxs = new vector<size_t>(triangle_idxs);
		return n;
	}

	// Points defining the 12 'rectangles' that
	// partition this subspace.
	// Meaning of letters:
	//		m: minimum, c: average, M: maximum
	// top level: maximum Z
	const vec3 cmM(n->center.x, vmin.y, vmax.z);
	const vec3 mcM(vmin.x, n->center.y, vmax.z);
	const vec3 cMM(n->center.x, vmax.y, vmax.z);
	const vec3 McM(vmax.x, n->center.y, vmax.z);
	const vec3 ccM(n->center.x, n->center.y, vmax.z);
	// mid level: mid Z
	const vec3 cmc(n->center.x, vmin.y, n->center.z);
	const vec3 mcc(vmin.x, n->center.y, n->center.z);
	const vec3 cMc(n->center.x, vmax.y, n->center.z);
	const vec3 Mcc(vmax.x, n->center.y, n->center.z);
	const vec3 mMc(vmin.x, vmax.y, n->center.z);
	const vec3 MMc(vmax.x, vmax.y, n->center.z);
	const vec3 Mmc(vmax.x, vmin.y, n->center.z);
	const vec3 mmc(vmin.x, vmin.y, n->center.z);
	// low level: minimum Z
	const vec3 cmm(n->center.x, vmin.y, vmin.z);
	const vec3 mcm(vmin.x, n->center.y, vmin.z);
	const vec3 cMm(n->center.x, vmax.y, vmin.z);
	const vec3 Mcm(vmax.x, n->center.y, vmin.z);
	const vec3 ccm(n->center.x, n->center.y, vmin.z);

	// rectangles at upper half
	const rectangle rt1(ccM, mcM, mcc, n->center);
	const rectangle rt2(ccM, cMM, cMc, n->center);
	const rectangle rt3(ccM, McM, Mcc, n->center);
	const rectangle rt4(ccM, cmM, cmc, n->center);
	// rectangles at division plane
	const rectangle rm1(mcc, mMc, cMc, n->center);
	const rectangle rm2(cMc, MMc, Mcc, n->center);
	const rectangle rm3(Mcc, Mmc, cmc, n->center);
	const rectangle rm4(cmc, mmc, mcc, n->center);
	// rectangles at lower half
	const rectangle rb1(ccm, mcm, mcc, n->center);
	const rectangle rb2(ccm, cMm, cMc, n->center);
	const rectangle rb3(ccm, Mcm, Mcc, n->center);
	const rectangle rb4(ccm, cmm, cmc, n->center);
	auto intersections_rectangle =
	[&](const rectangle& r, size_t v1, size_t v2, bool i[8]) -> void {
		if (r.intersec_segment(vertices[v1], vertices[v2])) {
			unsigned char sub;

			sub = 0;
			__pm3_lt(sub, vertices[v1], n->center);
			i[sub] = true;
			sub = 0;
			__pm3_lt(sub, vertices[v2], n->center);
			i[sub] = true;
		}
	};
	auto intersections_all =
	[&](size_t v1, size_t v2, bool i[8]) -> void {
		intersections_rectangle(rt1, v1, v2, i);
		intersections_rectangle(rt2, v1, v2, i);
		intersections_rectangle(rt3, v1, v2, i);
		intersections_rectangle(rt4, v1, v2, i);
		intersections_rectangle(rm1, v1, v2, i);
		intersections_rectangle(rm2, v1, v2, i);
		intersections_rectangle(rm3, v1, v2, i);
		intersections_rectangle(rm4, v1, v2, i);
		intersections_rectangle(rb1, v1, v2, i);
		intersections_rectangle(rb2, v1, v2, i);
		intersections_rectangle(rb3, v1, v2, i);
		intersections_rectangle(rb4, v1, v2, i);
	};

	// ----------------------------------------------------
	// If there are more than 8 vertices then partition the
	// space into subtrees

	// subspace index per vertex, using '9' as invalid value
	vector<unsigned char> subspace_per_vertex(vertices.size(), 9);
	// vert_idxs_space contains indices pointing
	// to vertices in parameter 'vertices'.
	// These are the vertices that are put into each
	// subspace.
	vector<size_t> vert_idxs_space[8];
	// tris_idxs_space contains indices pointing
	// to triangles in parameter 'triangles'
	// These are the list of triangles incident
	// to each subspace.
	vector<size_t> tris_idxs_space[8];

	// 1. Partition the set of points into every subspace.
	// Also add partial results of the triangles incident
	// to that subspace.

	for (size_t v_idx : vertices_idxs) {
		const vec3& v = vertices[v_idx];

		unsigned char s = 0;
		__pm3_lt(s, v, n->center);

		// s contains in its three lowest-weight bits the
		// result of comparing v < n->center.
		// This points us to one of the node's children.

		// vertex at position 'v_idx' is
		// assigned to subspace 's'
		vert_idxs_space[s].push_back(v_idx);
		subspace_per_vertex[v_idx] = s;

		// if vertex 'v_idx' goes to subspace 's' then
		// so do all adjacent triangles
		tris_idxs_space[s].insert(
			tris_idxs_space[s].end(),
			tris_per_vertex[v_idx].begin(),
			tris_per_vertex[v_idx].end()
		);
	}

	// obtain unique indices
	for (unsigned char s = 0; s < 8; ++s) {
		sort(tris_idxs_space[s].begin(), tris_idxs_space[s].end());
		auto last = unique(tris_idxs_space[s].begin(), tris_idxs_space[s].end());
		tris_idxs_space[s].erase(last, tris_idxs_space[s].end());
	}

	// 2. Find what subspaces each triangle intersects. Already
	// existing triangles are not included.
	for (size_t t_idx : triangle_idxs) {
		bool intersected1[8] = {false,false,false,false,false,false,false,false};
		bool intersected2[8] = {false,false,false,false,false,false,false,false};
		bool intersected3[8] = {false,false,false,false,false,false,false,false};

		// 2.1. get all the subspaces triangle 't_idx' intersects
		size_t v1 = triangles[t_idx    ];
		size_t v2 = triangles[t_idx + 1];
		size_t v3 = triangles[t_idx + 2];

		intersections_all(v1,v2, intersected1);
		intersections_all(v2,v3, intersected2);
		intersections_all(v3,v1, intersected3);

		// 2.2. take i-th subspace 's_i'. If any of the
		// vertices of the triangle belongs to 's_i' then
		// the triangle was already added to it, so it
		// should not be added again.
		for (unsigned char i = 0; i < 8; ++i) {
			if (
				(intersected1[i] or intersected2[i] or intersected3[i]) and
				(not (
					subspace_per_vertex[v1] == i or
					subspace_per_vertex[v2] == i or
					subspace_per_vertex[v3] == i
				))
			)
			{
				tris_idxs_space[i].push_back(t_idx);
			}
		}
	}

	// free unused memory
	subspace_per_vertex.clear();

	vec3 submin, submax;

	// 3. Partition the subspaces
	for (unsigned char i = 0; i < 8; ++i) {

		// make minimum and maximum points for the i-th child
		if ((i & 0x01) == 0) { submax.x = n->vmax.x; submin.x = n->center.x; }
		else { submax.x = n->center.x; submin.x = n->vmin.x; }
		if ((i & 0x02) == 0) { submax.y = n->vmax.y; submin.y = n->center.y; }
		else { submax.y = n->center.y; submin.y = n->vmin.y; }
		if ((i & 0x04) == 0) { submax.z = n->vmax.z; submin.z = n->center.z; }
		else { submax.z = n->center.z; submin.z = n->vmin.z; }

		n->children[i] =
		make_octree_triangles(
			submin, submax,
			vertices, triangles, tris_per_vertex,
			vert_idxs_space[i], tris_idxs_space[i]
		);

		// free unused memory
		vert_idxs_space[i].clear();
		tris_idxs_space[i].clear();
	}

	return n;
}

octree::node *octree::make_octree_vertices(
	const vec3& vmin, const vec3& vmax,
	const vector<vec3>& vertices,
	const vector<size_t>& vertices_idxs
)
const
{
	node *n = new node();

	// set minimum and maximum points
	__pm3_assign_v(n->vmin, vmin);
	__pm3_assign_v(n->vmax, vmax);
	// make center point
	__pm3_add_v_v(n->center, n->vmin, n->vmax);
	__pm3_div_acc_s(n->center, 2.0f);

	// vert_idxs_space contains indices pointing to vertices
	// in parameter 'vertices'. These are the vertices that
	// are put into each subspace.
	vector<size_t> vert_idxs_space[8];

	// partition the vertices
	for (size_t v_idx : vertices_idxs) {
		const vec3& v = vertices[v_idx];

		unsigned char s = 0;
		__pm3_lt(s, v, n->center);

		// s contains in its three lowest-weight bits the
		// result of comparing v < n->center.
		// This points us to one of the node's children.

		// vertex at position 'v_idx' is
		// assigned to subspace 's'
		vert_idxs_space[s].push_back(v_idx);
	}

	vec3 submin, submax;

	// 3. Partition the subspaces
	for (unsigned char i = 0; i < 8; ++i) {

		// make minimum and maximum points for the i-th child
		if ((i & 0x01) == 0) { submax.x = n->vmax.x; submin.x = n->center.x; }
		else { submax.x = n->center.x; submin.x = n->vmin.x; }
		if ((i & 0x02) == 0) { submax.y = n->vmax.y; submin.y = n->center.y; }
		else { submax.y = n->center.y; submin.y = n->vmin.y; }
		if ((i & 0x04) == 0) { submax.z = n->vmax.z; submin.z = n->center.z; }
		else { submax.z = n->center.z; submin.z = n->vmin.z; }

		n->children[i] =
		make_octree_vertices(submin, submax, vertices, vert_idxs_space[i]);

		// free unused memory
		vert_idxs_space[i].clear();
	}

	return n;
}

octree::node *octree::copy_node(const node *n) const {
	if (n == nullptr) {
		return nullptr;
	}

	node *c = new node();
	__pm3_assign_v(c->center, n->center);

	if (n->tris_idxs != nullptr) {
		// this node is a leaf, so no need to copy the children
		c->tris_idxs = new vector<size_t>(*(n->tris_idxs));
		return c;
	}

	for (unsigned char i = 0; i < 8; ++i) {
		c->children[i] = copy_node(n->children[i]);
	}
	return c;
}

void octree::get_boxes_node
(const node *n, std::vector<std::pair<math::vec3, math::vec3> >& boxes)
const
{
	if (n == nullptr) {
		return;
	}
	if (n->is_leaf()) {
		boxes.push_back(make_pair(n->vmin, n->vmax));
		return;
	}

	for (unsigned char c = 0; c < 8; ++c) {
		get_boxes_node(n->children[c], boxes);
	}
}

// PUBLIC

octree::octree() {
	root = nullptr;
}

octree::~octree() {
	clear();
}

// MEMORY

void octree::init(
	const vector<vec3>& vertices,
	const vector<size_t>& tris_indices
)
{
	clear();
	vector<size_t> vert_idxs(vertices.size());
	vector<size_t> tris_idxs(tris_indices.size()/3);

	size_t vidx = 0;
	size_t tidx = 0;

	auto vit = vert_idxs.begin();
	auto tit = tris_idxs.begin();
	while (vit != vert_idxs.end() and tit != tris_idxs.end()) {
		*vit = vidx;
		++vidx;

		*tit = tidx;
		tidx += 3;

		++vit; ++tit;
	}
	for (; vit != vert_idxs.end(); ++vit) {
		*vit = vidx; ++vidx;
	}
	for (; tit != tris_idxs.end(); ++tit) {
		*tit = tidx; tidx += 3;
	}

	// construct the maximum and minimum points of the root.
	static const float inf = numeric_limits<float>::max();
	vec3 vmin, vmax;
	__pm3_assign_s(vmin, inf);
	__pm3_assign_s(vmax, -inf);

	// Construct the 'tris_per_vertex' table.
	// The i-th position contains the indices of all the
	// triangles incident to the i-th vertex.
	// The indices stored are the indices of the first
	// corner of every triangle.
	vector<vector<size_t> > tris_per_vertex(vertices.size());
	for (size_t i = 0; i < tris_indices.size(); i += 3) {
		tris_per_vertex[tris_indices[i + 0]].push_back(i);
		tris_per_vertex[tris_indices[i + 1]].push_back(i);
		tris_per_vertex[tris_indices[i + 2]].push_back(i);

		// compute minimum and maximum
		__pm3_min2(vmin, vmin, vertices[tris_indices[i + 0]]);
		__pm3_max2(vmax, vmax, vertices[tris_indices[i + 0]]);
		__pm3_min2(vmin, vmin, vertices[tris_indices[i + 1]]);
		__pm3_max2(vmax, vmax, vertices[tris_indices[i + 1]]);
		__pm3_min2(vmin, vmin, vertices[tris_indices[i + 2]]);
		__pm3_max2(vmax, vmax, vertices[tris_indices[i + 2]]);
	}

	// perturb the minimum and maximum points
	// a little so that they do not have coordinates
	// equal to the center point
	__pm3_add_acc_s(vmin, -0.01f);
	__pm3_add_acc_s(vmax, +0.01f);

	root =
	make_octree_triangles(
		vmin, vmax, vertices, tris_indices,
		tris_per_vertex, vert_idxs, tris_idxs
	);
}

void octree::init(const vector<vec3>& vertices) {
	clear();

	static const float inf = numeric_limits<float>::max();
	vec3 vmin, vmax;
	__pm3_assign_s(vmin, inf);
	__pm3_assign_s(vmax, -inf);

	vector<size_t> idxs(vertices.size());
	for (size_t i = 0; i < vertices.size(); ++i) {
		__pm3_min2(vmin, vmin, vertices[i]);
		__pm3_max2(vmax, vmax, vertices[i]);
		idxs[i] = i;
	}

	root = make_octree_vertices(vmin, vmax, vertices, idxs);
}

void octree::clear() {
	if (root != nullptr) {
		delete root;
		root = nullptr;
	}
}

void octree::copy(const octree& part) {
	clear();
	root = copy_node(part.root);
}

// SETTERS

// GETTERS

void octree::get_indices
(const vec3& p, vector<size_t>& tris_idxs) const
{
	assert(root != nullptr);

	node *n = root;
	while (not n->is_leaf()) {
		unsigned char s = 0;
		__pm3_lt(s, p, n->center);
		n = n->children[s];
	}

	if (__pm3_inside_box(p, n->vmin, n->vmax)) {
		tris_idxs.insert(tris_idxs.end(),
						 n->tris_idxs->begin(),
						 n->tris_idxs->end());
	}
}

void octree::get_boxes(vector<pair<vec3, vec3> >& boxes) const {
	get_boxes_node(root, boxes);
}

// OTHERS

} // -- namespace structures
} // -- namespace physim
