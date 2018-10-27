#include "mesh_utils.hpp"

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
