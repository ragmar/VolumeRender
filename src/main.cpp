#pragma comment (lib,"../lib/freeglut.lib")
#pragma comment (lib,"../lib/AntTweakBar.lib")
#pragma comment (lib,"../lib/glew32.lib")
#include "../tools/GL/glew.h"
#include "../tools/GL/freeglut.h"
#include "../tools/AntTweakBar/AntTweakBar.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include "Quaternion.h"
#include "shader.h"
#include "textura.h"
#include "volumen.h"


#define SQRT3 1.7320508f
#define CantidadInterfaz 6

Shader colorearVol;

using namespace std;
char documento[200];


struct point
{
	float x;
	float y;
};

struct color
{
	point pos;
	float r,g,b,a;
};

float XT=0.0f,YT=0.0f,ZT=0.0f,YE=0.0f,XR=0.0f,YR=0.0f;
bool rotar, trasladar,trasladarY,escalarX,escalarY,escalarZ,slideR,slideG,slideB,dot,dibujarVolumen;
float rotacion[3],traslacion[3],escalamiento[3];

GLint w;
GLint h;

float alturaSlide[3];
point transparencia[2];
point slider;
point rangoTransparencia;
point rangoSeleccionador;

volumen vol;
GLubyte *textColor;
textura color1D;
textura interfaz[CantidadInterfaz];
vector <color> puntos;
unsigned int seleccionado;


#include "controles.h"

GLvoid idle() {

	glutPostRedisplay();
}

void Draw()
{
	glPushMatrix();
		float Matrix[16];
		glTranslatef(traslacion[0],traslacion[1],traslacion[2]);
		glScalef(escalamiento[0],escalamiento[1],escalamiento[2]);
		Quaternion XYZ,aux;
		aux.CreateFromAxisAngle(0.0f,1.0f,0.0f,-rotacion[1]);
		XYZ.Mult(aux);
		aux.CreateFromAxisAngle(1.0f,0.0f,0.0f,-rotacion[0]);
		XYZ.Mult(aux);
		aux.CreateFromAxisAngle(0.0f,0.0f,1.0f,-rotacion[2]);
		XYZ.Mult(aux);
		XYZ.CreateMatrix(Matrix);
		glMultMatrixf(Matrix);
		
		colorearVol.useShader(true);

			glActiveTexture(GL_TEXTURE0_ARB);
			glEnable(GL_TEXTURE_1D);
			int texture1DLocation = glGetUniformLocationARB(colorearVol.program, "tex1D");
			glUniform1iARB(texture1DLocation, 0);
			color1D.Bind1D();

			glActiveTexture(GL_TEXTURE1_ARB);
			glEnable(GL_TEXTURE_2D);
			int texture2DLocation = glGetUniformLocationARB(colorearVol.program,"tex2D");
			glUniform1iARB(texture2DLocation, 1);

			vol.dibujar(true);

			glActiveTexture(GL_TEXTURE1_ARB);
			glBindTexture(GL_TEXTURE_2D, 0);	
			glDisable(GL_TEXTURE_2D);

			glActiveTexture(GL_TEXTURE0_ARB);
			glBindTexture(GL_TEXTURE_1D, 0);
			glDisable(GL_TEXTURE_1D);

		colorearVol.useShader(false);

	glPopMatrix();
}


void camara()
{
	glLoadIdentity();
	glTranslated(0.0f, 0.0f, -5.0f);
}
void renderScene(void) {//renderizar

	glClearColor(0.0, 0.0, 0.0, 0.0);	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor4f(1.0f,1.0f,1.0f,1.0f);

	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);
	camara();
	if(dibujarVolumen){
		Draw();
	}
	glLoadIdentity();
	setOrthographicProjection();
	glDisable(GL_DEPTH_TEST);
	DrawFuncion();	
	restorePerspectiveProjection();
	TwDraw();
	glutSwapBuffers();
	
	GLenum error = glGetError(); 
	
	if (error != GL_NO_ERROR){
		std::cout << "[OpenGL ERROR]: " << error << " " << gluErrorString(error) << std::endl;
//		assert(error != GL_NO_ERROR);
		system("pause");
		exit(0);
	}
}


void reshape(int x, int y){//redimensiono la panatlla
	w = x;
	h = y;
	glViewport(0, 0, w, h);	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, ((GLfloat) w) / ((GLfloat) h), 1.0f, 1500.0f);
	glMatrixMode(GL_MODELVIEW);
	TwWindowSize(w, h);
	glLoadIdentity();
}




void TW_CALL pressExit(void *clientData)
{ 
	TwTerminate();
	exit(0);
}



void TW_CALL CargarObjeto(void *clientData){
	char salvar[300];
	strcpy_s(salvar,"../files/Modelos/");
	strcat_s(salvar,documento);
	strcat_s(salvar,".raw");
	vol.Init(salvar);
	if(vol.CargarVolumen()){
		dibujarVolumen=true;
		seleccionado=0;
		color aux;
		aux.pos.x = puntos[puntos.size()-1].pos.x;
		aux.pos.y = puntos[puntos.size()-1].pos.y;
		aux.r = puntos[puntos.size()-1].r;
		aux.g = puntos[puntos.size()-1].g;
		aux.b = puntos[puntos.size()-1].b;
		aux.a = puntos[puntos.size()-1].a;
		unsigned int tam=puntos.size();
		for(unsigned int i=1;i<tam;i++)
		{
			puntos.pop_back();
		}
		puntos.push_back(aux);
	}else{
		seleccionado=0;
		color aux;
		aux.pos.x = puntos[puntos.size()-1].pos.x;
		aux.pos.y = puntos[puntos.size()-1].pos.y;
		aux.r = puntos[puntos.size()-1].r;
		aux.g = puntos[puntos.size()-1].g;
		aux.b = puntos[puntos.size()-1].b;
		aux.a = puntos[puntos.size()-1].a;
		unsigned int tam=puntos.size();
		for(unsigned int i=1;i<tam;i++)
		{
			puntos.pop_back();
		}
		puntos.push_back(aux);
		dibujarVolumen=false;
	}
	actColor();
}

void TW_CALL pressCargar(void *clientData)
{ 
	char abrir[300];
	strcpy_s(abrir,"../files/");
	strcat_s(abrir,documento);
	strcat_s(abrir,".42");
	ifstream entrada;
	entrada.open(abrir,ios::in | ios::binary);
	if(!entrada.is_open()){
		cout<<"No se pudo abrir el archivo .raw"<<endl;
		return;
	}
	seleccionado=0;
	int points;
	entrada>>vol.archivo;
	entrada>>vol.XDIM>>vol.YDIM>>vol.ZDIM>>vol.BITS;
	entrada>>points;
	color aus;
	unsigned int tam=puntos.size();
	for(unsigned int i=0;i<tam;i++)
	{
		puntos.pop_back();
	}
	for(int i=0;i<points;i++)
	{
		entrada>>aus.pos.x>>aus.pos.y>>aus.r>>aus.g>>aus.b>>aus.a;
		puntos.push_back(aus);
	}
	entrada>>rotacion[0]>>rotacion[1]>>rotacion[2];
	entrada>>traslacion[0]>>traslacion[1]>>traslacion[2];
	entrada>>escalamiento[0]>>escalamiento[1]>>escalamiento[2];
	vol.CargarVolumen();
	entrada.close();
	actColor();
	glutPostRedisplay();
}

void TW_CALL pressGuardar(void *clientData)
{ 
	FILE * Abrir;
	char salvar[90];
	strcpy_s(salvar,"../files/");
	strcat_s(salvar,documento);
	strcat_s(salvar,".42");
	Abrir=fopen(salvar,"w");
	if(Abrir)
	{
		fprintf(Abrir,"%s\n",vol.archivo.c_str());
		fprintf(Abrir,"%i %i %i %i\n",vol.XDIM,vol.YDIM,vol.ZDIM,vol.BITS);
		fprintf(Abrir,"%i\n",puntos.size());
		for(unsigned int i=0;i<puntos.size();i++)
		{
			fprintf(Abrir,"%f %f %f %f %f %f\n",puntos[i].pos.x,puntos[i].pos.y,puntos[i].r,puntos[i].g,puntos[i].b,puntos[i].a);	
		}
		fprintf(Abrir,"%f %f %f\n",rotacion[0],rotacion[1],rotacion[2]);
		fprintf(Abrir,"%f %f %f\n",traslacion[0],traslacion[1],traslacion[2]);
		fprintf(Abrir,"%f %f %f\n",escalamiento[0],escalamiento[1],escalamiento[2]);
		fclose (Abrir);
	}
	glutPostRedisplay();
}

void init_TW()
{
		TwInit(TW_OPENGL, NULL);
		TwWindowSize(w,h);
		glutPassiveMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);
		TwGLUTModifiersFunc(glutGetModifiers);
		TwBar *myBar;		
		myBar = TwNewBar("Opciones");
		TwAddVarRW(myBar,"objeto",TW_TYPE_CSSTRING(sizeof(documento)),documento,"label='Nombre' group=Archivo");
		TwAddButton(myBar,"CargarObjeto", CargarObjeto,NULL,"label='Cargar Modelo' group='Archivo'");
		TwAddButton(myBar,"Guardar", pressGuardar,NULL,"label='Guardar Documento' group=Archivo");
		TwAddButton(myBar,"Cargar", pressCargar,NULL,"label='Cargar Documento' group=Archivo");
		TwAddButton(myBar,"Salir", pressExit,NULL,"label='Salir' group=Archivo");
}
void init()
{
	init_TW();

	glEnable (GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);//BACK FACE
	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST);//zbuffer
	glDepthFunc(GL_LEQUAL);

	interfaz[0].Init("../files/Texturas/Colores.png");
	interfaz[1].Init("../files/Texturas/selector.png");
	interfaz[2].Init("../files/Texturas/transparencia.png");
	interfaz[3].Init("../files/Texturas/efecto.png");
	interfaz[4].Init("../files/Texturas/c.png");
	interfaz[5].Init("../files/Texturas/cselector.png");
	color1D.CargarTextura1D(textColor,256);
	for(int i=0;i<CantidadInterfaz; i++) interfaz[i].CargarTextura2D();
	vol.Init("../files/Modelos/Bucky32x32x32.raw");
	dibujarVolumen=false;
	if(vol.CargarVolumen())
		dibujarVolumen=true;

	initFunc();
	colorearVol.initShader();
	colorearVol.createShader("../files/Shader/VolVS.vert","../files/Shader/VolFS.frag", "volumen");
	actColor();
}

//Main Principal
int main(int argc, char *argv[])
{	
	w = 900;
	h = 650;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0,0);
	glutInitWindowSize(w,h);
	glutCreateWindow("F.C.G. - Volume Rendering - Alejandro Sans && Victor Felipe");
	init();
	glutDisplayFunc(renderScene);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(teclado);
	glutKeyboardUpFunc(tecladoUp);
	glutMouseFunc(clickMouse);
	glutMotionFunc(movimientoMouse);
	glutMainLoop();
	return 0;
}
