#include "mesh.hpp"

face::face(int nidxs[4], int verts[4], int texts[4], const string& mat_id, bool four, bool s) {
	for (int i = 0; i < 4; ++i) {
		normal_index[i] = nidxs[i];
		vertex_index[i] = verts[i];
		text_coord[i] = texts[i];
	}
	M_ID = mat_id;
	is_quad = four;
	smooth = s;
}

face::face(const face& f) {
	for (int i = 0; i < 4; ++i) {
		normal_index[i] = f.normal_index[i];
		vertex_index[i] = f.vertex_index[i];
		text_coord[i] = f.text_coord[i];
	}
	M_ID = f.M_ID;
	is_quad = f.is_quad;
	smooth = f.smooth;
}

material::material
(
	const vec3& amb,
	const vec3& dif,
	const vec3& spec,
	float ns, float ni,float D,
	int ill, int textID, const string& id
)
{
	ID = id;
	Ns = ns;
	Ni = ni;
	d =  D;
	illum = ill;
	textureID = textID;
	Ka = vec4(amb[0], amb[1], amb[2], D);
	Kd = vec4(dif[0], dif[1], dif[2], D);
	Ks = vec4(spec[0], spec[1], spec[2], D);
}

material::material(const material& m) {
	ID = m.ID;
	Ns = m.Ns;
	Ni = m.Ni;
	d =  m.d;
	illum = m.illum;
	textureID = m.textureID;
	Ka = m.Ka;
	Kd = m.Kd;
	Ks = m.Ks;
}

// PUBLIC

mesh::mesh() {
	mesh_name = "anonymous";
}

mesh::~mesh() {
	vector<unsigned int>::iterator it;
	for (it = textures_indexes.begin(); it != textures_indexes.end(); ++it) {
		glDeleteTextures(TEXTURE_START, &(*it));
	}
}

// SETTERS

void mesh::set_name(const string& name) {
	mesh_name = name;
}

void mesh::set_vertices(const vector<vec3>& verts) {
	vertices = verts;
}

void mesh::set_faces(const vector<face>& facs) {
	faces = facs;
}

void mesh::set_normals(const vector<vec3>& nrmls) {
	normals = nrmls;
}

void mesh::set_materials(const vector<material>& mats) {
	materials = mats;
}

void mesh::set_texture_coords(const vector<vec2>& texts) {
	textures_coords = texts;
}

void mesh::set_texture_indexes(const vector<unsigned int>& text_idxs) {
	textures_indexes = text_idxs;
}

// MODIFIERS

bool mesh::is_valid() const {
	if (vertices.size() == 0) {
		cerr << "mesh::is_valid: Error" << endl;
		cerr << "    Vertices not found in mesh '" << mesh_name << "'" << endl;
		return false;
	}
	if (faces.size() == 0) {
		cerr << "mesh::is_valid: Error" << endl;
		cerr << "    Faces not found in mesh '" << mesh_name << "'" << endl;
		return false;
	}

	// check that indexes are correct.
	for (size_t f = 0; f < faces.size(); ++f) {
		const face& F = faces[f];
		size_t lim = (F.is_quad ? 4 : 3);
		for (size_t i = 0; i < lim; ++i) {
			if (F.normal_index[i] != -1 and F.normal_index[i] > normals.size()) {
				cerr << "mesh::is_valid: Error:" << endl;
				cerr << "    Face " << f << " has " << i << "-th "
					 << "normal index " << F.normal_index[i]
					 << " out of bounds." << endl;
				return false;
			}
			if (F.vertex_index[i] != -1 and F.vertex_index[i] > vertices.size()) {
				cerr << "mesh::is_valid: Error:" << endl;
				cerr << "    Face " << f << " has " << i << "-th "
					 << "vertex index " << F.vertex_index[i]
					 << " out of bounds." << endl;
				return false;
			}
			if (F.text_coord[i] != -1 and F.text_coord[i] > textures_coords.size()) {
				cerr << "mesh::is_valid: Error:" << endl;
				cerr << "    Face " << f << " has " << i << "-th "
					 << "texture coordinate index " << F.text_coord[i]
					 << " out of bounds." << endl;
				return false;
			}
		}
	}

	return true;
}

void mesh::smooth() {

	// compute to what faces each
	// vertex is adjacent to
	vector<vector<int> > faces_per_vertex(vertices.size());
	for (size_t f = 0; f < faces.size(); ++f) {
		int lim = (faces[f].is_quad ? 4 : 3);
		for (int k = 0; k < lim; ++k) {
			int v = faces[f].vertex_index[k];
			faces_per_vertex[v].push_back(f);
		}
	}

	// Firstly, compute the smoothed normals for each vertex,
	// and store them in a separate vector.
	vector<vec3> smoothed_normals(vertices.size(), vec3(0.0f,0.0f,0.0f));

	// compute normals for the vertices that make
	// up the faces marked with 'smooth = true'
	for (size_t v = 0; v < vertices.size(); ++v) {
		vec3& normal = smoothed_normals[v];

		// add to 'normal' the normal of those faces that share vertex V
		// afi: adjacent face index
		for (int afi : faces_per_vertex[v]) {
			// when smoothing, it is assumed that each
			// face has the SAME normal index
			normal += normals[faces[afi].normal_index[0]];
		}

		// average the normal
		normal /= faces_per_vertex[v].size();
		// normalise the normal
		normal = glm::normalize(normal);

		// since 'normal' is a reference to a value of
		// 'smoothed_normals' we do not need to
		// assign this normal to the vector
	}

	// Secondly, replace normal indices in the faces.
	for (size_t f = 0; f < faces.size(); ++f) {
		int lim = (faces[f].is_quad ? 4 : 3);
		for (int k = 0; k < lim; ++k) {
			faces[f].normal_index[k] = faces[f].vertex_index[k];
		}
	}

	// Finally, store the smoothed normals
	normals = smoothed_normals;
}

void mesh::scale_to_unit() {
	glm::vec3 center(0.0f, 0.0f, 0.0f);
	glm::vec3 m(1e10, 1e10, 1e10);
	glm::vec3 M(-1e10, -1e10, -1e10);

	for (size_t i = 0; i < vertices.size(); ++i) {
		center += vertices[i];
		m = glm::min(m, vertices[i]);
		M = glm::max(M, vertices[i]);
	}
	center /= vertices.size();

	float largestSize = std::max(M.x - m.x, std::max(M.y - m.y, M.z - m.z));

	for (unsigned int i = 0; i < vertices.size(); ++i) {
		vertices[i] = (vertices[i] - center)/largestSize;
	}
}

// OTHERS

void mesh::display_mesh_info() {
	cout << "Mesh '" << mesh_name << "' information: " << endl;
	cout << "    # Vertices= " << vertices.size() << endl;
	cout << "    # Faces= " << faces.size() << endl;
	cout << "    # Normals= " << normals.size() << endl;
	cout << "    # Materials= " << materials.size() << endl;
	cout << "    # Texture coordinates= " << textures_coords.size() << endl;
	cout << "    # Textures= " << textures_indexes.size() << endl;

	cout << "    Materials: " << endl;
	for (size_t i = 0; i < materials.size(); ++i) {
		cout << "        newmtl " << materials[i].ID << endl;
		cout << "        Ns " << materials[i].Ns;
		cout << endl;
		cout << "        Ka";
		for (int j = 0; j < 3; ++j) {
			cout << " " << materials[i].Ka[j];
		}
		cout << endl;
		cout << "        Kd";
		for (int j = 0; j < 3; ++j) {
			cout << " " << materials[i].Kd[j];
		}
		cout << endl;
		cout << "        Ks";
		for (int j = 0; j < 3; ++j) {
			cout << " " << materials[i].Ks[j];
		}
		cout << endl;
		cout << "        Ni " << materials[i].Ni << endl;
		cout << "        illum " << materials[i].illum << endl;
		cout << "        map_Kd " << materials[i].textureID << endl;
		cout << endl;
	}
}

void mesh::render() const {
	for (size_t i = 0; i < faces.size(); ++i) {
		// set the material to the face
		bool textenable = false;
		// find the material color of the face
		int M = -1;
		for (size_t j = 0; j < materials.size(); ++j) {
			if (materials[j].ID == faces[i].M_ID) {
				M = j;
			}
		}
		// set the material so that the lighting works
		if (M != -1) {
			glMaterialfv(GL_FRONT,GL_DIFFUSE,  &(materials[M].Kd[0]));
			glMaterialfv(GL_FRONT,GL_AMBIENT,  &(materials[M].Ka[0]));
			glMaterialfv(GL_FRONT,GL_SPECULAR, &(materials[M].Ks[0]));
			glMaterialf(GL_FRONT,GL_SHININESS, materials[M].Ns);

			if (materials[M].textureID == -1) {
				glDisable(GL_TEXTURE_2D);
			}
			else {
				textenable = true;
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, materials[M].textureID);
			}
		}

		// build the face
		if (faces[i].is_quad) {
			glBegin(GL_QUADS);
			for (int j = 0; j < 4; ++j) {
				if (textenable) {
					const vec2& uv = textures_coords[faces[i].text_coord[j]];
					glTexCoord2f(uv.x, 1.0 - uv.y);
				}

				int vrtx_idx = faces[i].vertex_index[j];

				const vec3& n = normals[ faces[i].normal_index[j] ];
				glNormal3f(n.x, n.y, n.z);

				const vec3& v = vertices[vrtx_idx];
				glVertex3f(v.x, v.y, v.z);
			}
			glEnd();
		}
		else {
			glBegin(GL_TRIANGLES);
			for (int j = 0; j < 3; ++j) {
				if (textenable) {
					const vec2& uv = textures_coords[faces[i].text_coord[j]];
					glTexCoord2f(uv.x, 1.0 - uv.y);
				}

				int vrtx_idx = faces[i].vertex_index[j];

				const vec3& n = normals[ faces[i].normal_index[j] ];
				glNormal3f(n.x, n.y, n.z);

				const vec3& v = vertices[vrtx_idx];
				glVertex3f(v.x, v.y, v.z);
			}
			glEnd();
		}
	}
}
