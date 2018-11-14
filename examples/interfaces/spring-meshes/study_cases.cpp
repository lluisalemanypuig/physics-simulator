#include "study_cases.hpp"

// C++ includes
#include <iostream>
using namespace std;

// C includes
#include <string.h>

namespace study_cases {

	void choose_case(int argc, char *argv[]) {

		if (strcmp(argv[1], "mesh1d") == 0) {
			sim1_1dmeshes(argc, argv);
		}
		else if (strcmp(argv[1], "mesh2d-reg") == 0) {
			sim2_2dmeshes(argc, argv);
		}
		else if (strcmp(argv[1], "mesh2d-reg-sphere") == 0) {
			sim3_2dmeshes(argc, argv);
		}
		else {
			cerr << "Unknown case '" << string(argv[1]) << "'." << endl;
			cerr << "    Use './spring-meshes --list' to see all cases" << endl;
		}
	}

} // -- namespace study_cases
