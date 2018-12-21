// C includes
#include <string.h>

// C++ includes
#include <iostream>
using namespace std;

namespace study_cases {
	void sim_000(int argc, char *argv[]);
	void sim_001(int argc, char *argv[]);
	void sim_002(int argc, char *argv[]);
	void sim_003(int argc, char *argv[]);
	void sim_004(int argc, char *argv[]);
	void sim_005(int argc, char *argv[]);
	void sim_006(int argc, char *argv[]);
	void sim_100(int argc, char *argv[]);
	void sim_101(int argc, char *argv[]);
	void sim_102(int argc, char *argv[]);
	void sim_103(int argc, char *argv[]);
	void sim_104(int argc, char *argv[]);
	void sim_105(int argc, char *argv[]);
	void sim_106(int argc, char *argv[]);

} // -- namespace study_cases

void list_all_cases() {
	cout << "Welcome to the particles renderer" << endl;
	cout << "The list of simulations available are:" << endl;
	cout << endl;
	cout << "Free particles:" << endl;
	cout << "    000 : shower of 1000 particles on a plane." << endl;
	cout << endl;
	cout << "    001 : similar to '00' but with 300 particles and a" << endl;
	cout << "          triangle and a rectangle in between." << endl;
	cout << endl;
	cout << "    002 : particles rolling down a rectangle and" << endl;
	cout << "          colliding with a sphere." << endl;
	cout << endl;
	cout << "    003 : simulation of 16000 particles randomly shot" << endl;
	cout << "          inside a quadrilateral (delimited by planes)" << endl;
	cout << "          with geometry inside it (2 spheres and a triangle)." << endl;
	cout << endl;
	cout << "    004 : to study the effects of the friction coefficient." << endl;
	cout << "          11 particles, each with a different friction cofficient" << endl;
	cout << "          roll on an infinte plane with bouncing coefficient" << endl;
	cout << "          fixed to 1." << endl;
	cout << endl;
	cout << "    005 : likewise '04' but for the bouncing coefficient." << endl;
	cout << "          Friction coefficient is fixed to 0." << endl;
	cout << endl;
	cout << "    006 : (testing) One free particle colliding with an object." << endl;
	cout << endl;
	cout << "Sized particles:" << endl;
	cout << "    100 : (testing) sized particle bouncing on a plane." << endl;
	cout << endl;
	cout << "    101 : (testing) sized particle bouncing on a sphere." << endl;
	cout << endl;
	cout << "    102 : (testing) sized particle bouncing on a triangle." << endl;
	cout << endl;
	cout << "    103 : (testing) sized particle bouncing on a rectangle." << endl;
	cout << endl;
	cout << "    104 : (testing) two sized particles bouncing within a quadrilateral." << endl;
	cout << endl;
	cout << "    105 : (testing) three sized particles bouncing within a quadrilateral" << endl;
	cout << "          with no gravity." << endl;
	cout << endl;
	cout << "    106 : (testing) three sized particles bouncing within a quadrilateral" << endl;
	cout << "          with gravity. All planes have their normals pointing outwards" << endl;
	cout << "          quadrilateral." << endl;
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

	if (is_help(argv[1])) {
		list_all_cases();
		return 0;
	}

	int id_num = atoi(argv[1]);

	switch (id_num) {
	// free particles
	case 000:	study_cases::sim_000(argc, argv); break;
	case 001:	study_cases::sim_001(argc, argv); break;
	case 002:	study_cases::sim_002(argc, argv); break;
	case 003:	study_cases::sim_003(argc, argv); break;
	case 004:	study_cases::sim_004(argc, argv); break;
	case 005:	study_cases::sim_005(argc, argv); break;
	case 006:	study_cases::sim_006(argc, argv); break;
	case 100:	study_cases::sim_100(argc, argv); break;
	case 101:	study_cases::sim_101(argc, argv); break;
	case 102:	study_cases::sim_102(argc, argv); break;
	case 103:	study_cases::sim_103(argc, argv); break;
	case 104:	study_cases::sim_104(argc, argv); break;
	case 105:	study_cases::sim_105(argc, argv); break;
	case 106:	study_cases::sim_106(argc, argv); break;
	default:
		cerr << "Unknown case '" << string(argv[1]) << "'." << endl;
		cerr << "    Use './particles --list' to see all cases" << endl;
	}

	return 0;
}
