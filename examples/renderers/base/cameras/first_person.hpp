#pragma once

// physim includes
#include <physim/math/vec3.hpp>
typedef physim::math::vec3 vec3;

// base includes
#include <base/include_gl.hpp>

class first_person {
	private:
		vec3 UP, centre;
		float midaXOriginal;

		vec3 OBS, VRP, OBS_taf, VRP_taf;
		void traslladaCamera(const vec3& vector_trasllat);

	public:
		first_person();
		first_person(const vec3& obs, const vec3& vrp, const vec3& up);
		~first_person();

		void giraCamera(float angle);

		void setOBS(const vec3& obs);
		void setVRP(const vec3& vrp);
		void setOBS_taf(const vec3& obs);
		void setVRP_taf(const vec3& vrp);
		void setUP(const vec3& up);
		void setCentreOriginal(const vec3& c);
		void setMidaXOriginal(float c);

		vec3 avancaObjecte(float t);
		vec3 desplacaObjecte(float t);

		void setTafanerOrigen();
		void dibuixaLiniaFromObsToVRP() const;
		void dibuixaLiniaFromObs_TAF_ToVRP_TAF() const;

		vec3 getOBS() const;
		vec3 getVRP() const;
		vec3 getOBS_taf() const;
		vec3 getVRP_taf() const;

		vec3 getUP() const;
		vec3 getCentreOriginal() const;
		float getMidaXOriginal() const;
};
