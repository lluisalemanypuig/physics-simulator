#include "study_cases.hpp"

namespace study_cases {

	void list_all_cases() {
		cout << "floor : particle bouncing on floor." << endl;
		cout << "tri2f : two triangles on a floor. The particle" << endl;
		cout << "        falls directly on a triangle and bounces." << endl;
	}

	void choose_case(int argc, char *argv[]) {
		if (strcmp(argv[1], "--list") == 0) {
			list_all_cases();
			return;
		}

		if (strcmp(argv[1], "floor") == 0) {
			floor(argc, argv);
		}
		else if (strcmp(argv[1], "tri2f") == 0) {
			tri_tri_floor(argc, argv);
		}
		else {
			cerr << "Unknown case '" << string(argv[1]) << "'." << endl;
			cerr << "    Use './basic_simulations --list' to see all cases" << endl;
		}

	}

} // -- namespace study_cases
