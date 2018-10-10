#include "OBJ_reader.hpp"

#ifndef INICI
#define INICI 1
#endif

coordinate::coordinate(float a, float b, float c) {
	x = a; y = b; z = c;
	alreadySmoothed = false;
}

coordinate::coordinate(float a, float b, float c, bool n) {
	Null = n;
	x = y = z = 0.0;
}

texture::texture(float a, float b) {
	u = a; v = b;
}

face::face(int nI, int v1, int v2, int v3, int t1, int t2, int t3, string mat_id, bool s) {
	normalIndex = nI;
	M_ID = mat_id;
	vertexNumber[0] = v1;
	vertexNumber[1] = v2;
	vertexNumber[2] = v3;
	vertexNumber[3] = -1;
	
	textureCoordinate[0] = t1;
	textureCoordinate[1] = t2;
	textureCoordinate[2] = t3;
	textureCoordinate[3] = -1;
	four = false;
	smooth = s;
}

face::face(int nI, int v1, int v2, int v3, int v4, int t1, int t2, int t3, int t4, string mat_id, bool s) {
	normalIndex = nI;
	M_ID = mat_id;
	vertexNumber[0] = v1;
	vertexNumber[1] = v2;
	vertexNumber[2] = v3;
	vertexNumber[3] = v4;
	
	textureCoordinate[0] = t1;
	textureCoordinate[1] = t2;
	textureCoordinate[2] = t3;
	textureCoordinate[3] = t4;
	four = true;
	smooth = s;
}

material::material(float* amb, float* dif, float* spec, float ns, float ni, float D, int ill, int textID, string id) {
	ID = id;
	Ns = ns;
	Ni = ni;
	d =  D;
	illum = ill;
	textureID = textID;
	Ka[0] = amb[0];
	Ka[1] = amb[1];
	Ka[2] = amb[2];
	
	Kd[0] = dif[0];
	Kd[1] = dif[1];
	Kd[2] = dif[2];
	
	Ks[0] = spec[0];
	Ks[1] = spec[1];
	Ks[2] = spec[2];
}

OBJ_reader::OBJ_reader() {
	isnormals = false;
	issmooth = false;
}

void OBJ_reader::clean() {
	for (int i = 0; i < coord.size(); ++i) delete coord[i];
	for (int i = 0; i < textures.size(); ++i) delete textures[i];
	for (int i = 0; i < vertex.size(); ++i) delete vertex[i];
	for (int i = 0; i < faces.size(); ++i) delete faces[i];
	for (int i = 0; i < normals.size(); ++i) delete normals[i];
	for (int i = 0; i < materials.size(); ++i) delete materials[i];

	coord.clear();
	textures.clear();
	materials.clear();
	vertex.clear();
	faces.clear();
	normals.clear();
	smoothedNormals.clear();

	isnormals = false;
	issmooth = false;
}

OBJ_reader::~OBJ_reader() {
	clean();
	vector<unsigned int>::iterator it;
	for (it = textures_indexes.begin(); it != textures_indexes.end(); ++it) glDeleteTextures(INICI,&(*it));
	for (it = list_indexes.begin(); it != list_indexes.end(); ++it) glDeleteLists(*it,INICI);
}

void OBJ_reader::TEST() {
	cout << "Test: " << endl;
	cout << "	Nº Vertexs= " << vertex.size() << endl;
	cout << "	Nº Materials= " << materials.size() << endl;
	cout << "	Nº Normals= " << normals.size() << endl;
	cout << "	Nº Textures= " << textures.size() << endl;
	cout << "	Nº Faces= " << faces.size() << endl;
	cout << "	Nº Textures= " << textures_indexes.size() << endl << endl;
	
	cout << "Materials: " << endl;
	for (int i = 0; i < materials.size(); ++i) {
		cout << "	newmtl " << materials[i]->ID << endl;
		cout << "		Ns " << materials[i]->Ns;
		cout << endl << "		Ka";
		for (int j = 0; j < 3; ++j) cout << " " << materials[i]->Ka[j];
		cout << endl << "		Kd";
		for (int j = 0; j < 3; ++j) cout << " " << materials[i]->Kd[j];
		cout << endl << "		Ks";
		for (int j = 0; j < 3; ++j) cout << " " << materials[i]->Ks[j];
		cout << endl;
		cout << "		Ni " << materials[i]->Ni << endl;
		cout << "		illum " << materials[i]->illum << endl;
		cout << "		map_Kd " << materials[i]->textureID << endl;
		cout << endl;
	}
	
	int s = 0;
	for (int i = 0; i < smoothedNormals.size(); ++i) {
		if (not smoothedNormals[i].Null) ++s;
	}
	cout << "	Number of smoothed normals= " << s << endl << endl;
}

bool OBJ_reader::loadFile(const char* fileOBJ) {
	ifstream obj;
	obj.open(fileOBJ);
	if (not obj.is_open()) {
		cout << "ERROR OPENING OBJECT FILE '" << fileOBJ << "'" << endl;
		return false;
	}
	char buf[256];
	while (not obj.eof()) {
		obj.getline(buf, 256);
		coord.push_back(new string(buf));
	}
	obj.close();
	return true;
}

void OBJ_reader::loadMTL(const char* fileMTL) {
	ifstream mtl;
	mtl.open(fileMTL);
	
	if (not mtl.is_open()) {
		cout << "ERROR OPENING MATERIAL FILE '" << fileMTL << "'" << endl;
		//return false;
		return;
	}
	
	vector<string*> mat;
	char buf[256];
	while (not mtl.eof()) {
		mtl.getline(buf, 256);
		mat.push_back(new string(buf));
	}
	mtl.close();
	
	string materialName;
	bool valid_material = false;
	float amb[3], dif[3], spec[3];
	float Ns, Ni, d;
	int illum;
	int textID = -1;
	
	for (int i = 0; i < mat.size(); ++i) {
		if ((*mat[i])[0] != '#') {
			if ((*mat[i])[0] == 'n' and (*mat[i])[1] == 'e' and (*mat[i])[2] == 'w') {
				if (valid_material) materials.push_back(new material(amb, dif, spec, Ns, Ni, d, illum, textID, materialName));
				valid_material = true;
				sscanf(mat[i]->c_str(),"newmtl %s", buf);
				materialName = buf;
				textID = -1;
			}
			else if ((*mat[i])[0] == 'N' and (*mat[i])[1] == 's') sscanf(mat[i]->c_str(), "Ns %f", &Ns);
			else if ((*mat[i])[0] == 'K' and (*mat[i])[1] == 'a') sscanf(mat[i]->c_str(), "Ka %f %f %f", &amb[0], &amb[1], &amb[2]);
			else if ((*mat[i])[0] == 'K' and (*mat[i])[1] == 'd') sscanf(mat[i]->c_str(), "Kd %f %f %f", &dif[0], &dif[1], &dif[2]);
			else if ((*mat[i])[0] == 'K' and (*mat[i])[1] == 's') sscanf(mat[i]->c_str(), "Ks %f %f %f", &spec[0], &spec[1], &spec[2]);
			else if ((*mat[i])[0] == 'N' and (*mat[i])[1] == 'i') sscanf(mat[i]->c_str(), "Ni %f", &Ni);
			else if ((*mat[i])[0] == 'd' and (*mat[i])[1] == ' ') sscanf(mat[i]->c_str(), "d %f", &d);
			else if ((*mat[i])[0] == 'i' and (*mat[i])[1] == 'l') sscanf(mat[i]->c_str(), "illum %d", &illum);
			else if ((*mat[i])[0] == 'm' and (*mat[i])[1] == 'a') {
				char aux[100];
				sscanf(mat[i]->c_str(), "map_Kd %s", aux);
				
				/*string fileTXT = aux;
				int k = fileTXT.size() - 1;
				while (k > 0 and fileTXT[k] != '/') --k;
				
				if (k == 0) textID = loadTXT(fileTXT.c_str());
				else textID = loadTXT((fileTXT.substr(k + 1, fileTXT.size())).c_str());*/
				
				if (textID == 0) {
					cout << "	Error in file: " << fileMTL << endl;
					cout << "		Cannot load texture" << endl;
					cout << "		Image '" << aux << "' could not be loaded" << endl << endl;
					//return false;
				}
			}
		}
	}
	materials.push_back(new material(amb, dif, spec, Ns, Ni, d, illum, textID, materialName));
	
	for (int i = 0; i < mat.size(); ++i) delete mat[i];
	mat.clear();
	//return true;
}

void OBJ_reader::smoothNormalsFaceByFace(int faceNumber, int facesStart, int facesEnd) {
	int lim;
	if (faces[faceNumber]->four) lim = 4;
	else lim = 3;
	
	for (int k = 0; k < lim; ++k) {
		int V = faces[faceNumber]->vertexNumber[k];
		if (not vertex[V - 1]->alreadySmoothed) {
			float X = 0.0;
			float Y = 0.0;
			float Z = 0.0;
			int num = 0;
			for (int j = facesStart; j < facesEnd; ++j) {
				if (faces[j]->vertexNumber[0] == V or faces[j]->vertexNumber[1] == V or
					faces[j]->vertexNumber[2] == V or faces[j]->vertexNumber[3] == V) {
					
					X += normals[faces[j]->normalIndex - 1]->x;
					Y += normals[faces[j]->normalIndex - 1]->y;
					Z += normals[faces[j]->normalIndex - 1]->z;
					++num;
				}
			}
			X /= num;
			Y /= num;
			Z /= num;
			float d = sqrt(X*X + Y*Y + Z*Z);
			X /= d;
			Y /= d;
			Z /= d;
			
			smoothedNormals[V - 1] = coordinate(X, Y, Z);
			smoothedNormals[V - 1].Null = false;
			
			// fent això ens assegurem que no repetirem el procés per un 
			// sol vèrtex tantes vegades com a cares pertany
			vertex[V - 1]->alreadySmoothed = true;
		}
	}
}

// Pre: A < B, A >= 0, B < coord.size()
// Post: llegeix la informació necessària de l'estructura coord
// 		 i retorna el nombre de cares que tenen smooth
int OBJ_reader::getVertexsFacesNormals(int A, int B) {
	string current_material;
	char buf[256];
	bool smoothFace = false;
	int S = 0; // nombre de cares amb smooth
	
	for (int i = A; i <= B; ++i) {
		if ((*coord[i])[0] == 'v') {
			if ((*coord[i])[1] == ' ') {
				// vertex
				float x, y, z;
				sscanf(coord[i]->c_str(), "v %f %f %f", &x, &y, &z);
				vertex.push_back(new coordinate(x, y, z));
			}
			else if ((*coord[i])[1] == 'n') {
				// normal vector
				float x, y, z;
				sscanf(coord[i]->c_str(), "vn %f %f %f", &x, &y, &z);
				normals.push_back(new coordinate(x, y, z));
				isnormals = true;
			}
			else if ((*coord[i])[1] == 't') {
				// texture
				float a, b;
				sscanf(coord[i]->c_str(), "vt %f %f", &a, &b);
				textures.push_back(new texture(a, b));
			}
		}
		else if ((*coord[i])[0] == 'u' and (*coord[i])[1] == 's' and (*coord[i])[2] == 'e') {
			// use material
			sscanf(coord[i]->c_str(),"usemtl %s", buf);
			current_material = buf;
		}
		else if ((*coord[i])[0] == 'f' and (*coord[i])[1] == ' ') {
			int C = count(coord[i]->begin(), coord[i]->end(),' ');
			if (C == 3) {
				// TRIANGLE
				if (coord[i]->find("//") != string::npos) {
					int nI, v1, v2, v3;
					sscanf(coord[i]->c_str(),"f %d//%d %d//%d %d//%d",&v1,&nI,&v2,&nI,&v3,&nI);
					faces.push_back(new face(nI,v1,v2,v3,0,0,0,current_material,smoothFace));
				}
				else if (coord[i]->find("/") != string::npos) {
					int VN[3], TC[3], nI;
					sscanf(coord[i]->c_str(),"f %d/%d/%d %d/%d/%d %d/%d/%d",&VN[0],&TC[0],&nI,&VN[1],&TC[1],&nI,&VN[2],&TC[2],&nI);
					faces.push_back(new face(nI,VN[0],VN[1],VN[2],TC[0],TC[1],TC[2],current_material,smoothFace));
				}
				else {
					int a, b, c;
					sscanf(coord[i]->c_str(),"f %d %d %d",&a,&b,&c);
					faces.push_back(new face(-1,a,b,c,0,0,0,current_material,smoothFace));
				}
			}
			else if (C == 4) {
				// QUAD
				if (coord[i]->find("//") != string::npos) {
					int nI, v1, v2, v3, v4;
					sscanf(coord[i]->c_str(),"f %d//%d %d//%d %d//%d %d//%d",&v1,&nI,&v2,&nI,&v3,&nI,&v4,&nI);
					faces.push_back(new face(nI,v1,v2,v3,v4,0,0,0,0,current_material,smoothFace));
				}
				else if (coord[i]->find("/") != string::npos) {
					int VN[4], TC[4], nI;
					sscanf(coord[i]->c_str(),"f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d",&VN[0],&TC[0],&nI,&VN[1],&TC[1],&nI,&VN[2],&TC[2],&nI,&VN[3],&TC[3],&nI);
					faces.push_back(new face(nI,VN[0],VN[1],VN[2],VN[3],TC[0],TC[1],TC[2],TC[3],current_material,smoothFace));
				}
				else {
					int a, b, c, d;
					sscanf(coord[i]->c_str(),"f %d %d %d %d",&a,&b,&c,&d);
					faces.push_back(new face(-1,a,b,c,d,0,0,0,0,current_material,smoothFace));
				}
			}
			if (smoothFace) ++S;
		}
		else if ((*coord[i])[0] == 's' and (*coord[i])[1] == ' ') {
			if ((*coord[i])[2] == '1') smoothFace = true;
			else if ((*coord[i])[2] == 'o') smoothFace = false;
		 }
	}
	return S;
}

GLuint OBJ_reader::getObject(int C, int D) {
	GLuint num = glGenLists(INICI);
	glNewList(num, GL_COMPILE);
	glPushMatrix();
	
	for (int i = C; i < D; ++i) {
		/*   SET THE MATERIAL TO THE FACE   */
		bool textenable = false;
		// find the material color of the face
		int M = -1;
		for (int j = 0; j < materials.size(); ++j) {
			if (materials[j]->ID == faces[i]->M_ID) M = j;
		}
		if (M != -1) {
			float amb[] = {materials[M]->Ka[0],materials[M]->Ka[1],materials[M]->Ka[2], materials[M]->d};
			float dif[] = {materials[M]->Kd[0],materials[M]->Kd[1],materials[M]->Kd[2], materials[M]->d};
			float spec[] = {materials[M]->Ks[0],materials[M]->Ks[1],materials[M]->Ks[2], materials[M]->d};
			
			glMaterialfv(GL_FRONT,GL_DIFFUSE,dif);
			glMaterialfv(GL_FRONT,GL_AMBIENT,amb);
			glMaterialfv(GL_FRONT,GL_SPECULAR,spec);
			glMaterialf(GL_FRONT,GL_SHININESS,materials[M]->Ns);
			
			if (materials[M]->textureID == -1) glDisable(GL_TEXTURE_2D);
			else {
				textenable = true;
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, materials[M]->textureID);
			}
		}
		
		/*   BUILD THE FACE   */
		if (faces[i]->four) {
			glBegin(GL_QUADS);
				
				if (isnormals) {
					glNormal3f(normals[faces[i]->normalIndex - 1]->x,
							   normals[faces[i]->normalIndex - 1]->y,
							   normals[faces[i]->normalIndex - 1]->z);
				}
				
				if (textenable) {
					glTexCoord2f(textures[faces[i]->textureCoordinate[0] - 1]->u,
								1.0 - textures[faces[i]->textureCoordinate[0] - 1]->v);
				}
				
				if (not smoothedNormals[faces[i]->vertexNumber[0] - 1].Null) {
					glNormal3f(smoothedNormals[faces[i]->vertexNumber[0] - 1].x,
							   smoothedNormals[faces[i]->vertexNumber[0] - 1].y,
							   smoothedNormals[faces[i]->vertexNumber[0] - 1].z);
				}
				
				glVertex3f(vertex[faces[i]->vertexNumber[0] - 1]->x,
						   vertex[faces[i]->vertexNumber[0] - 1]->y,
						   vertex[faces[i]->vertexNumber[0] - 1]->z);
				
				if (textenable) {
					glTexCoord2f(textures[faces[i]->textureCoordinate[1] - 1]->u,
								1.0 - textures[faces[i]->textureCoordinate[1] - 1]->v);
				}
				
				if (not smoothedNormals[faces[i]->vertexNumber[1] - 1].Null) {
					glNormal3f(smoothedNormals[faces[i]->vertexNumber[1] - 1].x,
							   smoothedNormals[faces[i]->vertexNumber[1] - 1].y,
							   smoothedNormals[faces[i]->vertexNumber[1] - 1].z);
				}
				
				glVertex3f(vertex[faces[i]->vertexNumber[1] - 1]->x,
						   vertex[faces[i]->vertexNumber[1] - 1]->y,
						   vertex[faces[i]->vertexNumber[1] - 1]->z);
				
				if (textenable) {
					glTexCoord2f(textures[faces[i]->textureCoordinate[2] - 1]->u,
								1.0 - textures[faces[i]->textureCoordinate[2] - 1]->v);
				}

				if (not smoothedNormals[faces[i]->vertexNumber[2] - 1].Null) {
					glNormal3f(smoothedNormals[faces[i]->vertexNumber[2] - 1].x,
							   smoothedNormals[faces[i]->vertexNumber[2] - 1].y,
							   smoothedNormals[faces[i]->vertexNumber[2] - 1].z);
				}
				
				glVertex3f(vertex[faces[i]->vertexNumber[2] - 1]->x,
						   vertex[faces[i]->vertexNumber[2] - 1]->y,
						   vertex[faces[i]->vertexNumber[2] - 1]->z);
				
				if (textenable) {
					glTexCoord2f(textures[faces[i]->textureCoordinate[3] - 1]->u,
								1.0 - textures[faces[i]->textureCoordinate[3] - 1]->v);
				}
				
				if (not smoothedNormals[faces[i]->vertexNumber[3] - 1].Null) {
					glNormal3f(smoothedNormals[faces[i]->vertexNumber[3] - 1].x,
							   smoothedNormals[faces[i]->vertexNumber[3] - 1].y,
							   smoothedNormals[faces[i]->vertexNumber[3] - 1].z);
				}
				
				glVertex3f(vertex[faces[i]->vertexNumber[3] - 1]->x,
						   vertex[faces[i]->vertexNumber[3] - 1]->y,
						   vertex[faces[i]->vertexNumber[3] - 1]->z);
				
			glEnd();
		}
		else {
			glBegin(GL_TRIANGLES);
				
				if (isnormals) {
					glNormal3f(normals[faces[i]->normalIndex - 1]->x,
							   normals[faces[i]->normalIndex - 1]->y,
							   normals[faces[i]->normalIndex - 1]->z);
				}
				
				if (textenable) {
					glTexCoord2f(textures[faces[i]->textureCoordinate[0] - 1]->u,
								1.0 - textures[faces[i]->textureCoordinate[0] - 1]->v);
				}
				
				if (not smoothedNormals[faces[i]->vertexNumber[0] - 1].Null) {
					glNormal3f(smoothedNormals[faces[i]->vertexNumber[0] - 1].x,
							   smoothedNormals[faces[i]->vertexNumber[0] - 1].y,
							   smoothedNormals[faces[i]->vertexNumber[0] - 1].z);
				}
				
				glVertex3f(vertex[faces[i]->vertexNumber[0] - 1]->x,
						   vertex[faces[i]->vertexNumber[0] - 1]->y,
						   vertex[faces[i]->vertexNumber[0] - 1]->z);
				
				if (textenable) {
					glTexCoord2f(textures[faces[i]->textureCoordinate[1] - 1]->u,
								1.0 - textures[faces[i]->textureCoordinate[1] - 1]->v);
				}
				
				if (not smoothedNormals[faces[i]->vertexNumber[1] - 1].Null) {
					glNormal3f(smoothedNormals[faces[i]->vertexNumber[1] - 1].x,
							   smoothedNormals[faces[i]->vertexNumber[1] - 1].y,
							   smoothedNormals[faces[i]->vertexNumber[1] - 1].z);
				}
				
				glVertex3f(vertex[faces[i]->vertexNumber[1] - 1]->x,
						   vertex[faces[i]->vertexNumber[1] - 1]->y,
						   vertex[faces[i]->vertexNumber[1] - 1]->z);
				
				if (textenable) {
					glTexCoord2f(textures[faces[i]->textureCoordinate[2] - 1]->u,
								1.0 - textures[faces[i]->textureCoordinate[2] - 1]->v);
				}

				if (not smoothedNormals[faces[i]->vertexNumber[2] - 1].Null) {
					glNormal3f(smoothedNormals[faces[i]->vertexNumber[2] - 1].x,
							   smoothedNormals[faces[i]->vertexNumber[2] - 1].y,
							   smoothedNormals[faces[i]->vertexNumber[2] - 1].z);
				}
				
				glVertex3f(vertex[faces[i]->vertexNumber[2] - 1]->x,
						   vertex[faces[i]->vertexNumber[2] - 1]->y,
						   vertex[faces[i]->vertexNumber[2] - 1]->z);
				
			glEnd();
		}
	}
	glPopMatrix();
	glEndList();
	return num;
}

void OBJ_reader::scale_to_unit() {
	glm::vec3 center(0.0f, 0.0f, 0.0f);

	glm::vec3 m(1e10, 1e10, 1e10);
	glm::vec3 M(-1e10, -1e10, -1e10);

	for (unsigned int i = 0; i < vertex.size(); ++i) {
		center += glm::vec3(vertex[i]->x, vertex[i]->y, vertex[i]->z);

		m = glm::min(m, glm::vec3(vertex[i]->x, vertex[i]->y, vertex[i]->z));
		M = glm::max(M, glm::vec3(vertex[i]->x, vertex[i]->y, vertex[i]->z));
	}
	center /= vertex.size();

	float largestSize = max(M.x - m.x, max(M.y - m.y, M.z - m.z));

	for (unsigned int i = 0; i < vertex.size(); ++i) {
		vertex[i]->x = (vertex[i]->x - center.x)/largestSize;
		vertex[i]->y = (vertex[i]->y - center.y)/largestSize;
		vertex[i]->z = (vertex[i]->z - center.z)/largestSize;
	}
}

GLuint OBJ_reader::loadObject(const char* fileOBJ) {
	#ifdef DEBUG
	cout << "Loading file '" << fileOBJ << "'" << endl;
	#endif

	if (not loadFile(fileOBJ)) {
		cout << "File '" << fileOBJ << "' could not be loaded" << endl;
		return 0;
	}
	int aux = 0;
	while ((*coord[aux])[0] != 'm' and (*coord[aux])[1] != 't' and (*coord[aux])[2] != 'l') ++aux;
	if ((*coord[aux])[0] == 'm' and (*coord[aux])[1] == 't' and (*coord[aux])[2] == 'l') {
		char fileMTL[256];
		sscanf(coord[aux]->c_str(), "mtllib %s", fileMTL);
		//if (not loadMTL(fileMTL)) return -1;
		loadMTL(fileMTL);
		
		#ifdef DEBUG
		TEST();
		#endif
	}
	++aux;
	char objectname[128];
	if ((*coord[aux])[0] == 'o') sscanf(coord[aux]->c_str(),"o %s", objectname);
	
	#ifdef DEBUG
	cout << "Loading object '" << objectname << "'" << endl;
	cout << "	Object '" << objectname << "' is described from line " << 5 << " to line " << coord.size() << endl;
	#endif

	int S = getVertexsFacesNormals(4, coord.size() - 1);
	smoothedNormals = vector<coordinate>(vertex.size(), coordinate(0.0, 0.0, 0.0, true));
	
	if (vertex.size() == 0) {cout << "Error: vertexs not found in object' " << objectname << "'" << endl; return -1;}
	if (faces.size() == 0) {cout << "Error: faces not found in object '" << objectname << "'" << endl; return -1;}
	if (normals.size() == 0 and isnormals) {cout << "Error: normals not found in object '" << objectname << "'" << endl; return -1;}
	
	if (0 < S or S == faces.size()) {
		issmooth = true;
		for (int i = 0; i < faces.size(); ++i) {
			if (faces[i]->smooth) smoothNormalsFaceByFace(i, 0, faces.size());
		}
	}

	#ifdef DEBUG
	cout << "Object '" << objectname << "' from file '" << fileOBJ << "' was loaded successfully" << endl;
	#endif

	scale_to_unit();

	GLuint K = getObject(0, faces.size());
	clean();
	list_indexes.push_back(K);
	return K;
}

list<int> OBJ_reader::loadObjects(const char* fileOBJ) {
	list<int> L;

	#ifdef DEBUG
	cout << "Loading file '" << fileOBJ << "'" << endl;
	#endif

	if (not loadFile(fileOBJ)) {
		cout << "File '" << fileOBJ << "' could not be loaded" << endl;
		return L;
	}
	
	int aux = 0;
	while ((*coord[aux])[0] != 'm') ++aux;
	if ((*coord[aux])[0] == 'm' and (*coord[aux])[1] == 't' and (*coord[aux])[2] == 'l') {
		char fileMTL[128];
		sscanf(coord[aux]->c_str(), "mtllib %s", fileMTL);
		//if (not loadMTL(fileMTL)) return L;
		loadMTL(fileMTL);
		
		#ifdef DEBUG
		TEST();
		#endif
	}
	
	char objectname[128];
	
	int i = aux; // comencem amb la primera cara del primer objecte
	int C = 0;
	while (i <= coord.size()) {
		int j = i;
		while (j < coord.size() and (*coord[j])[0] != 'o') ++j;
		
		// j està al principi del següent objecte o al final de l'arxiu
		// principi objecte: "o ~nom_objecte~"
		
		if (j < coord.size()) {
			if ((*coord[j])[0] == 'o') sscanf(coord[j]->c_str(), "o %s", objectname);
			
			#ifdef DEBUG
			cout << "Loading object '" << objectname << "'" << endl;
			#endif
			
			++j;
			while (j < coord.size() and (*coord[j])[0] != 'o') ++j;
			--j;
	
			#ifdef DEBUG
			cout << "	Object '" << objectname << "' is described from line " << i + 1 << " to line " << j + 1 << endl;
			#endif
	
			int vauxsize = vertex.size();
			int fauxsize = faces.size();
			int nauxsize = normals.size();
			
			int S = getVertexsFacesNormals(i, j);
			smoothedNormals = vector<coordinate>(vertex.size(), coordinate(0.0,0.0,0.0,true));
			
			bool error = false;
			if (vertex.size() - vauxsize == 0) {cout << "Error: vertexs not found in object '" << objectname << "'" << endl; error = 1;}
			if (faces.size() - fauxsize == 0) {cout << "Error: faces not found in object '" << objectname << "'" << endl; error = 1;}
			if (normals.size() - nauxsize == 0 and isnormals) {cout << "Error: normals not found in object '" << objectname << "'" << endl; error = 1;}
			
			if (not error) {
				if (0 < S) {
					issmooth = true;
					for (int aux = C; aux < faces.size(); ++aux) {
						if (faces[aux]->smooth) smoothNormalsFaceByFace(aux, C, faces.size());
					}
				}
				int K = getObject(C, faces.size());
				C = faces.size();
				list_indexes.push_back(K);
				L.insert(L.end(), K);
				smoothedNormals.clear();
				
				#ifdef DEBUG
				cout << "Object '" << objectname << "' from file '" << fileOBJ << "' was loaded successfully" << endl << endl;
				#endif
			}

			isnormals = false;
			issmooth = false;
			if (error) cout << "Object '" << objectname << "' from file '" << fileOBJ << "' was not loaded" << endl << endl;
		}
		i = j + 1;
	}
	clean();
	cout << endl << "----------" << endl << endl;
	return L;
}






