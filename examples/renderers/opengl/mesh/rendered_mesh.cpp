#include "rendered_mesh.hpp"

// PRIVATE

// PUBLIC

rendered_mesh::rendered_mesh() : mesh() {
}

rendered_mesh::~rendered_mesh() {
}

// SETTERS

void rendered_mesh::set_material_ids(const vector<string>& material_ids) {
	mat_ids = material_ids;
}

void rendered_mesh::set_materials(const vector<material>& mats) {
	materials = mats;
}

void rendered_mesh::set_texture_coords(const vector<vec2>& texts) {
	texture_coords = texts;
}

void rendered_mesh::set_texture_idxs(const vector<int>& text_idxs) {
	texture_idxs = text_idxs;
}

void rendered_mesh::set_textures_indices(const vector<unsigned int>& text_idxs) {
	textures_indexes = text_idxs;
}

// GETTERS

mesh_state rendered_mesh::state() const {
	mesh_state s = mesh::state();
	if (s != mesh_state::correct) {
		return s;
	}

	// check that indexes are correct.
	for (size_t t = 0; t < triangles.size(); ++t) {
		if (texture_idxs[t] != -1 and texture_idxs[t] > texture_coords.size()) {
			cerr << "mesh::is_valid: Error:" << endl;
			cerr << "    Triangle " << t/3 << " has " << t%3 << "-th "
				 << "texture index (" << texture_idxs[t]
				 << ") out of bounds." << endl;
			return mesh_state::texture_coord_idx_ob;
		}
		if (t%3 == 0) {
			const string& m_id = mat_ids[t/3];
			bool found = false;
			for (const material& mat : materials) {
				if (mat.ID == m_id) {
					found = true;
				}
			}
			if (not found) {
				cerr << "mesh::is_valid: Error:" << endl;
				cerr << "    Triangle " << t/3 << " has a material assigned "
					 << "that was not loaded" << endl;
				return mesh_state::material_not_found;
			}
		}
	}

	return mesh_state::correct;
}

// MODIFIERS

void rendered_mesh::clear() {
	mesh::clear();

	if (textures_indexes.size() > 0) {
		vector<unsigned int>::iterator it;
		for (it = textures_indexes.begin(); it != textures_indexes.end(); ++it) {
			glDeleteTextures(TEXTURE_START, &(*it));
		}
	}

	mat_ids.clear();
	materials.clear();
	texture_coords.clear();
	texture_idxs.clear();
	textures_indexes.clear();
}

// OTHERS

void rendered_mesh::display_mesh_info() {
	mesh::display_mesh_info();
	cout << "    # Materials= " << materials.size() << endl;
	cout << "    # Texture coordinates= " << texture_coords.size() << endl;
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

void rendered_mesh::slow_render() const {
	for (size_t t = 0; t < triangles.size(); t += 3) {
		// set the material of the face
		bool textenable = false;
		// find the material color of the face
		int M = -1;
		for (int i = 0; i < materials.size(); ++i) {
			if (materials[i].ID == mat_ids[t/3]) {
				M = i;
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

		glBegin(GL_TRIANGLES);
		for (int i = t; i < t + 3; ++i) {
			if (textenable) {
				const vec2& uv = texture_coords[ texture_idxs[i] ];
				glTexCoord2f(uv.x, 1.0 - uv.y);
			}

			int vrtx_idx = triangles[i];

			const vec3& n = normals[ normal_idxs[i] ];
			glNormal3f(n.x, n.y, n.z);

			const vec3& v = vertices[vrtx_idx];
			glVertex3f(v.x, v.y, v.z);
		}
		glEnd();
	}
}
