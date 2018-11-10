// C includes
#include <string.h>

// C++ includes
#include <iostream>
using namespace std;

// custom includes
#include "study_cases.hpp"

void list_all_cases() {
	cout << "Welcome to the Spring-meshes renderer" << endl;
	cout << "The list of simulations available are:" << endl;
	cout << endl;
	cout << "    mesh1d         : simulation of several one-dimensional meshes." << endl;
	cout << "    mesh2d_regular : " << endl;
	cout << "    mesh2d_regular-sphere : " << endl;
}

int main(int argc, char *argv[]) {
	if (argc == 1) {
		cerr << "Error: lacking parameter for scene" << endl;
		cerr << "    ./spring-meshes case_identifier" << endl;
		cerr << "    Use './spring-meshes --list' or" << endl;
		cerr << "        './spring-meshes --help' or" << endl;
		cerr << "    to see all cases." << endl;
		return 1;
	}

	if (strcmp(argv[1], "--list") == 0 or strcmp(argv[1], "--help") == 0) {
		list_all_cases();
		return 0;
	}

	study_cases::choose_case(argc, argv);
	return 0;
}
