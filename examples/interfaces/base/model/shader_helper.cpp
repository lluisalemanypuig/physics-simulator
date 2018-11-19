#include <base/model/shader_helper.hpp>

// C++ includes
#include <iostream>
#include <string>
#include <set>
using namespace std;

// base includes
#include <base/model/model_utils.hpp>

namespace shader_helper {

	static inline
	glm::vec3 to_vec3(const glm::vec4& v) {
		return glm::vec3(v[0], v[1], v[2]);
	}

	void set_materials_shader(const rendered_model& M, shader& S) {
		const vector<size_t>& mat_idxs = M.get_material_idxs();
		set<size_t> unique_mat_idxs(mat_idxs.begin(), mat_idxs.end());

		if (unique_mat_idxs.size() >= 4) {
			cerr << "shader_helper::set_materials_shaders - Error:" << endl;
			cerr << "    Too many materials (" << unique_mat_idxs.size() << ")." << endl;
			return;
		}

		size_t idxs[4] = {mat_idxs.size(), mat_idxs.size(), mat_idxs.size(), mat_idxs.size()};
		auto it = unique_mat_idxs.begin();
		for (size_t i = 0; i < 4; ++i, ++it) {
			idxs[i] = *it;
		}

		const vector<material>& all_mats = M.get_materials();

		if (idxs[0] != mat_idxs.size()) {
			S.set_vec3("material0.ambient", to_vec3(all_mats[idxs[0]].Ka));
			S.set_vec3("material0.diffuse", to_vec3(all_mats[idxs[0]].Kd));
			S.set_vec3("material0.specular", to_vec3(all_mats[idxs[0]].Ks));
			S.set_float("material0.shininess", all_mats[idxs[0]].Ni);
		}
		if (idxs[1] != mat_idxs.size()) {
			S.set_vec3("material1.ambient", to_vec3(all_mats[idxs[1]].Ka));
			S.set_vec3("material1.diffuse", to_vec3(all_mats[idxs[1]].Kd));
			S.set_vec3("material1.specular", to_vec3(all_mats[idxs[1]].Ks));
			S.set_float("material1.shininess", all_mats[idxs[1]].Ni);
		}
		if (idxs[2] != mat_idxs.size()) {
			S.set_vec3("material2.ambient", to_vec3(all_mats[idxs[2]].Ka));
			S.set_vec3("material2.diffuse", to_vec3(all_mats[idxs[2]].Kd));
			S.set_vec3("material2.specular", to_vec3(all_mats[idxs[2]].Ks));
			S.set_float("material2.shininess", all_mats[idxs[2]].Ni);
		}
		if (idxs[3] != mat_idxs.size()) {
			S.set_vec3("material3.ambient", to_vec3(all_mats[idxs[3]].Ka));
			S.set_vec3("material3.diffuse", to_vec3(all_mats[idxs[3]].Kd));
			S.set_vec3("material3.specular", to_vec3(all_mats[idxs[3]].Ks));
			S.set_float("material3.shininess", all_mats[idxs[3]].Ni);
		}

	}

} // -- namespace shader_helper
