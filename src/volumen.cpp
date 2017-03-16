#pragma once
#include "volumen.h"

	
	volumen::volumen(){
	}
	volumen::~volumen(){
	
	}

	void volumen::Init(string fname){
		archivo=fname;
	}
	
	bool volumen::CargarVolumen()
	{
		bool bits8;
		XDIM=-1, YDIM=-1, ZDIM=-1;
		ifstream entrada;
		entrada.open(archivo.c_str(),ios::in | ios::binary);
		if(!entrada.is_open()){
			cout<<"No se pudo abrir el archivo .raw"<<endl;
			return false;
		}
		int encontrados=0;
		int pos=archivo.size()-1;
		int aux=-1;
		while(encontrados!=3||pos<0)
		{
			if (archivo[pos]=='.')aux= pos;
			else if(archivo[pos]=='x') {
				++encontrados;
				if(encontrados==1)
				{
					string X;
					X=archivo.substr(pos+1,(aux-1)-(pos));
					ZDIM=atoi(X.c_str());
					aux=pos;
				}
				if(encontrados==2)
				{
					string X;
					X=archivo.substr(pos+1,(aux-1)-(pos));
					YDIM=atoi(X.c_str());
					aux=pos;
				}
					
			}else if(encontrados==2&&(archivo[pos]<'0'||archivo[pos]>'9'))
			{
					++encontrados;
					string X;
					X=archivo.substr(pos+1,(aux-1)-(pos));
					XDIM=atoi(X.c_str());
					aux=pos;
			}
			pos--;
		}
		if(XDIM==-1||YDIM==-1||ZDIM==-1) return false;

		const int size = XDIM*YDIM*ZDIM;
		GLubyte *volumen8=new GLubyte[size];
		GLushort *volumen16=new GLushort[size];

		//puntero al final y al principio para saber la cantidad de data que hay
		entrada.seekg (0, ios::end);
		long dataArchivo = entrada.tellg();
		entrada.seekg (0, ios::beg);
		entrada.close();

		FILE *pFile = fopen(archivo.c_str(),"rb");
		if(dataArchivo==(long)size){
			BITS=8;
			bits8=true;
			GLubyte *auxiliarLectura=new GLubyte[size];
			fread(auxiliarLectura,sizeof(GLubyte),size,pFile);
			memcpy(volumen8,auxiliarLectura,size);
			free(auxiliarLectura);
		}else if(dataArchivo==(long)size*2)
		{
			BITS=16;
			bits8=false;
			GLushort *auxiliarLectura=new GLushort[size];
			fread(auxiliarLectura,sizeof(GLushort),size,pFile);
			memcpy(volumen16,auxiliarLectura,size);
			free(auxiliarLectura);
		}else{ 
			cout<<"dimensiones erroneas"<<endl;
			return false;
		}

		texZ = (GLuint *)realloc(texZ,sizeof(GLuint)*ZDIM);
		texY = (GLuint *)realloc(texY,sizeof(GLuint)*YDIM);
		texX = (GLuint *)realloc(texX,sizeof(GLuint)*XDIM);


		//DEPTH
		GLubyte *bit8 = (BYTE *)malloc(sizeof(BYTE)*XDIM*YDIM);
		GLushort *bit16 = (GLushort *)malloc(sizeof(GLushort)*XDIM*YDIM);
		
		glGenTextures(ZDIM,texZ);
		
		int elem=0;
		for(unsigned int i=0;i<ZDIM;i++)
		{
			int cont=0;
			glBindTexture( GL_TEXTURE_2D, texZ[i]);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			for(unsigned int j=0;j<YDIM;j++){
				for(unsigned int k=0;k<XDIM;k++){
					if(bits8) bit8[cont++]=volumen8[elem++];
					else bit16[cont++]=volumen16[elem++];
				}
			}
			if(bits8) glTexImage2D( GL_TEXTURE_2D, 0, 1, XDIM, YDIM, 0,GL_LUMINANCE, GL_UNSIGNED_BYTE, bit8 );
			else glTexImage2D( GL_TEXTURE_2D, 0, 1, XDIM, YDIM, 0,GL_LUMINANCE, GL_UNSIGNED_SHORT, bit16 );
		}



//WIDTH
		glGenTextures(XDIM,texX);

		bit8 = (BYTE *)realloc(bit8,sizeof(BYTE)*ZDIM*YDIM);
		bit16 = (GLushort *)realloc(bit16,sizeof(GLushort)*ZDIM*YDIM);

		for(unsigned int i=0;i<XDIM;i++)
		{
			int cont=0;
			glBindTexture( GL_TEXTURE_2D, texX[i]);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			for(unsigned int j=0;j<ZDIM;j++){
				for(unsigned int k=0;k<YDIM;k++){
					if(bits8) bit8[cont++]=volumen8[i*XDIM+j*XDIM*YDIM+k];
					else bit16[cont++]=volumen16[i*XDIM+j*XDIM*YDIM+k];
				}
			}
			if(bits8) glTexImage2D( GL_TEXTURE_2D, 0, 1, YDIM, ZDIM, 0,GL_LUMINANCE, GL_UNSIGNED_BYTE, bit8 );
			else glTexImage2D( GL_TEXTURE_2D, 0, 1, YDIM, ZDIM, 0,GL_LUMINANCE, GL_UNSIGNED_SHORT, bit16 );
		}

		//HEIGTH
		glGenTextures(YDIM,texY);

		bit8 = (BYTE *)realloc(bit8,sizeof(BYTE)*ZDIM*XDIM);
		bit16 = (GLushort *)realloc(bit16,sizeof(GLushort)*ZDIM*XDIM);

		for(unsigned int i=0;i<YDIM;i++)
		{
			int cont=0;
			glBindTexture( GL_TEXTURE_2D, texY[i]);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			for(unsigned int j=0;j<ZDIM;j++){
				for(unsigned int k=0;k<XDIM;k++){
					if(bits8) bit8[cont++]=volumen8[i+j*XDIM*YDIM+k*XDIM];
					else bit16[cont++]=volumen16[i+j*XDIM*YDIM+k*XDIM];
				}
			}
			if(bits8) glTexImage2D( GL_TEXTURE_2D, 0, 1, XDIM, ZDIM, 0,GL_LUMINANCE, GL_UNSIGNED_BYTE, bit8 );
			else glTexImage2D( GL_TEXTURE_2D, 0, 1, XDIM, ZDIM, 0,GL_LUMINANCE, GL_UNSIGNED_SHORT, bit16 );
		}

		distZ = 2.0f/(float)ZDIM;
		distY = 2.0f/(float)YDIM;
		distX = 2.0f/(float)XDIM;


		free(bit8);
		free(bit16);
		free(volumen8);
		free(volumen16);

		/* textura 3D
		//load data into a 3D texture
		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_3D, tex);

		// set the texture parameters
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		if(bits==8)
		{
			GLubyte* pVolume=new GLubyte[size];
			fread(pVolume,sizeof(GLubyte),size,pFile);
			glTexImage3D(GL_TEXTURE_3D,0,GL_INTENSITY,XDIM,YDIM,ZDIM,0,GL_LUMINANCE,GL_UNSIGNED_BYTE,pVolume);
			delete [] pVolume;
		}else
		{
			GLushort* pVolume=new GLushort[size];
			fread(pVolume,sizeof(GLushort),size,pFile);		

			
			glTexImage3D(GL_TEXTURE_3D,0,GL_INTENSITY,XDIM,YDIM,ZDIM,0,GL_LUMINANCE,GL_UNSIGNED_SHORT,pVolume);
			delete [] pVolume;
		}
		fclose(pFile);
		*/
		
		return true;
	}

void	volumen::dibujar(bool shader)	
{
		float dist;
		glEnable(GL_BLEND);
		if(shader) glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		else glBlendFunc(GL_SRC_COLOR,GL_ONE_MINUS_SRC_COLOR);


		GLint viewport[4];
		GLdouble mvmatrix[16], projmatrix[16];
		GLdouble wx, wy, wz;
		GLdouble abswx,abswy,abswz;

		glGetIntegerv (GL_VIEWPORT, viewport);
		glGetDoublev (GL_MODELVIEW_MATRIX, mvmatrix);
		glGetDoublev (GL_PROJECTION_MATRIX, projmatrix);
		gluUnProject (viewport[2]/2.0f, viewport[3]/2.0f, 0.0f,
		mvmatrix, projmatrix, viewport, &wx, &wy, &wz);

		abswx=abs(wx), abswy=abs(wy), abswz=abs(wz);
		int i;
		if(abswz >= abswx && abswz >=abswy){
			if( wz>=0){
				dist=-1.0f;
				//frente
				for(i=ZDIM-1;i>=0;--i){
					glBindTexture(GL_TEXTURE_2D, texZ[i]);
					glBegin(GL_QUADS);
						glTexCoord2d(0,0);
						glVertex3f(-1.0f,-1.0f,dist);
						glTexCoord2d(0,1);
						glVertex3f(1.0f,-1.0f,dist);
						glTexCoord2d(1,1);
						glVertex3f(1.0f,1.0f,dist);
						glTexCoord2d(1,0);
						glVertex3f(-1.0f,1.0f,dist);
					glEnd();
					dist+=distZ;
				}
			}else{
				dist=1.0f;
				//atras
				glFrontFace(GL_CW);
				for(i=0;i<ZDIM;++i){
					glBindTexture(GL_TEXTURE_2D, texZ[i]);
					glBegin(GL_QUADS);
						glTexCoord2d(0,0);
						glVertex3f(-1.0f,-1.0f,dist);
						glTexCoord2d(0,1);
						glVertex3f(1.0f,-1.0f,dist);
						glTexCoord2d(1,1);
						glVertex3f(1.0f,1.0f,dist);
						glTexCoord2d(1,0);
						glVertex3f(-1.0f,1.0f,dist);
					glEnd();
					dist-=distZ;
				}
			}
		}else if(abswx >= abswz && abswx >=abswy){
			if( wx>=0){
				dist=-1.0f;
				//derecha
				for(i=0;i<XDIM;++i){
					glBindTexture(GL_TEXTURE_2D, texX[i]);
					glBegin(GL_QUADS);
						glTexCoord2d(0,0);
						glVertex3f(dist,-1.0f,1.0f);
						glTexCoord2d(0,1);
						glVertex3f(dist,-1.0f,-1.0f);
						glTexCoord2d(1,1);
						glVertex3f(dist,1.0f,-1.0f);
						glTexCoord2d(1,0);
						glVertex3f(dist,1.0f,1.0f);
					glEnd();
					dist+=distX;
				}
			}else{
				dist=1.0f;
				//izquierda
				glFrontFace(GL_CW);
				for(i=XDIM-1;i>=0;--i){
					glBindTexture(GL_TEXTURE_2D, texX[i]);
					glBegin(GL_QUADS);
						glTexCoord2d(0,0);
						glVertex3f(dist,-1.0f,1.0f);
						glTexCoord2d(0,1);
						glVertex3f(dist,-1.0f,-1.0f);
						glTexCoord2d(1,1);
						glVertex3f(dist,1.0f,-1.0f);
						glTexCoord2d(1,0);
						glVertex3f(dist,1.0f,1.0f);
					glEnd();
					dist-=distX;
				}
			}
		}else{
			if( wy>=0){
				dist=-1.0f;
				//arriba
				for(i=0;i<YDIM;++i){
					glBindTexture(GL_TEXTURE_2D, texY[i]);
					glBegin(GL_QUADS);
					glTexCoord2d(0,0);
					glVertex3f(-1.0f,dist,1.0f);
					glTexCoord2d(1,0);
					glVertex3f(1.0f,dist,1.0f);
					glTexCoord2d(1,1);
					glVertex3f(1.0f,dist,-1.0f);
					glTexCoord2d(0,1);
					glVertex3f(-1.0f,dist,-1.0f);
					glEnd();
					dist+=distY;
				}
			}else{
				dist=1.0f;
				//abajo
				glFrontFace(GL_CW);
				for(i=YDIM-1;i>=0;--i){
					glBindTexture(GL_TEXTURE_2D, texY[i]);
					glBegin(GL_QUADS);
					glTexCoord2d(0,0);
					glVertex3f(-1.0f,dist,1.0f);
					glTexCoord2d(1,0);
					glVertex3f(1.0f,dist,1.0f);
					glTexCoord2d(1,1);
					glVertex3f(1.0f,dist,-1.0f);
					glTexCoord2d(0,1);
					glVertex3f(-1.0f,dist,-1.0f);
					glEnd();
					dist-=distY;
				}
			}
		}
		glFrontFace(GL_CCW);

}