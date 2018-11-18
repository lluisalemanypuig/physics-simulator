#include <base/model/model_utils.hpp>

material::material
(
	const std::string& _id,
	const std::string& _txt_name,
	unsigned int _txt_id,
	const vec3& amb,
	const vec3& dif,
	const vec3& spec,
	float ns, float ni,float D,
	int ill
)
{
	id = _id;
	txt_name = _txt_name;
	txt_id = _txt_id;

	Ns = ns;
	Ni = ni;
	d =  D;
	illum = ill;
	Ka = vec4(amb.x, amb.y, amb.z, D);
	Kd = vec4(dif.x, dif.y, dif.z, D);
	Ks = vec4(spec.x, spec.y, spec.z, D);
}

material::material(const material& m) {
	id = m.id;
	txt_name = m.txt_name;
	txt_id = m.txt_id;

	Ns = m.Ns;
	Ni = m.Ni;
	d =  m.d;
	illum = m.illum;
	Ka = m.Ka;
	Kd = m.Kd;
	Ks = m.Ks;
}
