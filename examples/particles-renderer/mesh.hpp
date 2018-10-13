#pragma once

// C++ includes
#include <iostream>
#include <vector>
using namespace std;

// glm includes
#include <glm/glm.hpp>
using namespace glm;

// Qt includes
#include <QOpenGLFunctions>

#if !defined (TEXTURE_START)
	#define TEXTURE_START 1
#else
	// display error message
#endif

struct face {
	string M_ID;
	bool is_quad;			// is this face a quad?
	bool smooth;			// should this face be smoothed?

	int normal_index[4];	// indexes of face's normals
							// normal_index[i] is the normal of vertex
							// vertex_number[i]
	int vertex_index[4];	// indices of vertices
	int text_coord[4];		// indices of texture coordinates

	face(int nidxs[4], int verts[4], int texts[4], const string& mat_id, bool isquad, bool s);

	face(const face& f);
};

struct material {
	string ID;
	int textureID;
	int illum;
	float Ns, Ni, d;
	vec4 Ka, Kd, Ks;

	material
	(
		const vec3& amb,
		const vec3& dif,
		const vec3& spec,
		float ns, float ni,float D,
		int ill, int textID, const string& id
	);
	material(const material& m);
};

class mesh {
	private:
		string mesh_name;

		// mesh data
		vector<vec3> vertices;
		vector<vec3> normals;
		vector<face> faces;

		// materials and textures
		vector<material> materials;
		vector<vec2> textures_coords;
		vector<unsigned int> textures_indexes;

	public:
		mesh();
		~mesh();

		// SETTERS

		void set_name(const string& name);

		void set_vertices(const vector<vec3>& verts);
		void set_faces(const vector<face>& faces);
		void set_normals(const vector<vec3>& normals);

		void set_materials(const vector<material>& mats);
		void set_texture_coords(const vector<vec2>& texts);
		void set_texture_indexes(const vector<unsigned int>& text_idxs);

		// GETTERS

		bool is_valid() const;

		// MODIFIERS

		void smooth();
		void scale_to_unit();

		// OTHERS

		void display_mesh_info();
		void render() const;
};

