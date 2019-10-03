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

// custom includes
#include "point_set_viewer.hpp"
#include "fluid_viewer.hpp"

// render includes
#include <render/include_gl.hpp>

int main(int argc, char* argv[]) {
	cout << "Real-time physics simulation project  Copyright (C) 2018-2019  Lluís Alemany Puig" << endl;
	cout << "This program comes with ABSOLUTELY NO WARRANTY; for details type `show w'." << endl;
	cout << "This is free software, and you are welcome to redistribute it" << endl;
	cout << "under certain conditions; type `show c' for details." << endl;
	
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
