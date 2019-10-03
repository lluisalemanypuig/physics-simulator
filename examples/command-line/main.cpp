/*********************************************************************
 * Real-time physics simulation project
 * Copyright (C) 2018-2019 Lluís Alemany Puig
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * Contact: Lluís Alemany Puig (lluis.alemany.puig@gmail.com)
 * 
 ********************************************************************/

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

	/* ------ */
	/* FLUIDS */

	void sim_300(int argc, char *argv[]);
	void sim_301(int argc, char *argv[]);
	void sim_302(int argc, char *argv[]);

	/* ------------ */
	/* INPUT/OUTPUT */

	void sim_800(int argc, char *argv[]);
	void sim_801(int argc, char *argv[]);
	void sim_802(int argc, char *argv[]);

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
	cout << "Fluids:" << endl;
	cout << "    300 : simulate a very small and simple fluid" << endl;
	cout << "    301 : simulate a very small and simple fluid" << endl;
	cout << "          over several time steps" << endl;
	cout << "    302 : simulate a very small and simple fluid" << endl;
	cout << "          over several time steps with geometry" << endl;
	cout << endl;
	cout << "Input testing:" << endl;
	cout << "    800 : reads a model from a file and create a geometrical" << endl;
	cout << "          object from it. Query of closest triangles." << endl;
	cout << "    801 : reads a point list from a file and queries the closest" << endl;
	cout << "          points within a certain radius." << endl;
	cout << "    802 : reads a model from a file and create a geometrical" << endl;
	cout << "          geometrical object from it. Query of closest triangles" << endl;
	cout << "          within radius." << endl;
	cout << endl;
	cout << "Memory leak testing:" << endl;
	cout << "    900 : Using vec3 inside a 'random' struct, fills an octree" << endl;
	cout << endl;
}

int main(int argc, char *argv[]) {
	cout << "Real-time physics simulation project  Copyright (C) 2018-2019  Lluís Alemany Puig" << endl;
	cout << "This program comes with ABSOLUTELY NO WARRANTY; for details type `show w'." << endl;
	cout << "This is free software, and you are welcome to redistribute it" << endl;
	cout << "under certain conditions; type `show c' for details." << endl;
	
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
	// fluids
	case 300:	study_cases::sim_300(argc, argv); break;
	case 301:	study_cases::sim_301(argc, argv); break;
	case 302:	study_cases::sim_302(argc, argv); break;
	// input/output
	case 800:	study_cases::sim_800(argc, argv); break;
	case 801:	study_cases::sim_801(argc, argv); break;
	case 802:	study_cases::sim_802(argc, argv); break;
	// memory
	case 900:	study_cases::sim_900(argc, argv); break;
	default:
		cerr << "Unknown case '" << string(argv[1]) << "'." << endl;
		cerr << "    Use './basic_simulations --list' to see all cases" << endl;
	}

	return 0;
}

