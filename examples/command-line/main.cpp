// C includes
#include <string.h>

// C++ includes
#include <iostream>
using namespace std;

/* -------------- */
/* FREE PARTICLES */

namespace study_cases {
	void sim_000(int argc, char *argv[]);
	void sim_001(int argc, char *argv[]);
	void sim_002(int argc, char *argv[]);
	void sim_003(int argc, char *argv[]);
	void sim_004(int argc, char *argv[]);
	void sim_005(int argc, char *argv[]);

	/* --------------- */
	/* SIZED PARTICLES */

	void sim_100(int argc, char *argv[]);
	void sim_101(int argc, char *argv[]);
	void sim_102(int argc, char *argv[]);

	/* ------------- */
	/* SPRING MESHES */

	void sim_200(int argc, char *argv[]);
	void sim_201(int argc, char *argv[]);

	/* ------------ */
	/* INPUT/OUTPUT */

	void sim_300(int argc, char *argv[]);
	void sim_301(int argc, char *argv[]);
	void sim_302(int argc, char *argv[]);

	/* ---------------------------------------------- */
	/* STANDALONE SIMULATIONS FOR MEMORY LEAK TESTING */

	void sim_900(int argc, char *argv[]);

} // -- namespace study_cases

/* ---------------- */
/* MAIN's FUNCTIONS */

void list_all_cases() {
	cout << "The available simulations are:" << endl;
	cout << endl;
	cout << "Free particles:" << endl;
	cout << "    000 : particle bouncing on a floor." << endl;
	cout << "    001 : particle rolling on a floor." << endl;
	cout << "    002 : particle bouncing on a sphere." << endl;
	cout << "    003 : two triangles on a floor. The particle" << endl;
	cout << "          falls directly on a triangle and bounces." << endl;
	cout << "    004 : a particle that rolls on a plane then collides" << endl;
	cout << "          with a sphere at the bottom of the plane." << endl;
	cout << "    005 : several geometrical objects. Intended for" << endl;
	cout << "          efficiency measuring." << endl;
	cout << endl;
	cout << "Sized particles:" << endl;
	cout << "    100 : sized particle bouncing on a floor." << endl;
	cout << "    101 : sized particle rolling on a floor." << endl;
	cout << "    102 : sized particle bouncing on a sphere." << endl;
	cout << endl;
	cout << "Spring meshes:" << endl;
	cout << "    200 : simulate a very simple 1-dimensional mesh" << endl;
	cout << "          of three particles" << endl;
	cout << "    201 : simulate a two-dimensional regular mesh" << endl;
	cout << endl;
	cout << "Input testing:" << endl;
	cout << "    300 : reads a model from a file and create a geometrical" << endl;
	cout << "          object from it. Query of closest triangles." << endl;
	cout << "    301 : reads a point list from a file and queries the closest" << endl;
	cout << "          points within a certain radius." << endl;
	cout << "    302 : reads a model from a file and create a geometrical" << endl;
	cout << "          geometrical object from it. Query of closest triangles" << endl;
	cout << "          within radius." << endl;
	cout << endl;
	cout << "Memory leak testing:" << endl;
	cout << "    900 : Using vec3 inside a 'random' struct, fills an octree" << endl;
	cout << endl;
}

int main(int argc, char *argv[]) {
	if (argc == 1) {
		list_all_cases();
		return 1;
	}

	if (strcmp(argv[1], "--list") == 0 or strcmp(argv[1], "--help") == 0) {
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
	// sized particles
	case 100:	study_cases::sim_100(argc, argv); break;
	case 101:	study_cases::sim_101(argc, argv); break;
	case 102:	study_cases::sim_102(argc, argv); break;
	// spring meshes
	case 200:	study_cases::sim_200(argc, argv); break;
	case 201:	study_cases::sim_201(argc, argv); break;
	// input/output
	case 300:	study_cases::sim_300(argc, argv); break;
	case 301:	study_cases::sim_301(argc, argv); break;
	case 302:	study_cases::sim_302(argc, argv); break;
	// memory
	case 900:	study_cases::sim_900(argc, argv); break;
	default:
		cerr << "Unknown case '" << string(argv[1]) << "'." << endl;
		cerr << "    Use './basic_simulations --list' to see all cases" << endl;
	}

	return 0;
}

