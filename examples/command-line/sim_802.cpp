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

	void sim_802_usage() {
		cout << "Input operations:" << endl;
		cout << endl;
		cout << "This study case reads a file containing an object, makes an" << endl;
		cout << "octree with them and uses the sphere passed as parameter to" << endl;
		cout << "perform a range query the closest triangles." << endl;
		cout << endl;
		cout << "Options:" << endl;
		cout << endl;
		cout << "    --directory d: specify input directory" << endl;
		cout << "    --file f: specify input file" << endl;
		cout << "    --point x y z: specify a 3d point" << endl;
		cout << "    --radius r: specify the sphere's radius" << endl;
		cout << endl;
		cout << "The file read is: d/f" << endl;
		cout << endl;
	}

	void sim_802(int argc, char *argv[]) {
		string directory = "none";
		string filename = "none";
		bool point_read = false;
		bool radius_read = false;
		vec3 point;
		float R;

		if (argc == 2) {
			sim_802_usage();
			return;
		}

		for (int i = 2; i < argc; ++i) {
			if (strcmp(argv[i], "-h") == 0 or strcmp(argv[i], "--help") == 0) {
				sim_802_usage();
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
				if (not point_read) {
					point.x = static_cast<float>(atof(argv[i + 1]));
					point.y = static_cast<float>(atof(argv[i + 2]));
					point.z = static_cast<float>(atof(argv[i + 3]));
					point_read = true;
				}
				i += 3;
			}
			else if (strcmp(argv[i], "--radius") == 0) {
				if (not radius_read) {
					R = static_cast<float>(atof(argv[i + 1]));
					radius_read = true;
				}
				++i;
			}
			else {
				cerr << "Error: unknown option '" << string(argv[i]) << "'" << endl;
				cerr << "    Use ./command-line 802 --help to see the usage" << endl;
				return;
			}
		}

		if (directory == "none") {
			cerr << "Error: directory not specified" << endl;
			cerr << "    Use ./command-line 802 --help to see the usage" << endl;
			return;
		}

		if (filename == "none") {
			cerr << "Error: filename not specified" << endl;
			cerr << "    Use ./command-line 802 --help to see the usage" << endl;
			return;
		}

		if (not point_read) {
			cerr << "Error: you have to specify one point!" << endl;
			cerr << "    Use ./command-line 802 --help to see the usage" << endl;
			return;
		}
		if (not radius_read) {
			cerr << "Error: you have to specify radius of query!" << endl;
			cerr << "    Use ./command-line 802 --help to see the usage" << endl;
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
		begin = timing::now();
		o->get_partition().get_indices(point, R, tris);
		end = timing::now();

		cout << "The triangles within radius " << R << " from point ("
			 << point.x << "," << point.y << "," << point.z
			 << ") are:";
		for (size_t t_idx : tris) {
			cout << ", " << t_idx/3;
		}
		cout << endl;

		r_sim = timing::elapsed_seconds(begin, end);
		cout << "Time spent in querying: " << r_sim << " s" << endl;

		delete o;
	}

} // -- namespace study_cases

