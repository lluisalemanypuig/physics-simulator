// C includes
#include <string.h>

// C++ includes
#include <iostream>
using namespace std;

namespace study_cases {
	void sim_00(int argc, char *argv[]);
	void sim_01(int argc, char *argv[]);
	void sim_02(int argc, char *argv[]);
	void sim_03(int argc, char *argv[]);

} // -- namespace study_cases

void list_all_cases() {
	cout << "Welcome to the Spring-meshes renderer" << endl;
	cout << "The list of simulations available are:" << endl;
	cout << endl;
	cout << "    * 00 : simulation of several one-dimensional meshes." << endl;
	cout << endl;
	cout << "    * 01 : simulation of a single two-dimensional mesh." << endl;
	cout << "        Most of its parameters (mass, size, elasticity, ...)" << endl;
	cout << "        can be modified. The model of the mesh is a regular grid." << endl;
	cout << endl;
	cout << "    * 02 : simulation of a single two-dimensional" << endl;
	cout << "        mesh that falls on a sphere." << endl;
	cout << "        Most of its parameters (mass, size, elasticity, ...)" << endl;
	cout << "        can be modified. The model of the mesh is a regular grid." << endl;
	cout << endl;
	cout << "    * 03 : simulation of a single two-dimensional" << endl;
	cout << "        mesh that falls on a geometrical object." << endl;
	cout << "        Most of its parameters (mass, size, elasticity, ...)" << endl;
	cout << "        can be modified. The model of the mesh is a regular grid." << endl;
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

	int id_num = atoi(argv[1]);
	switch (id_num) {
	// free particles
	case  0:	study_cases::sim_00(argc, argv); break;
	case  1:	study_cases::sim_01(argc, argv); break;
	case  2:	study_cases::sim_02(argc, argv); break;
	case  3:	study_cases::sim_03(argc, argv); break;
	default:
		cerr << "Unknown case '" << string(argv[1]) << "'." << endl;
		cerr << "    Use './particles --list' to see all cases" << endl;
	}

	return 0;
}
