#include "study_cases.hpp"

// C++ includes
#include <iostream>
using namespace std;

// C includes
#include <string.h>

namespace study_cases {

	void choose_case(int argc, char *argv[]) {

		if (strcmp(argv[1], "mesh1d") == 0) {
			sim0_1dmeshes(argc, argv);
		}
		else {
			cerr << "Unknown case '" << string(argv[1]) << "'." << endl;
			cerr << "    Use './spring-meshes --list' to see all cases" << endl;
		}
	}

} // -- namespace study_cases