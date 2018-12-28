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
#include <physim/structures/octree.hpp>
#include <physim/math/vec3.hpp>
using namespace physim;
using namespace input;
using namespace math;
using namespace geometric;
using namespace structures;

namespace study_cases {

	void sim_800_usage() {
		cout << "Input operations:" << endl;
		cout << endl;
		cout << "This study case simply reads one file from disk" << endl;
		cout << "and creates a geometrical object with it. Then uses" << endl;
		cout << "the point passed as parameter to locate the closest" << endl;
		cout << "triangles of the object closest to the point." << endl;
		cout << endl;
		cout << "Options:" << endl;
		cout << endl;
		cout << "    --directory d: specify input directory" << endl;
		cout << "    --file f: specify input file" << endl;
		cout << "    --point x y z: specify a 3d point" << endl;
		cout << endl;
		cout << "The file read is: d/f" << endl;
		cout << endl;
	}

	void sim_800(int argc, char *argv[]) {
		string directory = "none";
		string filename = "none";
		bool p1_read, p2_read;
		p1_read = p2_read = false;
		vec3 p1, p2;

		if (argc == 2) {
			sim_800_usage();
			return;
		}

		for (int i = 2; i < argc; ++i) {
			if (strcmp(argv[i], "-h") == 0 or strcmp(argv[i], "--help") == 0) {
				sim_800_usage();
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
			else if (strcmp(argv[i], "--point") == 0) {
				if (not p1_read) {
					p1.x = static_cast<float>(atof(argv[i + 1]));
					p1.y = static_cast<float>(atof(argv[i + 2]));
					p1.z = static_cast<float>(atof(argv[i + 3]));
					p1_read = true;
				}
				else {
					p2.x = static_cast<float>(atof(argv[i + 1]));
					p2.y = static_cast<float>(atof(argv[i + 2]));
					p2.z = static_cast<float>(atof(argv[i + 3]));
					p2_read = true;
				}
				i += 4;
			}
			else {
				cerr << "Error: unknown option '" << string(argv[i]) << "'" << endl;
				cerr << "    Use ./command-line 800 --help to see the usage" << endl;
				return;
			}
		}

		if (directory == "none") {
			cerr << "Error: directory not specified" << endl;
			cerr << "    Use ./command-line 800 --help to see the usage" << endl;
			return;
		}

		if (filename == "none") {
			cerr << "Error: filename not specified" << endl;
			cerr << "    Use ./command-line 800 --help to see the usage" << endl;
			return;
		}

		if (not p1_read) {
			cerr << "Error: you have to specify at least one point!" << endl;
			cerr << "    Use ./command-line 800 --help to see the usage" << endl;
			return;
		}

		cout << "Reading: " << directory + "/" + filename << endl;

		timing::time_point begin = timing::now();
		object *o = new object();
		bool r = input::read_file(directory, filename, o);
		if (not r) {
			cerr << "Error: some error occurred while trying to read" << endl;
			cerr << "    " << directory + "/" + filename << endl;
			return;
		}
		timing::time_point end = timing::now();
		double r_sim = timing::elapsed_seconds(begin, end);

		cout << "Time spent in reading: " << r_sim << " s" << endl;

		vector<size_t> tris;
		o->get_partition().get_indices(p1, tris);
		cout << "The closest triangles to point ("
			 << p1.x << "," << p1.y << "," << p1.z
			 << ") are:";
		for (size_t t_idx : tris) {
			cout << ", " << t_idx/3;
		}
		cout << endl;

		if (p2_read) {
			bool i = o->intersec_segment(p1,p2);
			cout << "Segment from ("
				 << p1.x << "," << p1.y << "," << p1.z
				 << ") to ("
				 << p2.x << "," << p2.y << "," << p2.z
				 << ")";
			if (i) {
				cout << " intersects ";
			}
			else {
				cout << " does NOT intersect ";
			}
			cout << "the object" << endl;
		}

		octree tree_copy;
		tree_copy.copy(o->get_partition());

		cout << "Repeating query..." << endl;
		tris.clear();
		o->get_partition().get_indices(p1, tris);

		cout << "The closest triangles to point ("
			 << p1.x << "," << p1.y << "," << p1.z
			 << ") are:";
		for (size_t t_idx : tris) {
			cout << ", " << t_idx/3;
		}
		cout << endl;

		delete o;
	}

} // -- namespace study_cases

