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

namespace study_cases {

	void sim_00(int argc, char *argv[]);
	void sim_01(int argc, char *argv[]);

	void sim_02(int argc, char *argv[]);

} // -- namespace study_cases

void list_all_cases() {
	cout << "Welcome to the Fluids renderer" << endl;
	cout << "The list of simulations available are:" << endl;
	cout << endl;
	cout << "    00 : simulation of a droplet of a fluid." << endl;
	cout << "    01 : filling a small box." << endl;
	cout << "    02 : fluid on monkey." << endl;
	cout << endl;
}

bool is_help(const char *arg) {
	return (strcmp(arg, "-h") == 0) or
		   (strcmp(arg, "--h") == 0) or
		   (strcmp(arg, "--he") == 0) or
		   (strcmp(arg, "--hel") == 0) or
		   (strcmp(arg, "--help") == 0) or
		   (strcmp(arg, "-l") == 0) or
		   (strcmp(arg, "--l") == 0) or
		   (strcmp(arg, "--li") == 0) or
		   (strcmp(arg, "--lis") == 0) or
		   (strcmp(arg, "--list") == 0);
}

int main(int argc, char *argv[]) {
	cout << "Real-time physics simulation project  Copyright (C) 2018-2019  Lluís Alemany Puig" << endl;
	cout << "This program comes with ABSOLUTELY NO WARRANTY; for details type `show w'." << endl;
	cout << "This is free software, and you are welcome to redistribute it" << endl;
	cout << "under certain conditions; type `show c' for details." << endl;
	
	if (argc == 1 or is_help(argv[1])) {
		list_all_cases();
		return 0;
	}

	if (strcmp(argv[1], "00") == 0) {
		study_cases::sim_00(argc, argv);
	}
	else if (strcmp(argv[1], "01") == 0) {
		study_cases::sim_01(argc, argv);
	}
	else if (strcmp(argv[1], "02") == 0) {
		study_cases::sim_02(argc, argv);
	}

	return 0;
}
