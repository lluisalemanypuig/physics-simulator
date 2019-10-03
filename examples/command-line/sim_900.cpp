/*********************************************************************
 * Real-time physics simulation project
 * Copyright (C) 2018-2019 Lluís Alemany Puig
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * Contact: Lluís Alemany Puig (lluis.alemany.puig@gmail.com)
 * 
 ********************************************************************/

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

#define N 1048576
//#define N 4

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
			size_t base = 12*i;
			v1[i].p1 = vec3( base + 0.0f, base +  1.0f, base +  2.0f );
			v1[i].p2 = vec3( base + 3.0f, base +  4.0f, base +  5.0f );
			v1[i].p3 = vec3( base + 6.0f, base +  7.0f, base +  8.0f );
			v1[i].p4 = vec3( base + 9.0f, base + 10.0f, base + 11.0f );
		}

		octree tree;

		cout << "Filling from vectors:" << endl;

		cout << "    v1 (1): TEST p1 " << N << endl;
		tree.init(&v1[0].p1, N, sizeof(big), 8);
		tree.clear();
		cout << "    v1 (1): TEST p2 " << N << endl;
		tree.init(&v1[0].p2, N, sizeof(big), 8);
		tree.clear();
		cout << "    v1 (1): TEST p3 " << N << endl;
		tree.init(&v1[0].p3, N, sizeof(big), 8);
		tree.clear();
		cout << "    v1 (1): TEST p4 " << N << endl;
		tree.init(&v1[0].p4, N, sizeof(big), 8);
		tree.clear();

		cout << "    v1 (2): TEST p1 " << N/2 << endl;
		tree.init(&v1[N/2].p1, N/2, sizeof(big), 8);
		tree.clear();
		cout << "    v1 (2): TEST p2 " << N/2 << endl;
		tree.init(&v1[N/2].p2, N/2, sizeof(big), 8);
		tree.clear();
		cout << "    v1 (2): TEST p3 " << N/2 << endl;
		tree.init(&v1[N/2].p3, N/2, sizeof(big), 8);
		tree.clear();
		cout << "    v1 (2): TEST p4 " << N/2 << endl;
		tree.init(&v1[N/2].p4, N/2, sizeof(big), 8);
		tree.clear();

		vector<bigger> v2(N);
		for (size_t i = 0; i < N; ++i) {
			size_t base = 27*i;
			v2[i].p1 = vec3( base +  0.0f, base +  1.0f, base +  2.0f );
			v2[i].p2 = vec3( base +  3.0f, base +  4.0f, base +  5.0f );
			v2[i].p3 = vec3( base +  6.0f, base +  7.0f, base +  8.0f );
			v2[i].p4 = vec3( base +  9.0f, base + 10.0f, base + 11.0f );
			v2[i].p5 = vec3( base + 12.0f, base + 13.0f, base + 14.0f );
			v2[i].p6 = vec3( base + 15.0f, base + 16.0f, base + 17.0f );
			v2[i].p7 = vec3( base + 18.0f, base + 19.0f, base + 20.0f );
			v2[i].p8 = vec3( base + 21.0f, base + 22.0f, base + 23.0f );
			v2[i].p9 = vec3( base + 24.0f, base + 25.0f, base + 26.0f );
		}

		cout << "    v2 (1): TEST p5 " << N << endl;
		tree.init(&v2[0].p5, N, sizeof(bigger), 8);
		tree.clear();
		cout << "    v2 (1): TEST p6 " << N << endl;
		tree.init(&v2[0].p6, N, sizeof(bigger), 8);
		tree.clear();
		cout << "    v2 (1): TEST p7 " << N << endl;
		tree.init(&v2[0].p7, N, sizeof(bigger), 8);
		tree.clear();
		cout << "    v2 (1): TEST p8 " << N << endl;
		tree.init(&v2[0].p8, N, sizeof(bigger), 8);
		tree.clear();
		cout << "    v2 (1): TEST p9 " << N << endl;
		tree.init(&v2[0].p9, N, sizeof(bigger), 8);
		tree.clear();

		cout << "    v2 (2): TEST p5 " << N/2 << endl;
		tree.init(&v2[N/2].p5, N/2, sizeof(bigger), 8);
		tree.clear();
		cout << "    v2 (2): TEST p6 " << N/2 << endl;
		tree.init(&v2[N/2].p6, N/2, sizeof(bigger), 8);
		tree.clear();
		cout << "    v2 (2): TEST p7 " << N/2 << endl;
		tree.init(&v2[N/2].p7, N/2, sizeof(bigger), 8);
		tree.clear();
		cout << "    v2 (2): TEST p8 " << N/2 << endl;
		tree.init(&v2[N/2].p8, N/2, sizeof(bigger), 8);
		tree.clear();
		cout << "    v2 (2): TEST p9 " << N/2 << endl;
		tree.init(&v2[N/2].p9, N/2, sizeof(bigger), 8);
		tree.clear();
	}

	void sim_900_test_arrays() {
		big *v1 = static_cast<big *>(malloc(N*sizeof(big)));
		if (v1 == nullptr) {
			cerr << "Error in allocating " << N << " elements of type big" << endl;
			return;
		}
		for (size_t i = 0; i < N; ++i) {
			size_t base = 12*i;
			v1[i].p1 = vec3( base + 0.0f, base +  1.0f, base +  2.0f );
			v1[i].p2 = vec3( base + 3.0f, base +  4.0f, base +  5.0f );
			v1[i].p3 = vec3( base + 6.0f, base +  7.0f, base +  8.0f );
			v1[i].p4 = vec3( base + 9.0f, base + 10.0f, base + 11.0f );
		}

		octree tree;

		cout << "Filling from arrays:" << endl;

		cout << "    v1 (1): TEST p1 " << N << endl;
		tree.init(&v1[0].p1, N, sizeof(big), 8);
		tree.clear();
		cout << "    v1 (1): TEST p2 " << N << endl;
		tree.init(&v1[0].p2, N, sizeof(big), 8);
		tree.clear();
		cout << "    v1 (1): TEST p3 " << N << endl;
		tree.init(&v1[0].p3, N, sizeof(big), 8);
		tree.clear();
		cout << "    v1 (1): TEST p4 " << N << endl;
		tree.init(&v1[0].p4, N, sizeof(big), 8);
		tree.clear();

		cout << "    v1 (2): TEST p1 " << N/2 << endl;
		tree.init(&v1[N/2].p1, N/2, sizeof(big), 8);
		tree.clear();
		cout << "    v1 (2): TEST p2 " << N/2 << endl;
		tree.init(&v1[N/2].p2, N/2, sizeof(big), 8);
		tree.clear();
		cout << "    v1 (2): TEST p3 " << N/2 << endl;
		tree.init(&v1[N/2].p3, N/2, sizeof(big), 8);
		tree.clear();
		cout << "    v1 (2): TEST p4 " << N/2 << endl;
		tree.init(&v1[N/2].p4, N/2, sizeof(big), 8);
		tree.clear();

		free(v1);

		bigger *v2 = static_cast<bigger *>(malloc(N*sizeof(bigger)));
		if (v2 == nullptr) {
			cerr << "Error in allocating " << N << " elements of type bigger" << endl;
			return;
		}
		for (size_t i = 0; i < N; ++i) {
			size_t base = 27*i;
			v2[i].p1 = vec3( base +  0.0f, base +  1.0f, base +  2.0f );
			v2[i].p2 = vec3( base +  3.0f, base +  4.0f, base +  5.0f );
			v2[i].p3 = vec3( base +  6.0f, base +  7.0f, base +  8.0f );
			v2[i].p4 = vec3( base +  9.0f, base + 10.0f, base + 11.0f );
			v2[i].p5 = vec3( base + 12.0f, base + 13.0f, base + 14.0f );
			v2[i].p6 = vec3( base + 15.0f, base + 16.0f, base + 17.0f );
			v2[i].p7 = vec3( base + 18.0f, base + 19.0f, base + 20.0f );
			v2[i].p8 = vec3( base + 21.0f, base + 22.0f, base + 23.0f );
			v2[i].p9 = vec3( base + 24.0f, base + 25.0f, base + 26.0f );
		}

		cout << "    v2 (1): TEST p5 " << N << endl;
		tree.init(&v2[0].p5, N, sizeof(bigger), 8);
		tree.clear();
		cout << "    v2 (1): TEST p6 " << N << endl;
		tree.init(&v2[0].p6, N, sizeof(bigger), 8);
		tree.clear();
		cout << "    v2 (1): TEST p7 " << N << endl;
		tree.init(&v2[0].p7, N, sizeof(bigger), 8);
		tree.clear();
		cout << "    v2 (1): TEST p8 " << N << endl;
		tree.init(&v2[0].p8, N, sizeof(bigger), 8);
		tree.clear();
		cout << "    v2 (1): TEST p9 " << N << endl;
		tree.init(&v2[0].p9, N, sizeof(bigger), 8);
		tree.clear();

		cout << "    v2 (2): TEST p5 " << N/2 << endl;
		tree.init(&v2[N/2].p5, N/2, sizeof(bigger), 8);
		tree.clear();
		cout << "    v2 (2): TEST p6 " << N/2 << endl;
		tree.init(&v2[N/2].p6, N/2, sizeof(bigger), 8);
		tree.clear();
		cout << "    v2 (2): TEST p7 " << N/2 << endl;
		tree.init(&v2[N/2].p7, N/2, sizeof(bigger), 8);
		tree.clear();
		cout << "    v2 (2): TEST p8 " << N/2 << endl;
		tree.init(&v2[N/2].p8, N/2, sizeof(bigger), 8);
		tree.clear();
		cout << "    v2 (2): TEST p9 " << N/2 << endl;
		tree.init(&v2[N/2].p9, N/2, sizeof(bigger), 8);
		tree.clear();

		free(v2);
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

