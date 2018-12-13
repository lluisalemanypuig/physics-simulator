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
	void sim_04(int argc, char *argv[]);
	void sim_05(int argc, char *argv[]);
	void sim_06(int argc, char *argv[]);
	void sim_07(int argc, char *argv[]);
	void sim_08(int argc, char *argv[]);
	void sim_09(int argc, char *argv[]);
	void sim_10(int argc, char *argv[]);
	void sim_11(int argc, char *argv[]);
	void sim_12(int argc, char *argv[]);
	void sim_13(int argc, char *argv[]);

} // -- namespace study_cases

void list_all_cases() {
	cout << "Welcome to the particles renderer" << endl;
	cout << "The list of simulations available are:" << endl;
	cout << endl;
	cout << "    * 00 : shower of 1000 particles on a plane." << endl;
	cout << endl;
	cout << "    * 01 : similar to '00' but with 300 particles and a" << endl;
	cout << "           triangle and a rectangle in between." << endl;
	cout << endl;
	cout << "    * 02 : particles rolling down a rectangle and" << endl;
	cout << "           colliding with a sphere." << endl;
	cout << endl;
	cout << "    * 03 : simulation of 16000 particles randomly shot" << endl;
	cout << "           inside a quadrilateral (delimited by planes)" << endl;
	cout << "           with geometry inside it (2 spheres and a triangle)." << endl;
	cout << endl;
	cout << "    * 04 : to study the effects of the friction coefficient." << endl;
	cout << "           11 particles, each with a different friction cofficient" << endl;
	cout << "           roll on an infinte plane with bouncing coefficient" << endl;
	cout << "           fixed to 1." << endl;
	cout << endl;
	cout << "    * 05 : likewise '04' but for the bouncing coefficient." << endl;
	cout << "           Friction coefficient is fixed to 0." << endl;
	cout << endl;
	cout << "    * 06 : (testing) sized particle bouncing on a plane." << endl;
	cout << endl;
	cout << "    * 07 : (testing) sized particle bouncing on a sphere." << endl;
	cout << endl;
	cout << "    * 08 : (testing) sized particle bouncing on a triangle." << endl;
	cout << endl;
	cout << "    * 09 : (testing) sized particle bouncing on a rectangle." << endl;
	cout << endl;
	cout << "    * 10 : (testing) two sized particles bouncing within a quadrilateral." << endl;
	cout << endl;
	cout << "    * 11 : (testing) three sized particles bouncing within a quadrilateral" << endl;
	cout << "           with no gravity." << endl;
	cout << endl;
	cout << "    * 12 : (testing) three sized particles bouncing within a quadrilateral" << endl;
	cout << "           with gravity. All planes have their normals pointing outwards" << endl;
	cout << "           quadrilateral." << endl;
	cout << endl;
	cout << "    * 13 : (testing) One free particle colliding with an object." << endl;
	cout << endl;
}

bool is_help(const char *arg) {
	return (strcmp(arg, "-h") == 0) or
		   (strcmp(arg, "--h") == 0) or
		   (strcmp(arg, "--he") == 0) or
		   (strcmp(arg, "--hel") == 0) or
		   (strcmp(arg, "--help") == 0);
}

int main(int argc, char *argv[]) {
	if (argc == 1) {
		cerr << "Error: lacking parameter for scene: ./particles case_identifier" << endl;
		cerr << "    Use './particles --help' or" << endl;
		cerr << "to see all scenes available." << endl;
		return 1;
	}

	cout << string(argv[1]) << endl;

	if (is_help(argv[1])) {
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
	case  4:	study_cases::sim_04(argc, argv); break;
	case  5:	study_cases::sim_05(argc, argv); break;
	case  6:	study_cases::sim_06(argc, argv); break;
	case  7:	study_cases::sim_07(argc, argv); break;
	case  8:	study_cases::sim_08(argc, argv); break;
	case  9:	study_cases::sim_09(argc, argv); break;
	case 10:	study_cases::sim_10(argc, argv); break;
	case 11:	study_cases::sim_11(argc, argv); break;
	case 12:	study_cases::sim_12(argc, argv); break;
	case 13:	study_cases::sim_13(argc, argv); break;
	default:
		cerr << "Unknown case '" << string(argv[1]) << "'." << endl;
		cerr << "    Use './particles --list' to see all cases" << endl;
	}

	return 0;
}
