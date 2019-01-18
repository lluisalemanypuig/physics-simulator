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
