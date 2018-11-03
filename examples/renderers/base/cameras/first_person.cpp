#include <base/cameras/first_person.hpp>

// C++ includes
#include <cmath>

static inline
vec3 giraPuntXZ(float angle, const vec3& punt, const vec3& centre) {
	float x = punt.x - centre.x;
	float z = punt.z - centre.z;
	return vec3(x*std::cos(-angle) - z*std::sin(-angle) + centre.x,
			 punt.y,
			 x*std::sin(-angle) + z*std::cos(-angle) + centre.z);
}

void first_person::traslladaCamera(const vec3& vector_trasllat) {
	OBS += vector_trasllat;
	VRP += vector_trasllat;
	OBS_taf += vector_trasllat;
	VRP_taf += vector_trasllat;
	centre += vector_trasllat;
}

first_person::first_person() {}
first_person::first_person(const vec3& obs, const vec3& vrp, const vec3& up) : OBS(obs), VRP(vrp), UP(up) {}
first_person::~first_person() {}

void first_person::giraCamera(float angle) {
	OBS = giraPuntXZ(angle, OBS, centre);
	VRP = giraPuntXZ(angle, VRP, centre);
	OBS_taf = giraPuntXZ(angle, OBS_taf, centre);
	VRP_taf = giraPuntXZ(angle, VRP_taf, centre);
}

void first_person::setOBS(const vec3& obs) {OBS = obs;}
void first_person::setVRP(const vec3& vrp) {VRP = vrp;}
void first_person::setOBS_taf(const vec3& obs) {OBS_taf = obs;}
void first_person::setVRP_taf(const vec3& vrp) {VRP_taf = vrp;}
void first_person::setUP(const vec3& up) {UP = up;}
void first_person::setCentreOriginal(const vec3& c) {centre = c;}
void first_person::setMidaXOriginal(float c) {midaXOriginal = c;}

vec3 first_person::avancaObjecte(float t) {
	vec3 vector_moviment = VRP - OBS;
	vec3 vector_normalit = physim::math::normalise(vector_moviment);
	vec3 vector_trasllat = vector_normalit*t;
	traslladaCamera(vector_trasllat);
	return vector_trasllat;
}

vec3 first_person::desplacaObjecte(float t) {
	vec3 VRPp = giraPuntXZ(90.0, VRP, centre);
	vec3 OBSp = giraPuntXZ(90.0, OBS, centre);
	vec3 vector_normalit_perpendi = physim::math::normalise(VRPp - OBSp);
	vec3 vector_trasllat = vector_normalit_perpendi*t;
	traslladaCamera(vector_trasllat);
	return vector_trasllat;
}

void first_person::setTafanerOrigen() {
	OBS_taf = OBS;
	VRP_taf = VRP;
}

void first_person::dibuixaLiniaFromObsToVRP() const {
	glBegin(GL_LINES);
		glVertex3f(OBS.x,OBS.y,OBS.z);
		glVertex3f(VRP.x,VRP.y,VRP.z);
	glEnd();
}

void first_person::dibuixaLiniaFromObs_TAF_ToVRP_TAF() const {
	glBegin(GL_LINES);
		glVertex3f(OBS_taf.x,OBS_taf.y,OBS_taf.z);
		glVertex3f(VRP_taf.x,VRP_taf.y,VRP_taf.z);
	glEnd();
}

vec3 first_person::getOBS() const {return OBS;}
vec3 first_person::getVRP() const {return VRP;}

vec3 first_person::getOBS_taf() const {return OBS_taf;}
vec3 first_person::getVRP_taf() const {return VRP_taf;}

vec3 first_person::getUP() const {return UP;}
vec3 first_person::getCentreOriginal() const {return centre;}
float first_person::getMidaXOriginal() const {return midaXOriginal;}
