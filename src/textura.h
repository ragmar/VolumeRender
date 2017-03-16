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




class textura
{
	private:
		string archivo;
	public:
		GLuint tex2D,tex1D;
		bool activa;
		textura();
		~textura();
		void Init(string fname);
		void CargarTextura2D();
		void CargarTextura1D(GLubyte *colores,int bits);
		void Bind2D();
		void Bind1D();
};