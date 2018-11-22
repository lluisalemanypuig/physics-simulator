// C++ includes
#include <iostream>
using namespace std;

// Custom includes
#include "study_cases.hpp"

void list_all_cases() {
	cout << "The available simulations are:" << endl;
	cout << endl;
	cout << "    bounce-floor       : particle bouncing on a floor." << endl;
	cout << "    bounce-floor-sized : sized particle bouncing on a floor." << endl;
	cout << "    roll-floor         : particle rolling on a floor." << endl;
	cout << "    bounce-sphere      : particle bouncing on a sphere." << endl;
	cout << "    tri2f              : two triangles on a floor. The particle" << endl;
	cout << "                         falls directly on a triangle and bounces." << endl;
	cout << "    roll-plane-sphere  : a particle that rolls on a plane then collides" << endl;
	cout << "                         with a sphere at the bottom of the plane." << endl;
	cout << "    hose               : several geometrical objects. Intended for" << endl;
	cout << "                         efficiency measuring." << endl;
	cout << "    mesh1d             : simulate a very simple 1-dimensional mesh" << endl;
	cout << "                         of three particles" << endl;
	cout << "    mesh2d_regular     : simulate a two-dimensional regular mesh" << endl;
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

