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

	void print_boxes(const octree& tree) {
		typedef pair<vec3,vec3> box;
		vector<box> boxes;
		tree.get_boxes(boxes);

		cout << "Boxes:" << endl;
		for (const box& b : boxes) {
			cout << "    "
				 << b.first.x << "," << b.first.y << "," << b.first.z << " ,, "
				 << b.second.x << "," << b.second.y << "," << b.second.z << endl;
		}
	}

	void sim_801_usage() {
		cout << "Input operations:" << endl;
		cout << endl;
		cout << "This study case reads a file containing a list of points from" << endl;
		cout << "disk. Then takes the vertices of the object created, makes an" << endl;
		cout << "octree with them and uses the sphere passed as parameter to" << endl;
		cout << "perform a range query." << endl;
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

	void sim_801(int argc, char *argv[]) {
		string directory = "none";
		string filename = "none";
		bool point_read = false;
		bool radius_read = false;
		vec3 point;
		float R;

		if (argc == 2) {
			sim_801_usage();
			return;
		}

		for (int i = 2; i < argc; ++i) {
			if (strcmp(argv[i], "-h") == 0 or strcmp(argv[i], "--help") == 0) {
				sim_801_usage();
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
				cerr << "    Use ./command-line 801 --help to see the usage" << endl;
				return;
			}
		}

		if (directory == "none") {
			cerr << "Error: directory not specified" << endl;
			cerr << "    Use ./command-line 801 --help to see the usage" << endl;
			return;
		}

		if (filename == "none") {
			cerr << "Error: filename not specified" << endl;
			cerr << "    Use ./command-line 801 --help to see the usage" << endl;
			return;
		}

		if (not point_read) {
			cerr << "Error: you have to specify one point!" << endl;
			cerr << "    Use ./command-line 801 --help to see the usage" << endl;
			return;
		}
		if (not radius_read) {
			cerr << "Error: you have to specify radius of query!" << endl;
			cerr << "    Use ./command-line 801 --help to see the usage" << endl;
			return;
		}

		cout << "Reading: " << directory + "/" + filename << endl;

		// read the file
		vector<vec3> points;
		input::read_file(directory, filename, points);

		cout << "File read: " << points.size() << " points" << endl;

		timing::time_point begin, end;

		// construct octree
		cout << "Constructing octree..." << endl;
		begin = timing::now();
		octree tree;
		tree.init(points);
		end = timing::now();

		cout << "Octree constructed in: "
			 << timing::elapsed_milliseconds(begin, end)
			 << " ms" << endl;

		print_boxes(tree);

		// Query
		vector<size_t> idxs;
		cout << "Querying neighbourhood of ("
			 << point.x << "," << point.y << "," << point.z
			 << ") within radius " << R
			 << endl;
		begin = timing::now();
		tree.get_indices(point, R, idxs);
		end = timing::now();

		cout << "Query made in: "
			 << timing::elapsed_milliseconds(begin, end)
			 << " ms" << endl;

		cout << "Points likely to be within radius:" << endl;
		for (size_t idx : idxs) {
			cout << "    " << idx << " ("
				 << points[idx].x << "," << points[idx].y << "," << points[idx].z
				 << "). Distance: " << math::dist(points[idx], point) << endl;
		}
		cout << endl;

		tree.clear();
	}

} // -- namespace study_cases

