#include "study_cases.hpp"

// C includes
#include <string.h>

// C++ includes
#include <iostream>
#include <fstream>
using namespace std;

// Custom includes
#include "utils.hpp"

// physim includes
#include <physim/input/input.hpp>
#include <physim/geometry/object.hpp>
using namespace physim;
using namespace input;

namespace study_cases {

	void sim_300_usage() {
		cout << "Input operations:" << endl;
		cout << endl;
		cout << "This study case simply reads several files from disk" << endl;
		cout << "and creates geometrical objects with them" << endl;
		cout << endl;
		cout << "Options:" << endl;
		cout << endl;
		cout << "    --directory d: specify input directory" << endl;
		cout << "    --file f: specify input file" << endl;
		cout << endl;
		cout << "The file read is: d/f" << endl;
		cout << endl;
	}

	void sim_300(int argc, char *argv[]) {
		string directory = "none";
		string filename = "none";

		for (int i = 2; i < argc; ++i) {
			if (strcmp(argv[i], "-h") == 0 or strcmp(argv[i], "--help") == 0) {
				sim_300_usage();
				return;
			}
			else if (strcmp(argv[i], "--directory") == 0) {
				directory = string(argv[i + 1]);
				++i;
			}
			else if (strcmp(argv[i], "--file") == 0) {
				filename = string(argv[i + 1]);
				++i;
			}
			else {
				cerr << "Error: unknown option '" << string(argv[i]) << "'" << endl;
				cerr << "Use ./command-line 300 --help to see the usage" << endl;
				return;
			}
		}

		if (directory == "none") {
			cerr << "Error: directory not specified" << endl;
			cerr << "Use ./command-line 300 --help to see the usage" << endl;
			return;
		}

		if (filename == "none") {
			cerr << "Error: filename not specified" << endl;
			cerr << "Use ./command-line 300 --help to see the usage" << endl;
			return;
		}

		cout << "Reading: " << directory + "/" + filename << endl;

		timing::time_point begin_sim = timing::now();

		geom::object *o = new geom::object();
		bool r = input::read_file(directory, filename, o);
		if (not r) {
			cerr << "Error: some error occurred while trying to read" << endl;
			cerr << "    " << directory + "/" + filename << endl;
		}

		timing::time_point end_sim = timing::now();

		double r_sim = timing::elapsed_seconds(begin_sim, end_sim);
		cout << "Time spent in reading: " << r_sim << " s" << endl;

		delete o;
	}

} // -- namespace study_cases

