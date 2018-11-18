#include <base/model/rendered_model.hpp>

// C++ includes
#include <iostream>
using namespace std;

// base includes
#include <base/include_gl.hpp>
#include <base/textures/texture_loader.hpp>

// PRIVATE

// PUBLIC

rendered_model::rendered_model() : model() {
	list_index = 0;
}

rendered_model::rendered_model(const rendered_model& m) : model(m) {
	materials = m.materials;
	mat_ids = m.mat_ids;
	texture_coords = m.texture_coords;
	texture_idxs = m.texture_idxs;
}

rendered_model::~rendered_model() {
	clear();
}

// SETTERS

void rendered_model::set_material_ids(const vector<string>& material_ids) {
	mat_ids = material_ids;
}

void rendered_model::set_materials(const vector<material>& mats) {
	materials = mats;
}

void rendered_model::set_texture_coords(const vector<vec2>& texts) {
	texture_coords = texts;
}

void rendered_model::set_texture_idxs(const vector<int>& text_idxs) {
	texture_idxs = text_idxs;
}

// GETTERS

mesh_state rendered_model::state(const mesh_state& ignore) const {
	mesh_state s = model::state(ignore);
	if (s != mesh_state::correct) {
		return s;
	}

	// check that indexes are correct.
	for (size_t t = 0; t < triangles.size(); ++t) {
		if ((ignore & mesh_state::texture_coord_idx_ob) == 0) {
			if (texture_idxs[t] != -1 and texture_idxs[t] > texture_coords.size()) {
				cerr << "mesh::is_valid: Error:" << endl;
				cerr << "    Triangle " << t/3 << " has " << t%3 << "-th "
					 << "texture index (" << texture_idxs[t]
					 << ") out of bounds." << endl;
				return mesh_state::texture_coord_idx_ob;
			}
		}
		if (t%3 == 0) {
			const string& m_id = mat_ids[t/3];
			bool found = false;
			for (const material& mat : materials) {
				if (mat.id == m_id) {
					found = true;
				}
			}
			if ((ignore & mesh_state::material_not_found) == 0 and not found) {
				cerr << "mesh::is_valid: Error:" << endl;
				cerr << "    Triangle " << t/3 << " has a material assigned "
					 << "that was not loaded" << endl;
				return mesh_state::material_not_found;
			}
		}
	}

	return mesh_state::correct;
}

void rendered_model::load_textures() {
	texture_loader& load = texture_loader::get_loader();
	load.load_textures(materials);
}

// MODIFIERS

void rendered_model::clear() {
	model::clear();

	if (list_index >= 1) {
		glDeleteLists(list_index, 1);
	}

	mat_ids.clear();
	materials.clear();
	texture_coords.clear();
	texture_idxs.clear();
}

// OTHERS

void rendered_model::display_mesh_info() const {
	model::display_mesh_info();
	cout << "    # Materials= " << materials.size() << endl;
	cout << "    # Texture coordinates= " << texture_coords.size() << endl;
	uint ntxts = 0;
	for (const material& m : materials) {
		if (m.txt_id != 0) {
			++ntxts;
		}
	}
	cout << "    # Textures: " << ntxts << endl;

	cout << "    Materials: " << endl;
	for (size_t i = 0; i < materials.size(); ++i) {
		cout << "        newmtl " << materials[i].id << endl;
		cout << "        Ns " << materials[i].Ns;
		cout << endl;
		cout << "        Ka" << materials[i].Ka.x << ","
							 << materials[i].Ka.y << ","
							 << materials[i].Ka.z << ","
							 << materials[i].Ka.u;
		cout << endl;
		cout << "        Kd" << materials[i].Kd.x << ","
							 << materials[i].Kd.y << ","
							 << materials[i].Kd.z << ","
							 << materials[i].Kd.u;
		cout << endl;
		cout << "        Ks" << materials[i].Ks.x << ","
							 << materials[i].Ks.y << ","
							 << materials[i].Ks.z << ","
							 << materials[i].Ks.u;
		cout << "        Ni " << materials[i].Ni << endl;
		cout << "        illum " << materials[i].illum << endl;
		cout << "        map_Kd " << materials[i].txt_id << endl;
		cout << endl;
	}
}

void rendered_model::slow_render() const {
	for (size_t t = 0; t < triangles.size(); t += 3) {
		// set the material of the face
		bool textenable = false;
		// find the material color of the face
		size_t M = materials.size();
		for (size_t i = 0; i < materials.size(); ++i) {
			if (materials[i].id == mat_ids[t/3]) {
				M = i;
			}
		}
		// set the material so that the lighting works
		if (M < materials.size()) {
			glMaterialfv(GL_FRONT,GL_DIFFUSE,  &(materials[M].Kd.x));
			glMaterialfv(GL_FRONT,GL_AMBIENT,  &(materials[M].Ka.x));
			glMaterialfv(GL_FRONT,GL_SPECULAR, &(materials[M].Ks.x));
			glMaterialf(GL_FRONT,GL_SHININESS, materials[M].Ns);

			if (materials[M].txt_id == 0) {
				glDisable(GL_TEXTURE_2D);
			}
			else {
				textenable = true;
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, materials[M].txt_id);
			}
		}

		glBegin(GL_TRIANGLES);
		for (size_t i = t; i < t + 3; ++i) {
			if (textenable) {
				const vec2& uv = texture_coords[ texture_idxs[i] ];
				glTexCoord2f(uv.x, 1.0 - uv.y);
			}

			int vrtx_idx = triangles[i];

			if (normals.size() > 0) {
				const vec3& n = normals[ normal_idxs[i] ];
				glNormal3f(n.x, n.y, n.z);
			}

			const vec3& v = vertices[vrtx_idx];
			glVertex3f(v.x, v.y, v.z);
		}
		glEnd();
	}
}

uint rendered_model::compile() {
	if (list_index == 0) {
		list_index = glGenLists(1);
		glNewList(list_index, GL_COMPILE);
		slow_render();
		glEndList();
	}

	#if defined (DEBUG)
	cout << "rendered_model::compile:" << endl;
	cout << "    Object compiled into list with index: " << list_index << endl;
	#endif
	return list_index;
}

void rendered_model::render() const {
	if (list_index > 0) {
		glCallList(list_index);
	}
	else {
		slow_render();
	}
}
