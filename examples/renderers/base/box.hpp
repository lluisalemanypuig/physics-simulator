#pragma once

// C++ includes
#include <fstream>
#include <vector>

// physim includes
#include <physim/math/vec3.hpp>
typedef physim::math::vec3 vec3;

class box {
	private:
		// all the vertices of the box
		// for faster drawing
		vec3 vs[8];
		// Minimum and maximum coordinates
		vec3 min, max;

		vec3 make_vi(int i) const;
		void update_vi();

	public:
		box();
		~box();

		inline friend
		std::ostream& operator<< (std::ostream& os, const box& b) {
			os << "(" << b.min.x << "," << b.min.y << "," << b.min.z << "),"
			   << "(" << b.max.x << "," << b.max.y << "," << b.max.z << ")";
			return os;
		}

		void set_min_max(const vec3& m, const vec3& M);
		void enlarge_box(const vec3& v);
		void enlarge_box(const box& b);
		void translate_box(const vec3& t);

		vec3 get_min() const;
		vec3 get_max() const;

		float length_x() const;
		float length_y() const;
		float length_z() const;

		vec3 get_middle_point() const;
		vec3 get_middle_bottom_point() const;
		vec3 get_middle_front_point() const;
		float get_diag_length() const;
		float get_half_diag_length() const;

		void draw_box() const;
};
