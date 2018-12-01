// C includes
#include <string.h>

// C++ includes
#include <iostream>
using namespace std;

// custom includes
#include "study_cases.hpp"

void list_all_cases() {
	cout << "Welcome to the particles renderer" << endl;
	cout << "The list of simulations available are:" << endl;
	cout << endl;
	cout << "    * 00 : shower of 1000 particles on a plane." << endl;
	cout << endl;
	cout << "    * 01 : similar to '00' but with 300 particles and a" << endl;
	cout << "           triangle and a rectangle in between" << endl;
	cout << endl;
	cout << "    * 02 : particles rolling down a rectangle and" << endl;
	cout << "           colliding with a sphere." << endl;
	cout << endl;
	cout << "    * 03 : simulation of 16000 particles randomly shot" << endl;
	cout << "           inside a quadrilateral (delimited by planes)" << endl;
	cout << "           with geometry inside it (2 spheres and a triangle)" << endl;
	cout << endl;
	cout << "    * 04 : to study the effects of the friction coefficient." << endl;
	cout << "           11 particles, each with a different friction cofficient" << endl;
	cout << "           roll on an infinte plane with bouncing coefficient" << endl;
	cout << "           fixed to 1." << endl;
	cout << endl;
	cout << "    * 05 : likewise '04' but for the bouncing coefficient." << endl;
	cout << "           Friction coefficient is fixed to 0." << endl;
	cout << endl;
}

int main(int argc, char *argv[]) {
	if (argc == 1) {
		cerr << "Error: lacking parameter for scene: ./spring-meshes case_identifier" << endl;
		cerr << "    Use './spring-meshes --list' or" << endl;
		cerr << "        './spring-meshes --help' or" << endl;
		cerr << "    to see all scenes available." << endl;
		return 1;
	}

	if (strcmp(argv[1], "--list") == 0 or strcmp(argv[1], "--help") == 0) {
		list_all_cases();
		return 0;
	}

	study_cases::choose_case(argc, argv);
	return 0;
}
