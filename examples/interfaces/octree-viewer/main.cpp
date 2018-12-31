// C includes
#include <string.h>

// C++ includes
#include <iostream>
using namespace std;

// custom includes
#include "point_set_viewer.hpp"
#include "fluid_viewer.hpp"

// render includes
#include <render/include_gl.hpp>

int main(int argc, char* argv[]) {
	if (argc == 1) {
		cerr << "Error: I need to choose between a point set visualiser" << endl;
		cerr << "    and a fluid's point set visualer" << endl;
		cerr << "    --file" << endl;
		cerr << "    --fluid" << endl;
		return 1;
	}

	if (strcmp(argv[1], "--file") == 0) {
		__psv_interface(argc, argv);
		return 0;
	}

	if (strcmp(argv[1], "--fluid") == 0) {
		__fv_interface(argc, argv);
		return 0;
	}

	cerr << "Error: unknown option '" << string(argv[1]) << endl;

}
