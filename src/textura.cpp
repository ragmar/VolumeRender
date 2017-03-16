#pragma once
#include "textura.h"

	
	textura::textura(){
		activa=true;
	}
	textura::~textura(){
	
	}

	void textura::Init(string fname){
		archivo=fname;
	}

	void textura::CargarTextura2D(){
		FREE_IMAGE_FORMAT fifmt = FreeImage_GetFileType(archivo.c_str(), 0);

		FIBITMAP *imagen = FreeImage_Load(fifmt, archivo.c_str(),0);

		FIBITMAP* temp = imagen;
        imagen = FreeImage_ConvertTo32Bits(temp);
        FreeImage_Unload(temp);

		if( imagen != NULL )
		{
			
			int w = FreeImage_GetWidth(imagen);
			int h = FreeImage_GetHeight(imagen);

			glGenTextures(1, &tex2D);
			glBindTexture(GL_TEXTURE_2D, tex2D);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);


			GLubyte* bits = new GLubyte[4*w*h];
			char* pixeles = (char*)FreeImage_GetBits(imagen);
			
			for(int j= 0; j<w*h; j++){
				bits[j*4+0]= pixeles[j*4+2];
				bits[j*4+1]= pixeles[j*4+1];
				bits[j*4+2]= pixeles[j*4+0];
				bits[j*4+3]= pixeles[j*4+3];
			}
        

			glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA, w, h, 0, GL_RGBA,GL_UNSIGNED_BYTE,(GLvoid*)bits);
			FreeImage_Unload(imagen);
			delete bits;
		}
	}

	void textura::CargarTextura1D(GLubyte *colores,int bits){
			glGenTextures( 1, &tex1D );
			glBindTexture( GL_TEXTURE_1D, tex1D );
			glTexParameteri(GL_TEXTURE_1D,GL_TEXTURE_WRAP_S,GL_REPEAT);
			glTexParameteri(GL_TEXTURE_1D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
			glTexParameteri(GL_TEXTURE_1D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
			
			if(bits==256)
				glTexImage1D(GL_TEXTURE_1D,0,GL_RGBA,256,0,GL_RGBA,GL_UNSIGNED_BYTE,colores);
			if(bits==65536)
				glTexImage1D(GL_TEXTURE_1D,0,GL_RGBA,65536,0,GL_RGBA,GL_UNSIGNED_BYTE,colores);
	}




	void textura::Bind2D(){
		glBindTexture( GL_TEXTURE_2D, tex2D );
	}

	void textura::Bind1D(){
		glBindTexture( GL_TEXTURE_1D, tex1D );
	}
