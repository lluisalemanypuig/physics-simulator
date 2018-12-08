// C includes
#include <string.h>

// C++ includes
#include <iostream>
using namespace std;

// Custom includes
#include "study_cases.hpp"

void list_all_cases() {
	cout << "The available simulations are:" << endl;
	cout << endl;
	cout << "    000 : particle bouncing on a floor." << endl;
	cout << "    100 : sized particle bouncing on a floor." << endl;
	cout << "    001 : particle rolling on a floor." << endl;
	cout << "    101 : sized particle rolling on a floor." << endl;
	cout << "    002 : particle bouncing on a sphere." << endl;
	cout << "    102 : sized particle bouncing on a sphere." << endl;
	cout << "    003 : two triangles on a floor. The particle" << endl;
	cout << "          falls directly on a triangle and bounces." << endl;
	cout << "    004 : a particle that rolls on a plane then collides" << endl;
	cout << "          with a sphere at the bottom of the plane." << endl;
	cout << "    005 : several geometrical objects. Intended for" << endl;
	cout << "          efficiency measuring." << endl;
	cout << "    200 : simulate a very simple 1-dimensional mesh" << endl;
	cout << "          of three particles" << endl;
	cout << "    201 : simulate a two-dimensional regular mesh" << endl;
	cout << "    300 : read two models from a file and create a" << endl;
	cout << "          geometrical object from it" << endl;
}

int main(int argc, char *argv[]) {
	if (argc == 1) {
		cerr << "Error: lacking parameter for scene" << endl;
		cerr << "    ./basic_simulations case_identifier" << endl;
		cerr << "    Use './basic_simulations --list' or" << endl;
		cerr << "        './basic_simulations --help' or" << endl;
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

