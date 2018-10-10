#pragma once

#include <GL/glu.h>
#include <GL/gl.h>

#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <string>
#include <cmath>
#include <list>
using namespace std;

struct coordinate {
	float x, y, z;
	bool alreadySmoothed;
	bool Null;	// if (Null) {'La normal no té smooth'} else {'La normal té smooth'}
	coordinate(float a, float b, float c, bool n);
	coordinate(float a, float b, float c);
};

struct texture {
	float u, v;
	texture(float a, float b);
};

struct face {
	string M_ID;
	bool four; 		// ens indica si té quatre vèrtexs
	bool smooth; 	// ens indica si hem de fer smooth als vertexs de la cara
	int normalIndex;
	int vertexNumber[4];
	int textureCoordinate[4];
	face(int nI, int v1, int v2, int v3, 
		 int t1, int t2, int t3, string mat_id,bool s);
		 
	face(int nI, int v1, int v2, int v3, int v4,
		 int t1, int t2, int t3, int t4, string mat_id,bool s);
};

struct material {
	string ID;
	int textureID;
	int illum;
	float Ns, Ni, d;
	float Ka[3], Kd[3], Ks[3];
	material(float* amb, float* dif, float* spec, float ns, 
			  float ni,float D, int ill, int textID, string id);
};

class OBJ_reader {
	private:
		vector<string*> coord;
		vector<coordinate*> vertex;
		vector<coordinate*> normals;
		vector<coordinate> smoothedNormals;
		vector<face*> faces;
		vector<material*> materials;
		vector<texture*> textures;
		
		vector<unsigned int> textures_indexes;
		vector<unsigned int> list_indexes;
		
		bool isnormals; // ens indica si hi ha normals
		bool issmooth;  // ens indica si hi ha smooth
		
		void clean();
		void TEST();
		
		bool loadFile(const char* filtOBJ);
		void loadMTL(const char* fileMTL);
		void smoothNormalsFaceByFace(int fN, int fS, int fE);
		int getVertexsFacesNormals(int A, int B);
		int getObject(int C, int D);
		
	public:
		OBJ_reader();
		~OBJ_reader();
		
		// Pre: filename és el path del fitxer *.obj que conté UN ÚNIC OBJECTE
		// Post: retorna un enter més gran o igual a 1 que representa l'objecte
		//		retorna -1 en cas que no es trobessin els fitxers *.obj, *.mtl o les textures.
		int loadObject(const char* filename);
		
		// Pre: filename és el path del fitxer que conté un o més objectes
		// Post: retorna una llista de identificadors que representen els diferents objectes
		// 		en cas d'error retorna una llista buida
		// *NOTA*: sembla que hi ha alguna cosa que no funciona bé: depenent del model la
		// 		llum no està bé i els models no s'acaben de veure.
		list<int> loadObjects(const char* filename);
};

