// C++ includes
#include <iostream>
using namespace std;

// Custom includes
#include "study_cases.hpp"

int main(int argc, char *argv[]) {
	if (argc == 1) {
		cerr << "Error: I need to know what scene you want" << endl;
		cerr << "    ./basic_simulations case_identifier" << endl;
		cerr << "    Use './basic_simulations --list' to see all cases" << endl;
		return 1;
	}

	study_cases::choose_case(argc, argv);
}

