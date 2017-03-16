#pragma once
#pragma comment (lib,"../lib/freeglut.lib")
#pragma comment (lib,"../lib/glew32.lib")
#include "../tools/GL/glew.h"
#include "../tools/GL/freeglut.h"
#include <iostream>
#include <string>


using namespace std;

class Shader
{
	public:
		bool activo;
		GLhandleARB program;
		Shader();
		~Shader();
		void createShader(char* vFileName,char* fFileName, string name);
		void destroyShader();
		void useShader(bool usar);
		bool initShader();
	//	void setUniformVec3( Vector3D <float> vec3D, char *name );
		void setUniformInt( int val, char *name);

	private:
		string shaderName;
		GLhandleARB vShader,fShader;
		char* vFile;
		char* fFile;
		void createProgram();
		bool checkError(GLenum shaderType);
		char* readTextFile( char *archivo );

};
