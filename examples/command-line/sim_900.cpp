// C includes
#include <string.h>

// C++ includes
#include <iostream>
#include <fstream>
using namespace std;

// Custom includes
#include "utils.hpp"

// physim includes
#include <physim/structures/octree.hpp>
#include <physim/math/vec3.hpp>
using namespace physim;
using namespace math;
using namespace structures;

class big {
	public:
		big() {}

		float a, b, c;
		char K;
		vec3 p1,p2,p3;
		char q;
		vec3 p4;
};

class bigger : public big {
	public:
		bigger() : big() {}

		float l;
		bool B;
		float m, n;
		vec3 p5,p6,p7,p8,p9;
		float i, j, k;
};

namespace study_cases {

	void sim_900_usage() {
		cout << "Memory testing:" << endl;
		cout << endl;
		cout << "Using vec3 inside a 'random' struct, fills an octree." << endl;
		cout << "Intended for memory leak testing." << endl;
		cout << endl;
	}

	void sim_900_test_vectors() {
		vector<big> v1(512);
		for (size_t i = 0; i < 512; ++i) {
			v1[i].p1 = vec3( i*12 + 0.0f, i*12 +  1.0f, i*12 +  2.0f );
			v1[i].p2 = vec3( i*12 + 3.0f, i*12 +  4.0f, i*12 +  5.0f );
			v1[i].p3 = vec3( i*12 + 6.0f, i*12 +  7.0f, i*12 +  8.0f );
			v1[i].p4 = vec3( i*12 + 9.0f, i*12 + 10.0f, i*12 + 11.0f );
		}

		octree tree;

		cout << "Filling from vectors:" << endl;

		cout << "    1" << endl;
		tree.init(&v1[0], 512, 3, sizeof(big), offsetof(big, p1), 8);
		tree.clear();

		cout << "    2" << endl;
		tree.init(&v1[0], 512, 1, sizeof(big), offsetof(big, p4), 8);
		tree.clear();

		cout << "    3" << endl;
		tree.init(&v1[256], 256, 3, sizeof(big), offsetof(big, p1), 8);
		tree.clear();

		cout << "    4" << endl;
		tree.init(&v1[256], 256, 1, sizeof(big), offsetof(big, p4), 8);
		tree.clear();

		vector<bigger> v2(512);
		for (size_t i = 0; i < 512; ++i) {
			v2[i].p1 = vec3( i*27 +  0.0f, i*12 +  1.0f, i*12 +  2.0f );
			v2[i].p2 = vec3( i*27 +  3.0f, i*12 +  4.0f, i*12 +  5.0f );
			v2[i].p3 = vec3( i*27 +  6.0f, i*12 +  7.0f, i*12 +  8.0f );
			v2[i].p4 = vec3( i*27 +  9.0f, i*12 + 10.0f, i*12 + 11.0f );
			v2[i].p5 = vec3( i*27 + 12.0f, i*12 + 13.0f, i*12 + 14.0f );
			v2[i].p6 = vec3( i*27 + 15.0f, i*12 + 16.0f, i*12 + 17.0f );
			v2[i].p7 = vec3( i*27 + 18.0f, i*12 + 19.0f, i*12 + 20.0f );
			v2[i].p8 = vec3( i*27 + 21.0f, i*12 + 22.0f, i*12 + 23.0f );
			v2[i].p9 = vec3( i*27 + 24.0f, i*12 + 25.0f, i*12 + 26.0f );
		}

		size_t base = reinterpret_cast<size_t>( &v2[0] );
		size_t member_p1 = reinterpret_cast<size_t>( &v2[0].p1 );
		size_t offset_p1 = member_p1 - base;
		size_t member_p5 = reinterpret_cast<size_t>( &v2[0].p5 );
		size_t offset_p5 = member_p5 - base;

		cout << "    5" << endl;
		tree.init(&v2[0], 512, 3, sizeof(bigger), offset_p1, 8);
		tree.clear();

		cout << "    6" << endl;
		tree.init(&v2[0], 512, 5, sizeof(bigger), offset_p5, 8);
		tree.clear();

		cout << "    7" << endl;
		tree.init(&v2[256], 256, 3, sizeof(bigger), offset_p1, 8);
		tree.clear();

		cout << "    8" << endl;
		tree.init(&v2[256], 256, 5, sizeof(bigger), offset_p5, 8);
		tree.clear();
	}

	void sim_900_test_arrays() {
		big v[512];
		for (size_t i = 0; i < 512; ++i) {
			v[i].p1 = vec3( i*12 + 0.0f, i*12 +  1.0f, i*12 +  2.0f );
			v[i].p2 = vec3( i*12 + 3.0f, i*12 +  4.0f, i*12 +  5.0f );
			v[i].p3 = vec3( i*12 + 6.0f, i*12 +  7.0f, i*12 +  8.0f );
			v[i].p4 = vec3( i*12 + 9.0f, i*12 + 10.0f, i*12 + 11.0f );
		}

		octree tree;

		cout << "Filling from arrays:" << endl;

		cout << "    1" << endl;
		tree.init(&v[0], 512, 3, sizeof(big), offsetof(big, p1), 8);
		tree.clear();

		cout << "    2" << endl;
		tree.init(&v[0], 512, 1, sizeof(big), offsetof(big, p4), 8);
		tree.clear();

		cout << "    3" << endl;
		tree.init(&v[256], 256, 3, sizeof(big), offsetof(big, p1), 8);
		tree.clear();

		cout << "    4" << endl;
		tree.init(&v[256], 256, 1, sizeof(big), offsetof(big, p4), 8);
		tree.clear();

		bigger v2[512];
		for (size_t i = 0; i < 512; ++i) {
			v2[i].p1 = vec3( i*27 +  0.0f, i*12 +  1.0f, i*12 +  2.0f );
			v2[i].p2 = vec3( i*27 +  3.0f, i*12 +  4.0f, i*12 +  5.0f );
			v2[i].p3 = vec3( i*27 +  6.0f, i*12 +  7.0f, i*12 +  8.0f );
			v2[i].p4 = vec3( i*27 +  9.0f, i*12 + 10.0f, i*12 + 11.0f );
			v2[i].p5 = vec3( i*27 + 12.0f, i*12 + 13.0f, i*12 + 14.0f );
			v2[i].p6 = vec3( i*27 + 15.0f, i*12 + 16.0f, i*12 + 17.0f );
			v2[i].p7 = vec3( i*27 + 18.0f, i*12 + 19.0f, i*12 + 20.0f );
			v2[i].p8 = vec3( i*27 + 21.0f, i*12 + 22.0f, i*12 + 23.0f );
			v2[i].p9 = vec3( i*27 + 24.0f, i*12 + 25.0f, i*12 + 26.0f );
		}

		size_t base = reinterpret_cast<size_t>( &v2[0] );
		size_t member_p1 = reinterpret_cast<size_t>( &v2[0].p1 );
		size_t offset_p1 = member_p1 - base;
		size_t member_p5 = reinterpret_cast<size_t>( &v2[0].p5 );
		size_t offset_p5 = member_p5 - base;

		cout << "    5" << endl;
		tree.init(&v2[0], 512, 3, sizeof(bigger), offset_p1, 8);
		tree.clear();

		cout << "    6" << endl;
		tree.init(&v2[0], 512, 5, sizeof(bigger), offset_p5, 8);
		tree.clear();

		cout << "    7" << endl;
		tree.init(&v2[256], 256, 3, sizeof(bigger), offset_p1, 8);
		tree.clear();

		cout << "    8" << endl;
		tree.init(&v2[256], 256, 5, sizeof(bigger), offset_p5, 8);
		tree.clear();
	}

	void sim_900(int argc, char *argv[]) {
		for (int i = 2; i < argc; ++i) {
			if (strcmp(argv[i], "-h") == 0 or strcmp(argv[i], "--help") == 0) {
				sim_900_usage();
				return;
			}
			else {
				cerr << "Error: unknown option '" << string(argv[i]) << "'" << endl;
				cerr << "    Use ./command-line 900 --help to see the usage" << endl;
				return;
			}
		}

		timing::time_point begin, end;

		begin = timing::now();
		sim_900_test_vectors();
		end = timing::now();
		cout << "Tested with vectors in "
			 << timing::elapsed_seconds(begin, end) << " seconds" << endl;

		begin = timing::now();
		sim_900_test_arrays();
		end = timing::now();
		cout << "Tested with arrays in "
			 << timing::elapsed_seconds(begin, end) << " seconds" << endl;


	}

} // -- namespace study_cases

