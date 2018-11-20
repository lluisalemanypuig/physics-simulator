#include <base/textures/texture_loader.hpp>

// C++ includes
#include <iostream>
#include <set>
using namespace std;

// base includes
#include <base/include_gl.hpp>
#include <base/textures/std_image.hpp>

// PRIVATE

texture_loader::texture_loader() {

}

// PUBLIC

texture_loader::~texture_loader() {
	clear_all();
}

// MODIFIERS

void texture_loader::load_textures(vector<material>& mats, vector<unsigned int>& idxs)
{
	set<unsigned int> unique_texs;
	for (size_t i = 0; i < mats.size(); ++i) {
		material& m = mats[i];

		if (m.txt_name == NULL_TEXTURE_NAME) {
			continue;
		}

		// check if it has been already loaded
		auto it = textures.find(m.txt_name);
		unsigned int id = 0;
		if (it != textures.end()) {
			id = it->second;
		}

		// already loaded or not previously freed
		if (id != 0) {
			unique_texs.insert(id);
			m.txt_id = id;
			continue;
		}

		#if defined (DEBUG)
		cout << "texture_loader::load_textures:" << endl;
		cout << "    About to load image '" << m.txt_name << "'" << endl;
		cout << "    of " << i << "-th material" << endl;
		#endif

		int width, height, n_channels;
		unsigned char *data =
			stbi_load(m.txt_name.c_str(), &width, &height, &n_channels, 0);
		if (data == nullptr) {
			cerr << "texture_loader::load_textures - Error:" << endl;
			cerr << "    Could not load texture '" << m.txt_name << "'" << endl;
			cerr << "    in material '" << m.id << "'" << endl;
		}

		#if defined (DEBUG)
		cout << "    image loaded!" << endl;
		cout << "        dimensions: " << width << "x" << height << endl;
		cout << "        # channels: " << n_channels << endl;
		cout << "    Generating texture... ";
		#endif

		glGenTextures(1, &id);

		#if defined (DEBUG)
		cout << "id: " << id << endl;
		cout << "    glBindTexture...      ";
		#endif

		glBindTexture(GL_TEXTURE_2D, id);

		#if defined (DEBUG)
		cout << "done" << endl;
		cout << "    glTexImage2D...       ";
		#endif

		if (n_channels == 3) {
			glTexImage2D
			(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else if (n_channels == 4) {
			glTexImage2D
			(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		#if defined (DEBUG)
		cout << "done" << endl;
		#endif

		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);

		textures[m.txt_name] = id;
		m.txt_id = id;
		unique_texs.insert(id);
	}

	idxs.clear();
	idxs = vector<unsigned int>( unique_texs.begin(), unique_texs.end() );
}

/**
 * @brief Clears the textures of a collection of materials.
 *
 * Given a collection of materials, either withor without textures,
 * clear all their textures. If a texture was cleared previouly,
 * then it is NOT cleared twice.
 * @param[in] mats Collection of materials.
 */
void texture_loader::clear_textures(const vector<material>& mats) {
	for (const material& m : mats) {
		if (m.txt_name == NULL_TEXTURE_NAME) {
			continue;
		}

		// check if it has been already freed
		auto it = textures.find(m.txt_name);
		unsigned int id = 0;
		if (it != textures.end()) {
			id = it->second;
		}
		// not loaded or previously freed
		if (id == 0) {
			continue;
		}

		glDeleteTextures(1, &id);
		it->second = 0;
	}
}

void texture_loader::clear_all() {
	for (auto& txt_id : textures) {
		if (txt_id.second == 0) {
			continue;
		}
		glDeleteTextures(1, &txt_id.second);
		txt_id.second = 0;
	}
}

// GETTERS

bool texture_loader::texture_index(const string& s, unsigned int& idx) const {
	auto it = textures.find(s);
	if (it == textures.end()) {
		return false;
	}
	idx = it->second;
	return true;
}
