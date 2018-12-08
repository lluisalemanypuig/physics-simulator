#include "study_cases.hpp"

// C includes
#include <string.h>

// C++ includes
#include <iostream>
using namespace std;

namespace study_cases {

	void choose_case(int argc, char *argv[]) {

		int id_num = atoi(argv[1]);

		switch (id_num) {
		// free particles
		case 000:	sim_000(argc, argv); break;
		case 001:	sim_001(argc, argv); break;
		case 002:	sim_002(argc, argv); break;
		case 003:	sim_003(argc, argv); break;
		case 004:	sim_004(argc, argv); break;
		case 005:	sim_005(argc, argv); break;
		// sized particles
		case 100:	sim_100(argc, argv); break;
		case 101:	sim_101(argc, argv); break;
		case 102:	sim_102(argc, argv); break;
		// spring meshes
		case 200:	sim_200(argc, argv); break;
		case 201:	sim_201(argc, argv); break;
		default:
			cerr << "Unknown case '" << string(argv[1]) << "'." << endl;
			cerr << "    Use './basic_simulations --list' to see all cases" << endl;
		}
	}

} // -- namespace study_cases
