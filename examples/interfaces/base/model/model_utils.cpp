#include <base/model/model_utils.hpp>

material::material
(
	const vec3& amb,
	const vec3& dif,
	const vec3& spec,
	float ns, float ni,float D,
	int ill, int textID, const std::string& id
)
{
	ID = id;
	Ns = ns;
	Ni = ni;
	d =  D;
	illum = ill;
	textureID = textID;
	Ka[0] = amb.x; Ka[1] = amb.y; Ka[2] = amb.z; Ka[3] = D;
	Kd[0] = dif.x; Kd[1] = dif.y; Kd[2] = dif.z; Kd[3] = D;
	Ks[0] = spec.x; Ks[1] = spec.y; Ks[2] = spec.z; Ks[3] = D;
}

material::material(const material& m) {
	ID = m.ID;
	Ns = m.Ns;
	Ni = m.Ni;
	d =  m.d;
	illum = m.illum;
	textureID = m.textureID;
	for (int i = 0; i < 4; ++i) {
		Ka[i] = m.Ka[i];
		Kd[i] = m.Kd[i];
		Ks[i] = m.Ks[i];
	}
}
