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

#define N 4

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
		vector<big> v1(N);
		for (size_t i = 0; i < N; ++i) {
			v1[i].p1 = vec3( i*12 + 0.0f, i*12 +  1.0f, i*12 +  2.0f );
			v1[i].p2 = vec3( i*12 + 3.0f, i*12 +  4.0f, i*12 +  5.0f );
			v1[i].p3 = vec3( i*12 + 6.0f, i*12 +  7.0f, i*12 +  8.0f );
			v1[i].p4 = vec3( i*12 + 9.0f, i*12 + 10.0f, i*12 + 11.0f );
		}

		octree tree;

		cout << "Filling from vectors:" << endl;

		cout << "    TEST 1" << endl;
		tree.init(&v1[0], N, 3, sizeof(big), offsetof(big, p1), 8);
		tree.clear();

		cout << "    TEST 2" << endl;
		tree.init(&v1[0], N, 1, sizeof(big), offsetof(big, p4), 8);
		tree.clear();

		cout << "    TEST 3" << endl;
		tree.init(&v1[0], N/2, 3, sizeof(big), offsetof(big, p1), 8);
		tree.clear();

		cout << "    TEST 4" << endl;
		tree.init(&v1[0], N/2, 1, sizeof(big), offsetof(big, p4), 8);
		tree.clear();

		cout << "    TEST 5" << endl;
		tree.init(&v1[N/2], N/2, 3, sizeof(big), offsetof(big, p1), 8);
		tree.clear();

		cout << "    TEST 6" << endl;
		tree.init(&v1[N/2], N/2, 1, sizeof(big), offsetof(big, p4), 8);
		tree.clear();

		vector<bigger> v2(N);
		for (size_t i = 0; i < N; ++i) {
			v2[i].p1 = vec3( i*27 +  0.0f, i*27 +  1.0f, i*27 +  2.0f );
			v2[i].p2 = vec3( i*27 +  3.0f, i*27 +  4.0f, i*27 +  5.0f );
			v2[i].p3 = vec3( i*27 +  6.0f, i*27 +  7.0f, i*27 +  8.0f );
			v2[i].p4 = vec3( i*27 +  9.0f, i*27 + 10.0f, i*27 + 11.0f );
			v2[i].p5 = vec3( i*27 + 12.0f, i*27 + 13.0f, i*27 + 14.0f );
			v2[i].p6 = vec3( i*27 + 15.0f, i*27 + 16.0f, i*27 + 17.0f );
			v2[i].p7 = vec3( i*27 + 18.0f, i*27 + 19.0f, i*27 + 20.0f );
			v2[i].p8 = vec3( i*27 + 21.0f, i*27 + 22.0f, i*27 + 23.0f );
			v2[i].p9 = vec3( i*27 + 24.0f, i*27 + 25.0f, i*27 + 26.0f );
		}

		size_t base = reinterpret_cast<size_t>( &v2[0] );
		size_t member_p1 = reinterpret_cast<size_t>( &v2[0].p1 );
		size_t offset_p1 = member_p1 - base;
		size_t member_p5 = reinterpret_cast<size_t>( &v2[0].p5 );
		size_t offset_p5 = member_p5 - base;

		cout << "    TEST 7" << endl;
		tree.init(&v2[0], N, 3, sizeof(bigger), offset_p1, 8);
		tree.clear();

		cout << "    TEST 8" << endl;
		tree.init(&v2[0], N, 5, sizeof(bigger), offset_p5, 8);
		tree.clear();

		cout << "    TEST 9" << endl;
		tree.init(&v2[0], N/2, 3, sizeof(bigger), offset_p1, 8);
		tree.clear();

		cout << "    TEST 10" << endl;
		tree.init(&v2[0], N/2, 5, sizeof(bigger), offset_p5, 8);
		tree.clear();

		cout << "    TEST 11" << endl;
		tree.init(&v2[N/2], N/2, 3, sizeof(bigger), offset_p1, 8);
		tree.clear();

		cout << "    TEST 12" << endl;
		tree.init(&v2[N/2], N/2, 5, sizeof(bigger), offset_p5, 8);
		tree.clear();
	}

	void sim_900_test_arrays() {
		big v[N];
		for (size_t i = 0; i < N; ++i) {
			v[i].p1 = vec3( i*12 + 0.0f, i*12 +  1.0f, i*12 +  2.0f );
			v[i].p2 = vec3( i*12 + 3.0f, i*12 +  4.0f, i*12 +  5.0f );
			v[i].p3 = vec3( i*12 + 6.0f, i*12 +  7.0f, i*12 +  8.0f );
			v[i].p4 = vec3( i*12 + 9.0f, i*12 + 10.0f, i*12 + 11.0f );
		}

		octree tree;

		cout << "Filling from arrays:" << endl;

		cout << "    TEST 1" << endl;
		tree.init(&v[0], N, 3, sizeof(big), offsetof(big, p1), 8);
		tree.clear();

		cout << "    TEST 2" << endl;
		tree.init(&v[0], N, 1, sizeof(big), offsetof(big, p4), 8);
		tree.clear();

		cout << "    TEST 3" << endl;
		tree.init(&v[0], N/2, 3, sizeof(big), offsetof(big, p1), 8);
		tree.clear();

		cout << "    TEST 4" << endl;
		tree.init(&v[0], N/2, 1, sizeof(big), offsetof(big, p4), 8);
		tree.clear();

		cout << "    TEST 5" << endl;
		tree.init(&v[N/2], N/2, 3, sizeof(big), offsetof(big, p1), 8);
		tree.clear();

		cout << "    TEST 6" << endl;
		tree.init(&v[N/2], N/2, 1, sizeof(big), offsetof(big, p4), 8);
		tree.clear();

		bigger v2[N];
		for (size_t i = 0; i < N; ++i) {
			v2[i].p1 = vec3( i*27 +  0.0f, i*27 +  1.0f, i*27 +  2.0f );
			v2[i].p2 = vec3( i*27 +  3.0f, i*27 +  4.0f, i*27 +  5.0f );
			v2[i].p3 = vec3( i*27 +  6.0f, i*27 +  7.0f, i*27 +  8.0f );
			v2[i].p4 = vec3( i*27 +  9.0f, i*27 + 10.0f, i*27 + 11.0f );
			v2[i].p5 = vec3( i*27 + 12.0f, i*27 + 13.0f, i*27 + 14.0f );
			v2[i].p6 = vec3( i*27 + 15.0f, i*27 + 16.0f, i*27 + 17.0f );
			v2[i].p7 = vec3( i*27 + 18.0f, i*27 + 19.0f, i*27 + 20.0f );
			v2[i].p8 = vec3( i*27 + 21.0f, i*27 + 22.0f, i*27 + 23.0f );
			v2[i].p9 = vec3( i*27 + 24.0f, i*27 + 25.0f, i*27 + 26.0f );
		}

		size_t base = reinterpret_cast<size_t>( &v2[0] );
		size_t member_p1 = reinterpret_cast<size_t>( &v2[0].p1 );
		size_t offset_p1 = member_p1 - base;
		size_t member_p5 = reinterpret_cast<size_t>( &v2[0].p5 );
		size_t offset_p5 = member_p5 - base;

		cout << "    TEST 7" << endl;
		tree.init(&v2[0], N, 3, sizeof(bigger), offset_p1, 8);
		tree.clear();

		cout << "    TEST 8" << endl;
		tree.init(&v2[0], N, 5, sizeof(bigger), offset_p5, 8);
		tree.clear();

		cout << "    TEST 9" << endl;
		tree.init(&v2[0], N/2, 3, sizeof(bigger), offset_p1, 8);
		tree.clear();

		cout << "    TEST 10" << endl;
		tree.init(&v2[0], N/2, 5, sizeof(bigger), offset_p5, 8);
		tree.clear();

		cout << "    TEST 11" << endl;
		tree.init(&v2[N/2], N/2, 3, sizeof(bigger), offset_p1, 8);
		tree.clear();

		cout << "    TEST 12" << endl;
		tree.init(&v2[N/2], N/2, 5, sizeof(bigger), offset_p5, 8);
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
		sim_900_test_arrays();
		end = timing::now();
		cout << "Tested with arrays in "
			 << timing::elapsed_seconds(begin, end) << " seconds" << endl;

		begin = timing::now();
		sim_900_test_vectors();
		end = timing::now();
		cout << "Tested with vectors in "
			 << timing::elapsed_seconds(begin, end) << " seconds" << endl;
	}

} // -- namespace study_cases

