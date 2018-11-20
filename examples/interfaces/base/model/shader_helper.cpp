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

	static inline
	void set_mat_shader(const material& mat, const string& name, shader& S) {
		S.set_vec3(name + ".ambient", to_vec3(mat.Ka));
		S.set_vec3(name + ".diffuse", to_vec3(mat.Kd));
		S.set_vec3(name + ".specular", to_vec3(mat.Ks));
		S.set_float(name + ".shininess", mat.Ni);
	}

	void set_materials_shader(const rendered_model& M, shader& S) {
		const set<size_t>& unique_mat_idxs = M.get_unique_material_idxs();
		size_t inf = unique_mat_idxs.size() + 1;

		if (unique_mat_idxs.size() > 4) {
			cerr << "shader_helper::set_materials_shaders - Error:" << endl;
			cerr << "    Too many materials (" << unique_mat_idxs.size() << ")." << endl;
			return;
		}

		size_t idxs[4] = {inf, inf, inf, inf};
		auto it = unique_mat_idxs.begin();
		for (size_t i = 0; i < 4; ++i, ++it) {
			idxs[i] = *it;
		}

		const vector<material>& all_mats = M.get_materials();

		if (idxs[0] != inf) {
			set_mat_shader(all_mats[idxs[0]], "material[0]", S);
		}
		if (idxs[1] != inf) {
			set_mat_shader(all_mats[idxs[1]], "material[1]", S);
		}
		if (idxs[2] != inf) {
			set_mat_shader(all_mats[idxs[2]], "material[2]", S);
		}
		if (idxs[3] != inf) {
			set_mat_shader(all_mats[idxs[3]], "material[3]", S);
		}

	}

} // -- namespace shader_helper
