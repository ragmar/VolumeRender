
#define TransparenciaInfY -0.60f
#define TransparenciaSupY -.25f
#define TransparenciaInfX 0.37f
#define TransparenciaSupX .98f

#define selectorInfX 0.415f
#define selectorSupX .91f
#define selectorRY -0.71f
#define selectorGY -0.82f
#define selectorBY -0.92f


GLvoid setOrthographicProjection() {

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(-1.0, 1.0,-1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
}

GLvoid restorePerspectiveProjection() {

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}


void DrawFuncion()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture( GL_TEXTURE_2D, 0 );
	glEnable (GL_BLEND); 
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//barra de colores
	interfaz[0].Bind2D();
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f,0.0f);
		glVertex2f(0.35f,-1.0f);
		glTexCoord2f(1.0f,0.0f);
		glVertex2f(1.0f,-1.0f);
		glTexCoord2f(1.0f,1.0f);
		glVertex2f(1.0f,TransparenciaInfY);	
		glTexCoord2f(0.0f,1.0f);
		glVertex2f(0.35f,TransparenciaInfY);	
	glEnd();

	interfaz[1].Bind2D();
	//seleccionadores
	for(int i=0;i<3;i++)
	{
		float color;
		if(i==0) color=puntos[seleccionado].r;
		else if(i==1) color=puntos[seleccionado].g;
		else if(i==2) color=puntos[seleccionado].b;
		float auxX=color*rangoSeleccionador.x + slider.x;
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f,0.0f);
			glVertex2f(auxX-0.035f,alturaSlide[i]-0.03f);
			glTexCoord2f(1.0f,0.0f);
			glVertex2f(auxX+0.035f,alturaSlide[i]-0.03f);
			glTexCoord2f(1.0f,1.0f);
			glVertex2f(auxX+0.035f,alturaSlide[i]+0.03f);	
			glTexCoord2f(0.0f,1.0f);
			glVertex2f(auxX-0.035f,alturaSlide[i]+0.03f);	
		glEnd();
	}

	//transparencia 
	interfaz[2].Bind2D();
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f,0.0f);
		glVertex2f(0.35f,TransparenciaInfY);
		glTexCoord2f(1.0f,0.0f);
		glVertex2f(1.0f,TransparenciaInfY);
		glTexCoord2f(1.0f,1.0f);
		glVertex2f(1.0f,-0.18f);	
		glTexCoord2f(0.0f,1.0f);
		glVertex2f(0.35f,-0.18f);	
	glEnd();


	interfaz[3].Bind2D();
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f,0.0f);
		glVertex2f(TransparenciaInfX,TransparenciaInfY);
		glTexCoord2f(1.0f,0.0f);
		glVertex2f(TransparenciaSupX,TransparenciaInfY);
		glTexCoord2f(1.0f,1.0f);
		glVertex2f(TransparenciaSupX,TransparenciaSupY);	
		glTexCoord2f(0.0f,1.0f);
		glVertex2f(TransparenciaInfX,TransparenciaSupY);	
	glEnd();

	//posicionY/rango
	for(unsigned int i=0;i<puntos.size();i++)
	{
		if(i+1<puntos.size()){
			glBindTexture( GL_TEXTURE_2D, 0 );
			glColor4f(1.0f,.0f,.0f,1.0f);
			glBegin(GL_LINES);
				glVertex2f(puntos[i].pos.x,puntos[i].pos.y);
				glVertex2f(puntos[i+1].pos.x,puntos[i+1].pos.y);
			glEnd();
		}
		glColor4f(1.0f,1.0f,1.0f,1.0f);

		interfaz[4].Bind2D();
		if(seleccionado==i){
			interfaz[5].Bind2D();
		}
		
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f,0.0f);
			glVertex2f(puntos[i].pos.x-0.01f,puntos[i].pos.y-0.01f);
			glTexCoord2f(1.0f,0.0f);
			glVertex2f(puntos[i].pos.x+0.01f,puntos[i].pos.y-0.01f);
			glTexCoord2f(1.0f,1.0f);
			glVertex2f(puntos[i].pos.x+0.01f,puntos[i].pos.y+0.01f);	
			glTexCoord2f(0.0f,1.0f);
			glVertex2f(puntos[i].pos.x-0.01f,puntos[i].pos.y+0.01f);	
		glEnd();
	}


	glBindTexture( GL_TEXTURE_2D, 0 );
	glColor4f(.0f,.0f,.0f,1.0f);
	glBegin(GL_LINE_LOOP);
		glVertex2f(0.70f,-0.68f);
		glVertex2f(0.91f,-0.68f);
		glVertex2f(0.91f,-0.63f);	
		glVertex2f(0.70f,-0.63f);
	glEnd();

	glColor4f(puntos[seleccionado].r,puntos[seleccionado].g,puntos[seleccionado].b,puntos[seleccionado].a);
	glBegin(GL_QUADS);
		glVertex2f(0.70f,-0.68f);
		glVertex2f(0.91f,-0.68f);
		glVertex2f(0.91f,-0.63f);	
		glVertex2f(0.70f,-0.63f);
	glEnd();
	glDisable(GL_BLEND);
}

void actColor()
{
	int cantColores=1;
	if(vol.BITS==8)
		cantColores=256;
	if(vol.BITS==16)
		cantColores=65536;
	if(cantColores==1)
		return;
	float viejoRango=puntos[puntos.size()-1].pos.x-puntos[0].pos.x;
	color *col = (color *)malloc(sizeof(color)*cantColores);
	int anterior=0, acceso=cantColores-1;
	textColor = (GLubyte*)malloc(sizeof(GLubyte)*4*cantColores);
	for(unsigned i=0;i<puntos.size();i++)
	{
		if(i==0)
		{
			col[0].r=puntos[0].r;
			col[0].g=puntos[0].g;
			col[0].b=puntos[0].b;
			col[0].a=puntos[0].a;
			anterior=0;
		}else{
			acceso=(puntos[i].pos.x-puntos[0].pos.x)*(cantColores-1)/(puntos[puntos.size()-1].pos.x-puntos[0].pos.x);
			col[acceso].r=puntos[i].r;col[acceso].g=puntos[i].g;
			col[acceso].b=puntos[i].b;col[acceso].a=puntos[i].a;
			float casillas = acceso-anterior-1;
			float paso=1/casillas;
			float pasor=(puntos[i].r-puntos[i-1].r)*paso;float pasog=(puntos[i].g-puntos[i-1].g)*paso;
			float pasob=(puntos[i].b-puntos[i-1].b)*paso;float pasoa=(puntos[i].a-puntos[i-1].a)*paso;
			for(int j=anterior+1;j<acceso;j++)
			{
				col[j].r=col[j-1].r+pasor;
				col[j].g=col[j-1].g+pasog;
				col[j].b=col[j-1].b+pasob;
				col[j].a=col[j-1].a+pasoa;
			}
			anterior=acceso;
		}
	}
	for(int i=0;i<cantColores;i++)
	{
		textColor[i*4+0]=(GLubyte)(col[i].r*255);
		textColor[i*4+1]=(GLubyte)(col[i].g*255);
		textColor[i*4+2]=(GLubyte)(col[i].b*255);
		textColor[i*4+3]=(GLubyte)(col[i].a*255);
	}

	/*OldRange = (OldMax - OldMin)
	NewRange = (NewMax - NewMin)
	NewValue = (((OldValue - OldMin) * NewRange) / OldRange) + NewMin*/


	color1D.CargarTextura1D(textColor,cantColores);
}

void initFunc()
{
	color primero;
	primero.pos.x= primero.pos.y = primero.r = primero.g = primero.b = primero.a = 0.0f;
	primero.pos.x=TransparenciaInfX;
	primero.pos.y=TransparenciaInfY;
	puntos.push_back(primero);
	primero.pos.x= primero.pos.y = primero.r = primero.g = primero.b = primero.a = 1.0f;
	primero.pos.x=TransparenciaSupX;
	primero.pos.y=TransparenciaSupY;
	puntos.push_back(primero);
	seleccionado=0;

	transparencia[0].x = TransparenciaInfX, transparencia[1].x = TransparenciaSupX, transparencia[0].y = TransparenciaInfY, transparencia[1].y =TransparenciaSupY;
	slider.x = selectorInfX, slider.y =selectorSupX;

	rangoSeleccionador.x =  slider.y - slider.x;
	rangoTransparencia.y = transparencia[1].y - transparencia[0].y;
	rangoTransparencia.x = transparencia[1].x - transparencia[0].x;

	alturaSlide[2]=selectorBY; 
	alturaSlide[1]=selectorGY; 
	alturaSlide[0]=selectorRY; 

	rotar = trasladar = escalarX = escalarY = escalarZ = slideR = slideG = slideB = dot = trasladarY = false;
	rotacion[0]= rotacion[1]=rotacion[2]=0.0f;
	traslacion[0] = traslacion[1] = traslacion[2] = 0.0f;
	escalamiento[0] = escalamiento[1] = escalamiento[2] = 1.0f;

}


void crearBoton(float x,float y)
{
	unsigned int nuevaPos=0;
	for(unsigned int i=0;i<puntos.size()-1;i++)
	{
		if(x>puntos[i].pos.x&&x<puntos[i+1].pos.x){ nuevaPos=i+1; break;}
	}
	if(nuevaPos==0)return;
	puntos.push_back(puntos[puntos.size()-1]);
	for(unsigned int i=puntos.size()-1;i>=nuevaPos;i--)
	{
		puntos[i].pos.x=puntos[i-1].pos.x;
		puntos[i].pos.y=puntos[i-1].pos.y;
		puntos[i].r=puntos[i-1].r;
		puntos[i].g=puntos[i-1].g;
		puntos[i].b=puntos[i-1].b;
	}
	puntos[nuevaPos].pos.x=x;
	puntos[nuevaPos].pos.y=y;
	puntos[nuevaPos].a=(puntos[nuevaPos].pos.y - TransparenciaInfY )/rangoTransparencia.y;

	float rangoRojo,rangoVerde,rangoAzul,rangoX,minRojo,minVerde,minAzul;
	rangoRojo = puntos[nuevaPos+1].r - puntos[nuevaPos-1].r;
	rangoVerde = puntos[nuevaPos+1].g - puntos[nuevaPos-1].g;
	rangoAzul = puntos[nuevaPos+1].b - puntos[nuevaPos-1].b;
	rangoX=puntos[nuevaPos+1].pos.x - puntos[nuevaPos-1].pos.x;

	minRojo = puntos[nuevaPos-1].r; if(minRojo>puntos[nuevaPos+1].r) minRojo =puntos[nuevaPos+1].r;
	minVerde = puntos[nuevaPos-1].g; if(minVerde>puntos[nuevaPos+1].g) minVerde =puntos[nuevaPos+1].g;
	minAzul = puntos[nuevaPos-1].b; if(minAzul>puntos[nuevaPos+1].b) minAzul=puntos[nuevaPos+1].b;

	if(rangoRojo<0) rangoRojo=-rangoRojo;
	if(rangoVerde<0) rangoVerde=-rangoVerde;
	if(rangoAzul<0) rangoAzul=-rangoAzul;

	puntos[nuevaPos].r = ((puntos[nuevaPos].pos.x - puntos[nuevaPos-1].pos.x)*rangoRojo/rangoX)+minRojo;
	puntos[nuevaPos].g = ((puntos[nuevaPos].pos.x - puntos[nuevaPos-1].pos.x)*rangoVerde/rangoX)+minVerde;
	puntos[nuevaPos].b = ((puntos[nuevaPos].pos.x - puntos[nuevaPos-1].pos.x)*rangoAzul/rangoX)+minAzul;

	seleccionado=nuevaPos;

	
/*OldRange = (OldMax - OldMin)
NewRange = (NewMax - NewMin)
NewValue = (((OldValue - OldMin) * NewRange) / OldRange) + NewMin*/


}

void eliminarBoton(float x1,float y1)
{
	unsigned int eliminar=0;
	for(unsigned int i=0;i<puntos.size();i++)
	{
		if(x1>=puntos[i].pos.x-0.01f&&x1<=puntos[i].pos.x+0.01f&&y1>=puntos[i].pos.y-0.01f&&y1<=puntos[i].pos.y+0.01f){
			eliminar=i; break;
		}
	}
	if(eliminar!=0&&eliminar!=puntos.size()-1)
	{
		if(seleccionado>=eliminar) seleccionado--;
		for(unsigned int i=eliminar;i<puntos.size()-1;i++)
		{
			puntos[i].pos.x=puntos[i+1].pos.x;
			puntos[i].pos.y=puntos[i+1].pos.y;
			puntos[i].r=puntos[i+1].r;
			puntos[i].g=puntos[i+1].g;
			puntos[i].b=puntos[i+1].b;
			puntos[i].a=puntos[i+1].a;
		}
		puntos.pop_back();
	}
}

void TratarPunto(float x1,float y1)
{
	if(y1>=TransparenciaInfY&&y1<=TransparenciaSupY)
			{
				puntos[seleccionado].pos.y=y1;
				puntos[seleccionado].a= (puntos[seleccionado].pos.y - TransparenciaInfY)/ rangoTransparencia.y;
			}else if(y1<TransparenciaInfY)
			{ 
				puntos[seleccionado].pos.y=TransparenciaInfY;
				puntos[seleccionado].a=0.0f;
			}else
			{
				puntos[seleccionado].pos.y=TransparenciaSupY;
				puntos[seleccionado].a=1.0f;
			}

			if(seleccionado!=0&&seleccionado!=puntos.size()-1)
			{
				if(x1>puntos[0].pos.x&&x1<puntos[puntos.size()-1].pos.x)
				{
					puntos[seleccionado].pos.x=x1;
					color aux;
					aux.pos.x=puntos[seleccionado].pos.x;
					aux.pos.y=puntos[seleccionado].pos.y;
					aux.r=puntos[seleccionado].r;
					aux.g=puntos[seleccionado].g;
					aux.b=puntos[seleccionado].b;
					aux.a=puntos[seleccionado].a;
					if(puntos[seleccionado].pos.x>=puntos[seleccionado+1].pos.x)
					{
						puntos[seleccionado].pos.x=puntos[seleccionado+1].pos.x;
						puntos[seleccionado].pos.y=puntos[seleccionado+1].pos.y;
						puntos[seleccionado].r=puntos[seleccionado+1].r;
						puntos[seleccionado].g=puntos[seleccionado+1].g;
						puntos[seleccionado].b=puntos[seleccionado+1].b;
						puntos[seleccionado].a=puntos[seleccionado+1].a;

						puntos[seleccionado+1].pos.x=aux.pos.x;
						puntos[seleccionado+1].pos.y=aux.pos.y;
						puntos[seleccionado+1].r=aux.r;
						puntos[seleccionado+1].g=aux.g;
						puntos[seleccionado+1].b=aux.b;
						puntos[seleccionado+1].a=aux.a;
						seleccionado++;
					}
					if(puntos[seleccionado].pos.x<puntos[seleccionado-1].pos.x)
					{
						puntos[seleccionado].pos.x=puntos[seleccionado-1].pos.x;
						puntos[seleccionado].pos.y=puntos[seleccionado-1].pos.y;
						puntos[seleccionado].r=puntos[seleccionado-1].r;
						puntos[seleccionado].g=puntos[seleccionado-1].g;
						puntos[seleccionado].b=puntos[seleccionado-1].b;
						puntos[seleccionado].a=puntos[seleccionado-1].a;

						puntos[seleccionado-1].pos.x=aux.pos.x;
						puntos[seleccionado-1].pos.y=aux.pos.y;
						puntos[seleccionado-1].r=aux.r;
						puntos[seleccionado-1].g=aux.g;
						puntos[seleccionado-1].b=aux.b;
						puntos[seleccionado-1].a=aux.a;
						seleccionado--;
					}

				}
			}
}
void movimientoMouse(int x,int y){
	if(!TwEventMouseMotionGLUT(x,y) ){
		float xn=0.0f,yn=0.0f,zn=0.0f;

		float x1 =2.0f*(float)x/(float)w-1.0f;//redimenciono los puntos al tamaño de mi ventana
		float y1= - (2.0f*(float)y/h - 1.0f);
		if(slideR)
		{
			if(x1>=selectorInfX&&x1<=selectorSupX)	puntos[seleccionado].r=(x1 - 0.415f)/rangoSeleccionador.x;
			else if (x1<selectorInfX) puntos[seleccionado].r=0.0f;
			else puntos[seleccionado].r=1.0f;
			actColor();
		}else if(slideG)
		{
			actColor();
			if(x1>=selectorInfX&&x1<=selectorSupX)	puntos[seleccionado].g=(x1 - 0.415f)/rangoSeleccionador.x;
			else if (x1<selectorInfX) puntos[seleccionado].g=0.0f;
			else puntos[seleccionado].g=1.0f;
		}else
		if(slideB)
		{
			if(x1>=selectorInfX&&x1<=selectorSupX)	puntos[seleccionado].b=(x1 - 0.415f)/rangoSeleccionador.x;
			else if (x1<selectorInfX) puntos[seleccionado].b=0.0f;
			else puntos[seleccionado].b=1.0f;
			actColor();
		}else
		if(escalarX||escalarY||escalarZ||rotar)
		{
			if(!escalarX&&!escalarY&&!escalarZ&&rotar)
			{
				xn=(-XR+(float)x)/(float)w;
				yn=(YR+(float)-y)/(float)h;
				rotacion[0]=rotacion[0]-(yn*50);
				rotacion[1]=rotacion[1]+(xn*50);
				rotacion[2]=0;
				XR=(float)x;
				YR=(float)y;
			}else{
				yn=(-YE+(float)y)/(float)h;
				if(escalarX)
					escalamiento[0]=escalamiento[0]-(yn*2);
				if(escalarY)
					escalamiento[1]=escalamiento[1]-(yn*2);
				if(escalarZ)
					escalamiento[2]=escalamiento[2]-(yn*2);
				YE=(float)y;
			}
		}else if(trasladar)
		{
			if(trasladarY){
				yn=(YT+(float)-y)/(float)h;
				traslacion[1]=traslacion[1]+(yn*2);

			}else{
				xn=(-XT+(float)x)/(float)w;
				zn=(-ZT+(float)y)/(float)h;
				traslacion[0]=traslacion[0]+(xn*2);
				traslacion[2]=traslacion[2]+(zn*2);
			}
			XT=(float)x;
			YT=(float)y;
			ZT=(float)y;
		}else	
		if(dot)
		{
			TratarPunto(x1,y1);
			actColor();
		}
	}

	glutPostRedisplay();

}


void teclado(unsigned char key, int x, int y){
	if(	!TwEventKeyboardGLUT(key,x,y))
	{
		switch (key) 
		{
			case 27:	
				exit(0);
				break;
			case 'x':
				escalarX=true;
				break;
			case 'y':
				escalarY=true;
				break;
			case 'z':
				escalarZ=true;
				break;
			default:
				escalarX = escalarY = escalarZ = false;
				break;
		}
	}
	glutPostRedisplay();
}

void tecladoUp(unsigned char key, int x, int y){
		switch (key) 
		{
			case 27:	
				exit(0);
				break;
			case 'x':
				escalarX=false;
				break;
			case 'y':
				escalarY=false;
				break;
			case 'z':
				escalarZ=false;
				break;
			default:
			break;
		}
	glutPostRedisplay();
}


void clickMouse(int boton, int estado, int x, int y) 
{
	if( !TwEventMouseButtonGLUT(boton,estado,x,y) )  // send event to AntTweakBar
	{
		float x1 =2.0f*(float)x/(float)w-1.0f;//redimenciono los puntos al tamaño de mi ventana
		float y1= - (2.0f*(float)y/h - 1.0f);
		if(estado==GLUT_UP)
		{
			rotar = trasladar = trasladarY = escalarX = escalarY = escalarZ = slideR = slideG = slideB = dot = false;
		}else if(estado==GLUT_DOWN)
		{
			if(boton==GLUT_LEFT_BUTTON)
			{
				if(x1>0.35f&&x1<1.0f&&y1>-1.0f&&y1<-0.18f)
				{
					if(x1>=puntos[seleccionado].r*rangoSeleccionador.x + slider.x-0.035f&&
						x1<=puntos[seleccionado].r*rangoSeleccionador.x + slider.x+0.035f&&
						y1>=alturaSlide[0]-0.03f&&y1<=alturaSlide[0]+0.03f)
					{
						slideR = true;
					}else if(x1>=puntos[seleccionado].g*rangoSeleccionador.x + slider.x-0.035f&&
						x1<=puntos[seleccionado].g*rangoSeleccionador.x + slider.x+0.035f&&
						y1>=alturaSlide[1]-0.03f&&y1<=alturaSlide[1]+0.03f){
						slideG = true;
					}else if(x1>=puntos[seleccionado].b*rangoSeleccionador.x + slider.x-0.035f&&
						x1<=puntos[seleccionado].b*rangoSeleccionador.x + slider.x+0.035f&&
						y1>=alturaSlide[2]-0.03f&&y1<=alturaSlide[2]+0.03f)
					{
						slideB = true;
					}else if(x1>=0.35f&&x1<=1.0f&&y1>=-.61f&&y1<=-.18f)
					{
						
						for(unsigned int i=0;i<puntos.size();i++)
						{
							if(x1>=puntos[i].pos.x-0.05f&&x1<=puntos[i].pos.x+0.05f&&y1>=puntos[i].pos.y-0.05f&&y1<=puntos[i].pos.y+0.05f){
								seleccionado=i;
								dot=true;
							}
						}
					}
				}else
				{
					rotar=true;
				}
			}
			if(boton==GLUT_RIGHT_BUTTON)
			{
				if(x1>0.35f&&x1<1.0f&&y1>-1.0f&&y1<-0.18f)
				{
					if(x1>TransparenciaInfX&&x1<TransparenciaSupX&&y1>=TransparenciaInfY&&y1<=TransparenciaSupY)
					{
						crearBoton(x1,y1);
						actColor();
					}
				}else
				{
					trasladar=true;
				}
			}
			if(boton==GLUT_MIDDLE_BUTTON)
			{
				if(x1>0.35f&&x1<1.0f&&y1>-1.0f&&y1<-0.18f)
				{
					if(x1>TransparenciaInfX-0.01f&&x1<TransparenciaSupX+0.01f&&y1>=TransparenciaInfY-0.01f&&y1<=TransparenciaSupY+0.01f)
					{
						eliminarBoton(x1,y1);
						actColor();
					}
				}else
				{
					trasladar=true;
					trasladarY=true;
				}
			}
			XR=(float)x;
			YR=(float)y;
			XT=(float)x;
			YT=(float)y;
			ZT=(float)y;
			YE=(float)y;
		}
	}
	glutPostRedisplay();
}
