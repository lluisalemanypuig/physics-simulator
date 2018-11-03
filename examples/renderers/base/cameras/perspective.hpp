#pragma once

#include <base/cameras/observer.hpp>
#include <base/box.hpp>
using namespace std;

class perspective : public observer {
	private:
		float raw;
		float d;

		// angles (radians)
		float alpha;
		float initial_alpha;

	public:
		perspective();
		~perspective();

		void init_camera(const box& b);
		void make_Zs(const box& b);

		void adapt_camera_to_viewport(int w, int h);

		// a: angle (degrees)
		void set_zoom(float a);
		void zoom(float a);
		void reset_zoom();

		float get_zoom() const;

		// retorna la distancia de la posicio de la camera al centre de l'escena
		float getD() const;

		float getRAw() const;
		// returns the Field Of View in degrees
		float get_FOV() const;
		float get_window_height() const;
		float get_window_width() const;
};
