#include "study_cases.hpp"

// C++ includes
#include <iostream>
using namespace std;

// C includes
#include <string.h>

namespace study_cases {

	void choose_case(int argc, char *argv[]) {

		if (strcmp(argv[1], "00") == 0) {
			sim_00(argc, argv);
		}
		else if (strcmp(argv[1], "01") == 0) {
			sim_01(argc, argv);
		}
		else if (strcmp(argv[1], "02") == 0) {
			sim_02(argc, argv);
		}
		else if (strcmp(argv[1], "03") == 0) {
			sim_03(argc, argv);
		}
		else if (strcmp(argv[1], "04") == 0) {
			sim_04(argc, argv);
		}
		else if (strcmp(argv[1], "05") == 0) {
			sim_05(argc, argv);
		}
		else if (strcmp(argv[1], "06") == 0) {
			sim_06(argc, argv);
		}
		else if (strcmp(argv[1], "07") == 0) {
			sim_07(argc, argv);
		}
		else if (strcmp(argv[1], "08") == 0) {
			sim_08(argc, argv);
		}
		else if (strcmp(argv[1], "09") == 0) {
			sim_09(argc, argv);
		}
		else if (strcmp(argv[1], "10") == 0) {
			sim_10(argc, argv);
		}
		else {
			cerr << "Unknown case '" << string(argv[1]) << "'." << endl;
			cerr << "    Use './spring-meshes --list' to see all cases" << endl;
		}
	}

} // -- namespace study_cases
