#pragma once
#pragma comment(lib,"../lib/glew32.lib")
#include "../tools/GL/glew.h"
#pragma comment (lib,"../lib/freeglut.lib")
#include "../tools/GL/freeglut.h"
#pragma comment (lib,"../lib/FreeImage.lib")
#include "../tools/FreeImage/FreeImage.h"
#include <string>
#include <stdio.h>
#include <iostream>
#include <fstream>

using namespace std;




class volumen
{
	public:
		string archivo;
		int BITS;
		GLuint XDIM,YDIM,ZDIM;
		float distX,distY,distZ;
		GLuint *texX,*texY,*texZ;
		bool activa;
		volumen();
		~volumen();
		void Init(string fname);
		bool CargarVolumen();
		void dibujar(bool shader);
};