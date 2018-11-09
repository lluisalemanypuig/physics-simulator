#include "study_cases.hpp"

namespace study_cases {

	void choose_case(int argc, char *argv[]) {

		if (strcmp(argv[1], "bounce-floor") == 0) {
			bounce_on_floor(argc, argv);
		}
		else if (strcmp(argv[1], "roll-floor") == 0) {
			roll_on_floor(argc, argv);
		}
		else if (strcmp(argv[1], "bounce-sphere") == 0) {
			bounce_on_sphere(argc, argv);
		}
		else if (strcmp(argv[1], "tri2f") == 0) {
			tri_tri_floor(argc, argv);
		}
		else if (strcmp(argv[1], "roll-plane-sphere") == 0) {
			roll_plane_sphere(argc, argv);
		}
		else if (strcmp(argv[1], "hose") == 0) {
			hose_case(argc, argv);
		}
		else if (strcmp(argv[1], "mesh1d") == 0) {
			mesh1d_case(argc, argv);
		}
		else if (strcmp(argv[1], "mesh2d_regular") == 0) {
			mesh2d_regular_case(argc, argv);
		}
		else {
			cerr << "Unknown case '" << string(argv[1]) << "'." << endl;
			cerr << "    Use './basic_simulations --list' to see all cases" << endl;
		}
	}

} // -- namespace study_cases
