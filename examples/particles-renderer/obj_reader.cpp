#include "obj_reader.hpp"

OBJ_reader::OBJ_reader() { }

void OBJ_reader::clean() {
	file_lines.clear();

	vertices.clear();
	faces.clear();
	normals.clear();
	textures_coords.clear();
	materials.clear();
}

OBJ_reader::~OBJ_reader() {
	clean();
}

bool OBJ_reader::load_file() {
	string full_name = directory + "/" + filename;

	ifstream obj;
	obj.open(full_name.c_str());
	if (not obj.is_open()) {
		cerr << "OBJ_reader::load_file: Error" << endl;
		cerr << "    An error occurred while trying to open the object file" << endl;
		cerr << "    '" << full_name << "'" << endl;
		return false;
	}
	// read all lines of the file and
	// dump them in 'file_lines'
	char buf[256];
	while (not obj.eof()) {
		obj.getline(buf, 256);
		buf[255] = '\0';
		file_lines.push_back(string(buf));
	}
	obj.close();
	return true;
}

bool OBJ_reader::load_material(const char *mtl_file) {
	string full_name = directory + "/" + string(mtl_file);

	ifstream mtl;
	mtl.open(full_name.c_str());
	if (not mtl.is_open()) {
		cerr << "OBJ_reader::load_material: Error" << endl;
		cerr << "    An error occurred while trying to open the material file" << endl;
		cerr << "    '" << full_name << "'" << endl;
		return false;
	}

	// read all lines of the file and
	// dump them in 'mat_lines'
	vector<string> mat_lines;
	char buf[256];
	while (not mtl.eof()) {
		mtl.getline(buf, 256);
		mat_lines.push_back(string(buf));
	}
	mtl.close();

	string material_name;
	bool valid_material = false;
	vec3 amb, dif, spec;
	float Ns, Ni, d;
	int illum;
	int textID = -1;

	for (size_t i = 0; i < mat_lines.size(); ++i) {
		// ignore comments
		if (mat_lines[i][0] == '#') {
			continue;
		}

		string sub2 = mat_lines[i].substr(0,2);
		string sub3 = mat_lines[i].substr(0,3);

		// if line starts with 'new'
		if (sub3 == "new") {
			if (valid_material) {
				// first, the contents of the variables do not
				// contain a valid material, so this if is
				// skipped in the first iteration it is found.
				materials.push_back(material(amb, dif, spec, Ns, Ni, d, illum, textID, material_name));
			}
			valid_material = true;
			sscanf(mat_lines[i].c_str(),"newmtl %s", buf);
			material_name = buf;
			textID = -1;
		}
		else if (sub2 == "Ns") {
			sscanf(mat_lines[i].c_str(), "Ns %f", &Ns);
		}
		else if (sub2 == "Ka") {
			sscanf(mat_lines[i].c_str(), "Ka %f %f %f", &amb[0], &amb[1], &amb[2]);
		}
		else if (sub2 == "Kd") {
			sscanf(mat_lines[i].c_str(), "Kd %f %f %f", &dif[0], &dif[1], &dif[2]);
		}
		else if (sub2 == "Ks") {
			sscanf(mat_lines[i].c_str(), "Ks %f %f %f", &spec[0], &spec[1], &spec[2]);
		}
		else if (sub2 == "Ki") {
			sscanf(mat_lines[i].c_str(), "Ni %f", &Ni);
		}
		else if (sub2 == "d ") {
			sscanf(mat_lines[i].c_str(), "d %f", &d);
		}
		else if (sub2 == "il") {
			sscanf(mat_lines[i].c_str(), "illum %d", &illum);
		}
		else if (sub2 == "ma") {
			char aux[100];
			sscanf(mat_lines[i].c_str(), "map_Kd %s", aux);

			/*string fileTXT = aux;
			int k = fileTXT.size() - 1;
			while (k > 0 and fileTXT[k] != '/') --k;

			if (k == 0) textID = loadTXT(fileTXT.c_str());
			else textID = loadTXT((fileTXT.substr(k + 1, fileTXT.size())).c_str());*/

			if (textID == 0) {
				cout << "	Error in file: " << mtl_file << endl;
				cout << "		Cannot load texture" << endl;
				cout << "		Texture '" << aux << "' could not be loaded" << endl << endl;
				//return false;
			}
		}
	}
	// load last batch of data read
	materials.push_back(material(amb, dif, spec, Ns, Ni, d, illum, textID, material_name));

	return true;
}

void OBJ_reader::get_vertices_faces_normals(size_t A, size_t B) {
	assert(A < B and B < file_lines.size());

	// current material's name
	string current_material;
	char buf[256];
	// the current face should be smoothed
	bool smooth_face = false;
	// amount of smoothed faces
	size_t S = 0;

	// face information
	int NI[4], VN[4], TC[4];
	// vertex information
	float x, y, z;
	// texture coordinate
	float u, v;

	for (size_t i = A; i <= B; ++i) {
		if (file_lines[i][0] == 'v') {
			// parse vertex information
			if (file_lines[i][1] == ' ') {
				// vertex coordinate
				sscanf(file_lines[i].c_str(), "v %f %f %f", &x, &y, &z);
				vertices.push_back(vec3(x, y, z));
			}
			else if (file_lines[i][1] == 'n') {
				// vertex normal vector
				sscanf(file_lines[i].c_str(), "vn %f %f %f", &x, &y, &z);
				normals.push_back(glm::normalize(vec3(x, y, z)));
			}
			else if (file_lines[i][1] == 't') {
				// vertex texture coordinate
				sscanf(file_lines[i].c_str(), "vt %f %f", &u, &v);
				textures_coords.push_back(vec2(u, v));
			}
		}
		else if (file_lines[i][0] == 'u' and file_lines[i][1] == 's' and file_lines[i][2] == 'e') {
			// use material
			sscanf(file_lines[i].c_str(),"usemtl %s", buf);
			current_material = string(buf);
		}
		else if (file_lines[i][0] == 'f' and file_lines[i][1] == ' ') {

			TC[0] = TC[1] = TC[2] = TC[3] = -1;
			VN[0] = VN[1] = VN[2] = VN[3] = -1;
			NI[0] = NI[1] = NI[2] = NI[3] = -1;

			// parse face information
			int C = count(file_lines[i].begin(), file_lines[i].end(),' ');
			if (C == 3) {
				// triangle
				if (file_lines[i].find("//") != string::npos) {
					sscanf(
						file_lines[i].c_str(),
						"f %d//%d %d//%d %d//%d",
						&VN[0],&NI[0], &VN[1],&NI[1], &VN[2],&NI[2]
					);
				}
				else if (file_lines[i].find("/") != string::npos) {
					sscanf(
						file_lines[i].c_str(),
						"f %d/%d/%d %d/%d/%d %d/%d/%d",
						&VN[0],&TC[0],&NI[0], &VN[1],&TC[1],&NI[1], &VN[2],&TC[2],&NI[2]
					);
				}
				else {
					sscanf(file_lines[i].c_str(),"f %d %d %d",&VN[0],&VN[1],&VN[2]);
				}
				faces.push_back(face(NI,VN,TC,current_material,false,smooth_face));
			}
			else if (C == 4) {
				// quad
				if (file_lines[i].find("//") != string::npos) {
					sscanf(
						file_lines[i].c_str(),
						"f %d//%d %d//%d %d//%d %d//%d",
						&VN[0],&NI[0], &VN[1],&NI[1], &VN[2],&NI[2], &VN[3],&NI[3]
					);
				}
				else if (file_lines[i].find("/") != string::npos) {
					sscanf(
						file_lines[i].c_str(),
						"f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d",
						&VN[0],&TC[0],&NI[0], &VN[1],&TC[1],&NI[1], &VN[2],&TC[2],&NI[2], &VN[3],&TC[3],&NI[3]
					);
				}
				else {
					sscanf(file_lines[i].c_str(),"f %d %d %d %d",&VN[0],&VN[1],&VN[2],&VN[3]);
				}
				faces.push_back(face(NI,VN,TC,current_material,true,smooth_face));
			}
			else {
				cerr << "OBJ_reader::get_vertices_faces_normals: Error" << endl;
				cerr << "    Line " << i << " does not contain a triangle or a quad" << endl;
				cerr << "    Found: " << file_lines[i] << endl;
			}

			// if the current face needs smoothing
			// increment amount of smoothed faces
			if (smooth_face) {
				++S;
			}
		}
		else if (file_lines[i][0] == 's' and file_lines[i][1] == ' ') {
			if (file_lines[i][2] == '1') {
				smooth_face = true;
			}
				else if (file_lines[i][2] == 'o') {
				smooth_face = false;
			}
		}
	}
}

bool OBJ_reader::load_object(const string& dir, const string& fname, mesh& M) {
	directory = dir;
	filename = fname;

	#if defined (DEBUG)
	cout << "OBJ_reader::load_object: Loading file '" << directory + filename << "'" << endl;
	#endif

	if (not load_file()) {
		cerr << "OBJ_reader::load_object: Error" << endl;
		cerr << "    File '" << filename << "' could not be loaded" << endl;
		return false;
	}

	// Find the line starting with 'mtl'
	size_t aux = 0;
	while (file_lines[aux].substr(0,3) != "mtl") {
		++aux;
	}
	// We have found the name of the material and its file name.
	// Read the material file now
	if (file_lines[aux].substr(0,3) == "mtl") {
		// parse the file name using sscanf
		char mtl_filename[256];
		sscanf(file_lines[aux].c_str(), "mtllib %s", mtl_filename);

		// parse the material file
		bool r = load_material(mtl_filename);
		if (not r) {
			cerr << "OBJ_reader::load_object: Warning" << endl;
			cerr << "    Material file '" << mtl_filename << "' was found" << endl;
			cerr << "    at line " << aux << " but could not be read." << endl;
		}
	}
	// Reading object name.
	// If this name can't be read -> error
	++aux;
	char buffer_object_name[128];
	if (file_lines[aux][0] == 'o') {
		sscanf(file_lines[aux].c_str(),"o %s", buffer_object_name);
		object_name = buffer_object_name;
	}
	else {
		cerr << "OBJ_reader::load_object: Error" << endl;
		cerr << "    Bad .obj file format." << endl;
		cerr << "    At line " << aux << " expected 'o %s', but found" << endl;
		cerr << "        '" << file_lines[aux] << "'" << endl;
		return false;
	}

	#if defined (DEBUG)
	cout << "OBJ_reader::load_object: Parsing object '" << object_name << "'" << endl;
	cout << "    Object '" << object_name << "' is described from line " << endl;
	cout << "    " << aux << " to line " << file_lines.size() << endl;
	#endif

	// parse the vertices and faces information
	get_vertices_faces_normals(4, file_lines.size() - 1);

	M.set_name(object_name);

	// Vertices are denoted as indices greater than 0
	// we want them greater than or EQUAL TO 0.
	// Same for normal indices and txeture coordinates
	for (size_t i = 0; i < faces.size(); ++i) {
		int lim = (faces[i].is_quad ? 4 : 3);
		for (int j = 0; j < lim; ++j) {
			--faces[i].vertex_index[j];
			--faces[i].normal_index[j];
			--faces[i].text_coord[j];
		}
	}

	M.set_vertices(vertices);
	M.set_faces(faces);
	M.set_normals(normals);

	M.set_materials(materials);
	M.set_texture_coords(textures_coords);
	M.set_texture_indexes(textures_indexes);

	#if defined (DEBUG)
	cout << "OBJ_reader::load_object: Object '" << object_name << "'" << endl;
	cout << "    from file '" << filename << "'" << endl;
	cout << "    was loaded successfully" << endl;

	bool valid = M.is_valid();
	if (not valid) {
		return false;
	}
	#endif

	clean();

	return true;
}
