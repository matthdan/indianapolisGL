/*
 * OpenGL Project using glut API. This program represent the IndianaPolis MotorSpeedWay
 *
 * Copyright (C) 2010  Matthieu DANIEL <matth.daniel@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 **/

#ifdef __APPLE__ 
#include <OpenGL/gl.h>
#else 
#include <GL/gl.h>
#endif 


#ifdef __APPLE__ 
#include <OpenGL/glu.h>
#else 
#include <GL/glu.h>
#endif 


#ifdef __APPLE__ 
#include <GLUT/glut.h> 
#else 
#include <GL/glut.h> 
#endif 

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/timeb.h>

#define PI 3.1415927

/*  Fonctions Prototypes  */

int  chargeTextureTiff( int , char *,int * , int * , int * , unsigned char **  );
GLvoid ChargeTextureProc(int numtex);
GLint FonctionTexture(GLint x,GLint y);
GLvoid  Initialize( char ** );
GLvoid  DrawScene( GLvoid );
GLvoid Keyboard(unsigned char  , int , int );
GLvoid SpecialKey(int,int,int);
GLvoid Mouse(int,int,int,int);
GLvoid MouseMove(int  , int );
void   Resize(int width, int height);
GLvoid  PolarView(GLdouble,GLdouble,GLdouble,GLdouble);
GLvoid visible(GLint );


/* Variables globales definissant les couleur */

GLfloat White[]={1.,1.,1.};
GLfloat Black[]={0.,0.,0.};
GLfloat Red[]={1.,0.,0.};
GLfloat Green[]={0.,1.,0.};
GLfloat Blue[]={0.,0.,1.};
GLfloat Magenta[]={1.,0.,1.};
GLfloat Cyan[]={0.,1.,1.};
GLfloat Yellow[]={1.,1.,0.};
GLfloat Voiture1[]={0.,0.4,1.};
GLfloat Voiture2[]={1.,0.2,0.4};

/* Variables globales */

GLfloat xtrans=0.;
GLfloat ytrans=0.;
GLfloat ztrans=0.;
GLfloat xangle=0.;
GLfloat yangle=0.;
GLfloat zangle=0.;

GLboolean SunFlag = GL_FALSE;

//--------------- VITESSE
GLfloat vitesse = 0.01;
GLfloat vitesse2 = 0.01;

enum Action { ROTATE_X,ROTATE_Y,ZOOM_IN,ZOOM_OUT};
enum Action Flag;

GLfloat aspect;
GLfloat near = 0.1;
GLfloat far = 16.;
GLfloat fovy = 45. ;

GLfloat AngleTour = 180.;
GLfloat LigneDroite = 0.;

// Boolean 
GLboolean   LightFlag=GL_FALSE;
GLboolean   BlendFlag=GL_FALSE;
GLboolean   MoveLightFlag=GL_FALSE;
GLboolean   SmoothFlag = GL_FALSE;
GLboolean   ColorFlag = GL_FALSE;
GLfloat xRot= 0;
GLfloat yRot = 0;
GLfloat spot_cutoff=60.;
GLfloat spot_exp=1;
GLfloat C0 = 1.;
GLfloat C1 = 0.;
GLfloat C2 = 0.;
GLfloat Angle=0;


int xstart=0 ;
int ystart=0;
int xoffset=0;
int yoffset=0 ;

GLfloat MaxObjectsSize;
GLfloat xtrans,ytrans,ztrans,xangle,yangle,zangle;

int IdTex[20]; /* tableau d'Id pour les textures */
float Decalage=0; /* décalage de la texture procedurale pour l'animation */
GLint TextureId;

/* déclaration des indicateurs booleens */

GLdouble    Radius=10.;
GLdouble    Twist=0.;
GLdouble    Latitude=0.;
GLdouble    Longitude=0.; 

// compteur TOUR
GLint Compteur1=0;
GLint Compteur2=0;


//GLboolean LightFlag = GL_TRUE;
GLboolean SeparateFlag = GL_TRUE;

GLfloat Distance=2.5;
GLfloat  Angley=20.;
GLfloat  Anglex=30.;


GLboolean View = GL_FALSE;

/* Booleens Animation auto */
int FlagAuto;
int FlagAuto2;

GLboolean FLagVu1=GL_FALSE;
GLboolean FLagVu2=GL_FALSE;
GLboolean FLagVu3=GL_FALSE;
GLboolean FLagVu4=GL_FALSE;

GLboolean LigneDroiteOk = GL_TRUE;
GLboolean ChicaneOk = GL_TRUE;

/* Variable Globale */
GLfloat distance =0.;
//distance += 8*vitesse;



/* Variables de temps */
GLfloat dt=0;;
struct timeb mytime;
GLfloat GetTime(GLvoid )
{
	static GLfloat t0=0;
	GLfloat t;
	ftime(&mytime);
	t = (mytime.time)%60 + (mytime.millitm)/1000.;
	dt = t-t0;
	t0 = t;
	return dt;
}


/* Programme principal
 ------------------- */
int main(int argc ,  char ** argv)
{
	
	// Initialisation
	glutInit(&argc,argv);
	
	// Chargement des textures
	Initialize(argv);
	
	// Mise en place des fonctions de rappel de glut
	glutDisplayFunc(DrawScene);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(SpecialKey);
	glutMouseFunc(Mouse);
	glutMotionFunc( MouseMove );
	glutReshapeFunc(Resize);
	glutVisibilityFunc(visible);
	
	// boucle principale
	glutMainLoop();
	return 0 ;
}

GLvoid Initialize( char ** Argv )
{
	GLvoid DefineWindow( char *) ;
	GLvoid DefineLight(GLvoid);
	GLvoid DefineTexture(int  , char * );
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
	
	//Initialisation
	
	MaxObjectsSize=far-near;
	xtrans = 0.;
	ytrans = 0.;
	ztrans = MaxObjectsSize/2. + near;
	xangle = 0.;
	yangle = 0.;
	zangle = 0;
	
	DefineWindow( Argv[0]) ; 
	
	// chargement des textures
	DefineTexture(20,Argv[20]);
	
	// definition de la source lumineuse
	DefineLight();
	
	
	// definition du monde de Coordonnées
	
	glPointSize(4);
	glLineWidth(2.5);
	
}

/* Fonction de definition des texture */

GLvoid DefineTexture(int Nbre , char * Textures)
{
	int ImageLargeur,ImageHauteur,Composantes;
	// unsigned char * image[Nbre de texture];
	unsigned char * image[20];
	/* Instructions pour charger une texture:
     ------------------------------------- */
	glGenTextures(Nbre,IdTex);
	
	//TEXTURE ROUTE
	chargeTextureTiff(1,"./imgs/route.tif",&ImageLargeur,&ImageHauteur,&Composantes,&image[0]);
	glBindTexture(GL_TEXTURE_2D,IdTex[0]);
	glTexImage2D(GL_TEXTURE_2D,0,Composantes,ImageLargeur,ImageHauteur,0,GL_RGB,GL_UNSIGNED_BYTE,image[0]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
	// TEXTURE SOL
	chargeTextureTiff(1,"./imgs/herbe.tif",&ImageLargeur,&ImageHauteur,&Composantes,&image[1]);
	glBindTexture(GL_TEXTURE_2D,IdTex[1]);
	glTexImage2D(GL_TEXTURE_2D,0,Composantes,ImageLargeur,ImageHauteur,0,GL_RGB,GL_UNSIGNED_BYTE,image[1]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
	// MICHELIN
	chargeTextureTiff(1,"./imgs/michelin.tif",&ImageLargeur,&ImageHauteur,&Composantes,&image[2]);
	glBindTexture(GL_TEXTURE_2D,IdTex[2]);
	glTexImage2D(GL_TEXTURE_2D,0,Composantes,ImageLargeur,ImageHauteur,0,GL_RGB,GL_UNSIGNED_BYTE,image[2]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
	// logo indianapolis motorway
	chargeTextureTiff(1,"./imgs/indianapolis.tif",&ImageLargeur,&ImageHauteur,&Composantes,&image[3]);
	glBindTexture(GL_TEXTURE_2D,IdTex[3]);
	glTexImage2D(GL_TEXTURE_2D,0,Composantes,ImageLargeur,ImageHauteur,0,GL_RGB,GL_UNSIGNED_BYTE,image[3]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
	// Ciel
	chargeTextureTiff(1,"./imgs/ciel.tif",&ImageLargeur,&ImageHauteur,&Composantes,&image[4]);
	glBindTexture(GL_TEXTURE_2D,IdTex[4]);
	glTexImage2D(GL_TEXTURE_2D,0,Composantes,ImageLargeur,ImageHauteur,0,GL_RGB,GL_UNSIGNED_BYTE,image[4]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
	// Ecorce
	chargeTextureTiff(1,"./imgs/ecorce.tif",&ImageLargeur,&ImageHauteur,&Composantes,&image[5]);
	glBindTexture(GL_TEXTURE_2D,IdTex[5]);
	glTexImage2D(GL_TEXTURE_2D,0,Composantes,ImageLargeur,ImageHauteur,0,GL_RGB,GL_UNSIGNED_BYTE,image[5]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
	// Feuille
	chargeTextureTiff(1,"./imgs/feuille.tif",&ImageLargeur,&ImageHauteur,&Composantes,&image[6]);
	glBindTexture(GL_TEXTURE_2D,IdTex[6]);
	glTexImage2D(GL_TEXTURE_2D,0,Composantes,ImageLargeur,ImageHauteur,0,GL_RGB,GL_UNSIGNED_BYTE,image[6]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
	//TEXTURE ROUTE
	chargeTextureTiff(1,"./imgs/route2.tif",&ImageLargeur,&ImageHauteur,&Composantes,&image[7]);
	glBindTexture(GL_TEXTURE_2D,IdTex[7]);
	glTexImage2D(GL_TEXTURE_2D,0,Composantes,ImageLargeur,ImageHauteur,0,GL_RGB,GL_UNSIGNED_BYTE,image[7]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
	// TEXTURE METAL
	chargeTextureTiff(1,"./imgs/metal.tif",&ImageLargeur,&ImageHauteur,&Composantes,&image[8]);
	glBindTexture(GL_TEXTURE_2D,IdTex[8]);
	glTexImage2D(GL_TEXTURE_2D,0,Composantes,ImageLargeur,ImageHauteur,0,GL_RGB,GL_UNSIGNED_BYTE,image[8]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
	// TEXTURE METAL 2 - Panneau
	chargeTextureTiff(1,"./imgs/metal3.tif",&ImageLargeur,&ImageHauteur,&Composantes,&image[9]);
	glBindTexture(GL_TEXTURE_2D,IdTex[9]);
	glTexImage2D(GL_TEXTURE_2D,0,Composantes,ImageLargeur,ImageHauteur,0,GL_RGB,GL_UNSIGNED_BYTE,image[9]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
	
	//tags
	chargeTextureTiff(1,"./imgs/tags.tif",&ImageLargeur,&ImageHauteur,&Composantes,&image[10]);
	glBindTexture(GL_TEXTURE_2D,IdTex[10]);
	glTexImage2D(GL_TEXTURE_2D,0,Composantes,ImageLargeur,ImageHauteur,0,GL_RGB,GL_UNSIGNED_BYTE,image[10]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
	
	// fumer cassemoteur
	chargeTextureTiff(1,"./imgs/feu.tif",&ImageLargeur,&ImageHauteur,&Composantes,&image[11]);
	glBindTexture(GL_TEXTURE_2D,IdTex[11]);
	glTexImage2D(GL_TEXTURE_2D,0,Composantes,ImageLargeur,ImageHauteur,0,GL_RGB,GL_UNSIGNED_BYTE,image[11]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
	
    
}

/* Fonction definition lumière */

GLvoid DefineLight(GLvoid)
{
	GLfloat Light0pos[]={ 0.0,2.0,-1.0};
	GLfloat Light0dif[]={ 1.0,0.6,0.6};
	GLfloat Light1pos[]={ 2.0,2.0,2.0};
	GLfloat Light1dif[]={ 0.0,0.5,1.0};
	GLfloat LumiereAmbient[]={0.5,0.5,0.5,1.};
	GLfloat LumiereDiffuse[]={1.,1.,0.,1.};
	GLfloat LumiereSpecular[]={1.,1.,1.,1.};
	
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,Light0dif);
	glLightfv(GL_LIGHT0,GL_POSITION,Light0pos);
	glLightfv(GL_LIGHT1,GL_DIFFUSE,Light1dif);
	glLightfv(GL_LIGHT1,GL_POSITION,Light1pos); 
	
	//--------------------------------------------
	glLightfv(GL_LIGHT0,GL_AMBIENT,LumiereAmbient);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,LumiereDiffuse);
	glLightfv(GL_LIGHT0,GL_SPECULAR,LumiereSpecular);
	
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);
	
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);
	glLightf(GL_LIGHT0,GL_CONSTANT_ATTENUATION,C0);
	glLightf(GL_LIGHT0,GL_LINEAR_ATTENUATION,C1);
	glLightf(GL_LIGHT0,GL_QUADRATIC_ATTENUATION,C2);
	
}
GLvoid 
DrawLight(GLvoid)
{
	glutSolidCone(0.15,0.5,7,7);
	glPushAttrib(GL_LIGHTING_BIT);
	glDisable(GL_LIGHTING);
	glColor3fv(Yellow);
	glutSolidSphere(0.1,30,30);
	glPopAttrib();
}
GLvoid 
LocateLight(GLfloat *Location,GLint LightNumber)
{
	GLfloat Direction[]={0.,0.,-1.};
	GLvoid DrawLight(GLvoid);
	glPushMatrix();
	glRotatef(yRot,0.,1.,0.);
	glRotatef(xRot,1.,0.,0.);
	glLightfv(LightNumber,GL_POSITION,Location);
	glTranslatef(Location[0],Location[1],Location[2]);
	glLightfv(LightNumber,GL_SPOT_DIRECTION,Direction);
	glLightf(LightNumber,GL_SPOT_CUTOFF,spot_cutoff);
	glLightf(LightNumber,GL_SPOT_EXPONENT,spot_exp);
	DrawLight();
	glPopMatrix();
	
}




GLvoid DefineWindow( char * Titre)
{
	GLsizei width,height;
	
	/* Largeur et hauteur de l'écran
     ---------------------------------- */
	
	width = glutGet(GLUT_SCREEN_WIDTH);
	height = glutGet(GLUT_SCREEN_HEIGHT);
	
	
	/* Définition de la fenêtre 
     ------------------------ */
	
	glutInitWindowPosition(width/4,height/4);
	glutInitWindowSize(width/2,height/2);
	
	glutCreateWindow(Titre);
	
	/* Définition de la couleur du fond de la fenêtre
     ----------------------------------------------- */
	
	glClearColor(0.,0.,0.,1.);
	
	
}

GLvoid SpecialKey(int key , int x ,int y)
{
	int Modifier ;
	Modifier = glutGetModifiers();
	switch(key)
    {
		case GLUT_KEY_LEFT:
			if ( Modifier == GLUT_ACTIVE_CTRL)
			{
				yangle += 1.;
				if ( yangle > 360.) yangle = 0. ;
			}
			else
				xtrans += 0.1;
			break;
		case GLUT_KEY_RIGHT:
			if ( Modifier == GLUT_ACTIVE_CTRL)
			{
				yangle -= 1.;
				if ( yangle < -360.) yangle = 0. ;
			}
			
			else
				xtrans -= 0.1;
			break;
		case GLUT_KEY_UP:
			if ( Modifier == GLUT_ACTIVE_CTRL)
			{
				xangle -= 1.;
				if ( xangle < -360.) xangle = 0.;
			}
			else
				ytrans -= 0.1;
			break;
			
		case GLUT_KEY_DOWN:
			if ( Modifier == GLUT_ACTIVE_CTRL)
			{
				xangle += 1.;
				if ( xangle > 360.) xangle = 0. ;
			}
			
			else
				ytrans += 0.1;
			break;
			
		case GLUT_KEY_PAGE_UP:
			if ( Modifier == GLUT_ACTIVE_CTRL)
			{
				zangle += 1.;
				if ( zangle > 360.) zangle = 0. ;
			}
			else
				ztrans += 0.1;
			break;
			
		case GLUT_KEY_PAGE_DOWN:
			if ( Modifier == GLUT_ACTIVE_CTRL)
			{
				zangle -= 1.;
				if ( zangle < -360.) zangle = 0. ;
			}
			else
				ztrans -= 0.1; 
			break;
			
    }
	glutPostRedisplay();
}
GLvoid Keyboard(unsigned char key , int x, int y)
{
	switch(key)
    {
			
		case 'i':
			xtrans = 0.;
			ytrans = 0.;
			ztrans = 0.;
			xangle = 0.;
			yangle = 0.;
			zangle = 0.;
			
			
			break;
			
		case 's':
			SunFlag = !SunFlag;
			break;
			
		case 27:
			exit(1);
			break;
			
			
		case 'd':
			FlagAuto=1;
			FlagAuto2=1;
			break;
			
		case 'l':
			LightFlag=!LightFlag;
			break;
			
		case 'v':
			View=!View;
			break;
			
		case 'a':
			vitesse = vitesse+0.005;
			break;
			
		case 'q':
			vitesse = vitesse - 0.005;
			break;
			
		case 'p':
			vitesse2 = vitesse2 + 0.005;
			break;
			
		case 'm':
			vitesse2 = vitesse2 -0.005;
			break;
			
		case 'f':
			FLagVu1=!FLagVu1;
			break;
			
		case 'g':
			FLagVu2=!FLagVu2;
			break;
			
		case 'h':
			FLagVu3=!FLagVu3;
			break;
			
		case 'j':
			FLagVu4=!FLagVu4;
			break;
			
			
    }
	glutPostRedisplay();
}

void Resize(int width,int height)
{
	
	glViewport(0, 0, width, height);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0, (float)width / (float)height, 0.1, 90.0);
	glMatrixMode(GL_MODELVIEW);
	
}

GLvoid MouseMove(int x , int y)
{
	GLsizei WindowWidth,WindowHeight;
	WindowWidth = glutGet(GLUT_WINDOW_WIDTH);
	WindowHeight = glutGet(GLUT_WINDOW_HEIGHT);
	
	xoffset=x-xstart;
	yoffset=y-ystart;
	xstart=x;
	ystart=y; 
	if ( Flag == ROTATE_X )
    {
		
		xangle+=(xoffset* 360.) / WindowWidth;
		if ( xangle > 360. || xangle < -360. ) xangle=0. ; 
		
    }
	else if ( Flag == ROTATE_Y )
    {
		yangle+=(yoffset*360.) / WindowHeight;
		if ( yangle > 360. || yangle < -360. ) yangle=0.; 
    }
	else if ( Flag == ZOOM_IN )
		ztrans-=0.05;
	else
		ztrans+=0.05;
	glutPostRedisplay(); 
}

GLvoid Mouse(int button , int state , int x , int y)
{
	int Modifier ;
	Modifier = glutGetModifiers();
	xstart=x;
	ystart=y; 
	if ( state == GLUT_DOWN )
    {
		if (  button == GLUT_LEFT_BUTTON  )
			Flag = ROTATE_X ;
		else if (button == GLUT_MIDDLE_BUTTON ) 
			Flag = ROTATE_Y ;
		
		else 
		{
			if ( Modifier == GLUT_ACTIVE_SHIFT) Flag = ZOOM_OUT ;
			else
				Flag = ZOOM_IN ;
		}
    }
}

GLvoid DefineSun(GLvoid)
{
	static GLfloat sunpos[4] = { 0.7071, 0.7071, 0.0, 0.0 };
	static GLfloat suncolor[4] = { 0.5, 0.5, 0.4, 1.0 };
	static GLfloat sunambient[4] = { 0.5, 0.5, 0.4, 1.0 };
	glEnable(GL_LIGHTING);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, sunambient);
	
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);
	glEnable(GL_COLOR_MATERIAL);
	
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, sunpos);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, suncolor);
	glLightfv(GL_LIGHT0, GL_AMBIENT, sunambient);
}


GLvoid DrawScene( GLvoid )
{
	GLvoid DrawAxes(GLvoid);
	GLvoid drawFloor(GLvoid);
	GLvoid drawCircuit(GLvoid);
	GLvoid Formule1(GLfloat couleur[]);
	GLvoid pilotView( GLvoid );
	GLvoid gradinCircuit(GLvoid);
	GLvoid drawBackyard(GLvoid);
	GLvoid drawArbreGeneral(GLvoid);
	GLvoid AnimeFormule1(GLvoid);
	GLvoid AnimeFormule2(GLvoid);
	GLvoid drawPiloneGeneral(GLvoid);
	GLvoid drawGradin(GLvoid);
	
	GLfloat   Light0Position[]={0.,0.,2.7,1.};
	GLvoid LocateLight(GLfloat *, GLint ) ;
	
	void glutBitmapCharacter ( void *font, int character );
	
	
	/* Effacement de la fenetre pour un nouvel affichage
     ------------------------------------------------- */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	if (SunFlag == GL_TRUE )
		DefineSun() ;
	else
		glDisable(GL_LIGHTING);
	
	//printf(" Vitesse : %f \n",vitesse);
	
	glPushMatrix();
	//-----------------------------------
	glTranslatef(0.,0.,-25.);
	glRotatef(-70.,1.,0.,0.);
	pilotView();
	
	if(vitesse > 0.1)
    {
		vitesse = 0;
    }
	if(vitesse2 > 0.1)
    {
		vitesse2 = 0;
    }
	
	//-------------------------------------
	// ---------------------- Vue Subjective
	if ( View == GL_TRUE )
    {
		
		glLoadIdentity();
		
		glRotatef(-90.,1.,0.,0.);
		glTranslatef(-7.,0.,-1.);
		
		
		if(FlagAuto == 1)
      	{ 
			glTranslatef(0.,-distance,0.);
			distance += 8*vitesse;
			
      	}
		if(FlagAuto == 2)
      	{
			static GLfloat AngleTemp = 180/30;
			static GLfloat Angle = 0.;
			static GLfloat distance2 = 0.;
			static GLfloat distance3 = 0.;
			
			
			glTranslatef(0.,-8.,0.);
			
			glRotatef(-Angle,0.,0.,1.);
			glTranslatef(cos(distance2)*8,-sin(distance2)*8,0.);
			distance2 -= (PI/3)*vitesse;
			Angle += AngleTemp*10*vitesse;
			
      	}
		
    }
	if( FLagVu1 == GL_TRUE )
    {
		glLoadIdentity();
		
		glRotatef(-90.,1.,0.,0.);
		glTranslatef(-7.,0.,-1.);
		glTranslatef(0.,0.,-10);
		
		glRotatef(80.,0.,0.,1.);
		glRotatef(-20.,0.,1.,0.);
		glTranslatef(18.,5.,0.);
	}
	
	//-------------------------------------------------
	if(LightFlag == GL_TRUE)
    {
		glEnable(GL_LIGHTING);
		if(ColorFlag == GL_TRUE)
			glEnable(GL_COLOR_MATERIAL);
		else
			glDisable(GL_COLOR_MATERIAL);
		glEnable(GL_LIGHT0);
		if(MoveLightFlag == GL_TRUE)
			Angle += 36*dt;
		glPushMatrix();
		glRotatef(Angle,0.,1.,0.);
		LocateLight(Light0Position,GL_LIGHT0);
		glPopMatrix();
    }
	else
    {
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
		
    }
	
	//DrawAxes();
	
	AnimeFormule1();
	AnimeFormule2();
	
	drawGradin();
	
	//--------------------- Backyard
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	drawBackyard();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	//----------------------------------
	
	drawArbreGeneral();
	
	drawPiloneGeneral();
	
	
	//glColor3f(0.,0.,1.);
	glEnable(GL_TEXTURE_2D);
	drawFloor();
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	
	if (SeparateFlag)
		glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL,GL_SEPARATE_SPECULAR_COLOR);
	else
		glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL,GL_SINGLE_COLOR);
	// Affichage Circiut
	
	glEnable(GL_TEXTURE_2D);
	drawCircuit();
	glDisable(GL_TEXTURE_2D);
	//---------------------------------------
	glPopMatrix();
	
	// Force l'affichage
	glutSwapBuffers();
}

GLvoid drawPiloneGeneral(GLvoid)
{
	GLvoid drawPilone(GLvoid);
	// Pilone lumineux
	//------- Pilone 1
	glPushMatrix();
	glTranslatef(-10.,15.,0.);
	glPushMatrix();
	glRotatef(50.,0.,0.,1.);
	glEnable(GL_TEXTURE_2D);
	drawPilone();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glPopMatrix();
	//-------- Pilone 2
	glPushMatrix();
	glTranslatef(-10.,-15.,0.);
	glPushMatrix();
	glRotatef(140.,0.,0.,1.);
	drawPilone();
	glPopMatrix();
	glPopMatrix();
	//-------- Pilone 3
	glPushMatrix();
	glTranslatef(10.,-15.,0.);
	glPushMatrix();
	glRotatef(230.,0.,0.,1.);
	drawPilone();
	glPopMatrix();
	glPopMatrix();
	//-------- Pilone 4
	glPushMatrix();
	glTranslatef(10.,15.,0.);
	glPushMatrix();
	glRotatef(310.,0.,0.,1.);
	drawPilone();
	glPopMatrix();
	glPopMatrix();
	
	
}
GLvoid drawGradin(GLvoid)
{
	GLvoid gradinCircuit();
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glTranslatef(12,0.,0.);
	gradinCircuit();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glRotatef(180.,0.,0.,1.);
	glTranslatef(12.,0.,0.);
	gradinCircuit();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	
}

GLvoid drawArbreGeneral(GLvoid)
{
	GLvoid drawArbre(GLvoid);
	//----------------------- Arbes
	int i,j;
	glPushMatrix();
	glTranslatef(-22,-20.,0.);
	glPushMatrix();
	for(i=0;i<12;i++)
    {
		drawArbre();
		glTranslatef(0.,4.,0.);
		
    }
	
	glPopMatrix();
	glPopMatrix();
	
	
	glPushMatrix();
	glTranslatef(22,-20.,0.);
	glPushMatrix();
	for(i=0;i<12;i++)
    {
		drawArbre();
		glTranslatef(0.,4.,0.);
		
    }
	
	glPopMatrix();
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(-22,24.,0.);
	glPushMatrix();
	for(i=0;i<12;i++)
    {
		drawArbre();
		glTranslatef(4.,0.,0.);
		
    }
	
	glPopMatrix();
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(18.,18.,0.);
	glScalef(3.,3.,3.);
	drawArbre();
	
	glPopMatrix();
	
}


GLvoid pilotView(GLvoid)
{
	glTranslatef(-xtrans,-ytrans,-ztrans);
	glRotatef(zangle,0.,0.,1.);
	glRotatef(xangle,1.,0.,0.);
	glRotatef(yangle,0.,1.,0.);
}
GLvoid DefineNormal(int NombreDePoints , GLfloat Points[NombreDePoints][3] , GLfloat Normal[3] )
{
	GLfloat V1[3] ;
	GLfloat V2[3] ;
	int i ;
	GLfloat Norme = 0 ;
	for ( i = 0 ; i < 3 ; i++)
    {
		V1[i] = Points[1][i] - Points[0][i] ;
		V2[i] = Points[2][i] - Points[1][i] ;
    }
	
	Normal[0] = V2[2]*V1[1] - V2[1]*V1[2] ;
	Normal[1] = V2[0]*V1[2] - V2[2]*V1[0] ;
	Normal[2] = V2[1]*V1[0] - V2[0]*V1[1] ;
	
	
	for ( i = 0 ; i < 3 ; i++)
    {
		Norme += Normal[i]*Normal[i] ;
    }
	Norme = sqrt(Norme);
	for ( i = 0 ; i < 3 ; i++)
    {
		Normal[i] /= Norme ;
    }
}


// Dssin et texturation du sol


GLvoid drawFloor(GLvoid)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,IdTex[1]);
	
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0,0.0); glVertex3f(25.0,25.0,0.0);
	glTexCoord2f(0.0,15.0); glVertex3f(-25.0,25.0,0.0);
	glTexCoord2f(15.0,15.0); glVertex3f(-25.0,-25.0,0.0);
	glTexCoord2f(15.0,0.0); glVertex3f(25.0,-25.0,0.0);
	glEnd();
	
	glDisable(GL_TEXTURE_2D);
	
}

GLvoid drawBackyard(GLvoid)
{
	// Texture fond
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,IdTex[4]);
	
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0,0.0); glVertex3f(-25.0,25.0,-50.0);
	glTexCoord2f(1.0,0.0); glVertex3f(25.0,25.0,-50.0);
	glTexCoord2f(1.0,1.0); glVertex3f(25.0,25.0,50.0);
	glTexCoord2f(0.0,1.0); glVertex3f(-25.0,25.0,50.0);
	glEnd();
	
	glDisable(GL_TEXTURE_2D);
	
	//--------------------------------------------------
	// Texture droite
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,IdTex[4]);
	
	glBegin(GL_POLYGON);
	glTexCoord2f(1.0,0.0); glVertex3f(25.0,25.0,-50.0);
	glTexCoord2f(0.0,0.0); glVertex3f(25.0,-25.0,-50.0);
	glTexCoord2f(0.0,1.0); glVertex3f(25.0,-25.0,50.0);
	glTexCoord2f(1.0,1.0); glVertex3f(25.0,25.0,50.0);
	glEnd();
	
	glDisable(GL_TEXTURE_2D);
	
	//----------------------------------------------------
	//Textue Droite
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,IdTex[4]);
	
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0,0.0); glVertex3f(-25.0,25.0,-50.0);
	glTexCoord2f(1.0,0.0); glVertex3f(-25.0,-25.0,-50.0);
	glTexCoord2f(1.0,1.0); glVertex3f(-25.0,-25.0,50.0);
	glTexCoord2f(0.0,1.0); glVertex3f(-25.0,25.0,50.0);
	glEnd();
	
	
	glDisable(GL_TEXTURE_2D);
	
	// Texture derriere
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,IdTex[4]);
	
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0,0.0); glVertex3f(25.0,-25.0,-50.0);
	glTexCoord2f(1.0,0.0); glVertex3f(-25.0,-25.0,-50.0);
	glTexCoord2f(1.0,1.0); glVertex3f(-25.0,-25.0,50.0);
	glTexCoord2f(0.0,1.0); glVertex3f(25.0,-25.0,50.0);
	glEnd();
	
	glDisable(GL_TEXTURE_2D);
	
}


//--------------------------------------- CIRCUIT ------------------------------------//
//***********************************************************************************//
GLvoid drawCircuit(GLvoid)
{
	GLvoid drawLigneDroite(GLint TextureId);
	GLvoid drawChicane(GLvoid);
	GLvoid drawTrafficlights(GLvoid);
	GLvoid drawBarriere(GLvoid);
	GLvoid drawGrid(GLvoid);
	glEnable(GL_TEXTURE_2D);
	glTranslatef(0.,0.,0.01);
	glPushMatrix();
	drawLigneDroite(IdTex[0]);
	glRotatef(180.,0.,0.,1.);
	drawLigneDroite(IdTex[0]);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	
	glPushMatrix();
	glPushMatrix();
	glTranslatef(0.,5.,0.);
	drawChicane();
	glPopMatrix();
	
	glRotatef(180.,0.,0.,1.);
	glPushMatrix();
	glTranslatef(0.,5.,0.);
	drawChicane();
	glPopMatrix();
	glPopMatrix();
	
	// Feux de depart
	glPushMatrix();
	drawTrafficlights();
	glPopMatrix();
	// grille d'arrivée
	
	
	// barriere de protection = CAGE !!!
	float i;
	glPushMatrix();
	glTranslatef(0.,-8,0.);
	glPushMatrix();
	for(i=0;i<16;i++)
    {
		drawBarriere();
		glTranslatef(0.,1.,0.);
		
    }
	glPopMatrix();
	//**********************************
	glPushMatrix();
	glRotatef(180.,0.,0.,1.);
	glPushMatrix();
	for(i=0;i<16;i++)
    {
		drawBarriere();
		glTranslatef(0.,-1.,0.);
		
    }
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	// barriere Securite dans les chicane
	glPushMatrix();
	glTranslatef(0.,8.,0.);
	glPushMatrix();
	for(i=0;i<36;i++)
    {
		drawBarriere();
		glRotatef(5.2,0.,0.,1.);
    }
	glPopMatrix();
	glPopMatrix();
	//*************************************
	glPushMatrix();
	glRotatef(180.,0.,0.,1.); 
	glPushMatrix();
	glTranslatef(0.,8.,0.);
	glPushMatrix();
	for(i=0;i<36;i++)
    {
		drawBarriere();
		glRotatef(5.2,0.,0.,1.);
    }
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	
	glPushMatrix();
	glScalef(1.,1.,1.);
	glTranslatef(0.,-9.,0.);
	drawGrid();
	glPopMatrix();
}
GLvoid drawLigneDroite(GLint TextureId)
{
	int i,j;
	GLfloat Normal[3] ;
	
	/* GLfloat Cube [1][4][3] =  */
	/*   { { {-0.5, 0.5, 0.5}, {-0.5, -0.5, 0.5}, {0.5, -0.5, 0.5}, {0.5, 0.5, 0.5}}}; */
	
	GLfloat LigneDroite [1][4][3] = 
    { { {6, 8, 0.}, {6, -8, 0.}, {10, -8, 0.}, {10., 8, 0.}}};
	
	GLfloat TextCoord[4][2] = { {0.0,3.0},{0.0,0.0},{3.0,0.0},{3.0,3.0}};
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,IdTex[0]);
	
	for(i=0;i<1;i++)
    {
		DefineNormal(4,LigneDroite[i],Normal);
		glBegin(GL_POLYGON);
		glNormal3fv(Normal);
		for(j=0;j<4;j++)
		{
			glTexCoord2fv(TextCoord[j]);glVertex3fv(LigneDroite[i][j]);
		}
		glEnd();
    }
	
	glDisable(GL_TEXTURE_2D);  
}
GLvoid drawChicane(GLvoid)
{
	GLUquadric *chicane1,*chicane2;
	GLUquadric *Souschicane1,*Souschicane2;
	chicane1 = gluNewQuadric();
	Souschicane1 = gluNewQuadric();
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,IdTex[7]);
	gluQuadricTexture(chicane1, GL_TRUE);
	
	//design chicane
	glPushMatrix();
	glColor3f(0.6,0.6,0.6);
	glTranslatef(0.,3.,0.);
	glRotatef(90.,0.,0.,1.);
	gluPartialDisk(chicane1, 6.0 , 10. , 50., 1. , 0. , 180. );
	glPopMatrix();
	glDisable(GL_TEXTURE_2D); 
	
	//design sous chicane
	glPushMatrix();
	glTranslatef(0.,3.,0.);
	glColor3f(1.,0.,0.);
	glRotatef(90.,0.,0.,1.);
	gluPartialDisk(Souschicane1, 5.8 , 6. , 50., 1. , 0. , 180. );
	glPopMatrix();
}

GLvoid drawBarriere(GLvoid)
{
	
	GLUquadric *pot1,*pot2,*pot3,*pot4;
	pot1 = gluNewQuadric();
	pot2 = gluNewQuadric();
	pot3 = gluNewQuadric();
	pot4 = gluNewQuadric();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,IdTex[8]);
	glPushMatrix();
	glColor3f(0.8,0.8,0.8);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.,1.);glVertex3f(11.0,1.0,0.);
	glTexCoord2f(1.,1.);glVertex3f(11.0,1.0,1.);
	glTexCoord2f(1.,0.);glVertex3f(11.0,0.0,1.);
	glTexCoord2f(0.,0.);glVertex3f(11.0,0.0,0.);
	glEnd();
	glPushMatrix();
	glDisable(GL_TEXTURE_2D);
	
	
	glTranslatef(11.,0.,1.);
	//******* MIDDLE
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,IdTex[8]);
	gluQuadricTexture(pot1, GL_TRUE);
	gluCylinder(pot1,0.05,0.05,2.5,20,2);
	glTranslatef(0.,1.,0.);
	gluCylinder(pot1,0.05,0.05,2.5,20,2);
	glPushMatrix();
	glRotatef(20.,1.,0.,0.);
	gluCylinder(pot1,0.05,0.05,2.7,20,2);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(0.,-1.,0.);
	glRotatef(-20.,1.,0.,0.);
	gluCylinder(pot1,0.05,0.05,2.7,20,2);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	//******** HAUT
	glPushMatrix();
	glTranslatef(0.,-1,2.5);
	glRotatef(-40.,0.,1.,0.);
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,IdTex[8]);
	gluQuadricTexture(pot1, GL_TRUE);
	gluCylinder(pot1,0.05,0.05,0.5,20,2);
	glTranslatef(0.,1.,0.);
	gluCylinder(pot1,0.05,0.05,0.5,20,2);
	glPushMatrix();
	glRotatef(70.,1.,0.,0.);
	gluCylinder(pot1,0.05,0.05,1.12,20,2);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(0.,-1.,0.);
	glRotatef(-70.,1.,0.,0.);
	gluCylinder(pot1,0.05,0.05,1.12,20,2);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	//************
	
	glPopMatrix();
	glPopMatrix();
	
}

GLvoid drawTrafficlights(GLvoid)
{
	GLUquadric *poteau1,*poteau2;
	poteau1 = gluNewQuadric();
	poteau2 = gluNewQuadric();
	
	glColor3f(0.6,0.6,0.6);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,IdTex[8]);
	gluQuadricTexture(poteau1, GL_TRUE);
	glPushMatrix();
	glTranslatef(6.,0.,0.);
	gluCylinder(poteau1,0.2,0.2,5.,20,2);
	glDisable(GL_TEXTURE_2D);
	glTranslatef(4.,0.,0.);
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,IdTex[8]);
	gluQuadricTexture(poteau2, GL_TRUE);
	gluCylinder(poteau2,0.2,0.2,5.,20,2);
	glDisable(GL_TEXTURE_2D);
	
	
	glPopMatrix();
	
	// Panneau des feux de départ
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,IdTex[9]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.,0.);glVertex3f(6.0,0.0,3.0);
	glTexCoord2f(0.,1.);glVertex3f(10.0,0.0,3.0);
	glTexCoord2f(1.,1.);glVertex3f(10.0,0.0,5.0);
	glTexCoord2f(1.,0.);glVertex3f(6.0,0.0,5.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	// Feu tricolores
	GLUquadric *RedLight,*GreenLight;
	RedLight = gluNewQuadric();
	GreenLight = gluNewQuadric();
	
	glPushMatrix();
	glTranslatef(9,0.,4.5);
	glColor3f(1.,0.,0.);
	gluSphere(RedLight,0.3,50,60);
	glTranslatef(-1.,0.,0.);
	gluSphere(RedLight,0.3,50,60);
	glTranslatef(-1.,0.,0.);
	gluSphere(RedLight,0.3,50,60);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(9.,0.,3.5);
	glColor3f(0.2,1.,0.2);
	gluSphere(GreenLight,0.3,50,60);
	glTranslatef(-1.,0.,0.);
	gluSphere(GreenLight,0.3,50,60);
	glTranslatef(-1.,0.,0.);
	gluSphere(GreenLight,0.3,50,60);
	glPopMatrix();
}

GLvoid drawGrid( GLvoid )
{
	glTranslatef(0.,0.,0.01);
	GLboolean useblack=GL_TRUE;
	GLfloat i=6;
	GLfloat j=6;
	for(j=6.;j<10.;j+=0.5)
    {
		for (i=6.;i<10.;i+=0.5)
		{
			if(useblack==GL_TRUE) glColor4fv(Black);
			else glColor3fv(White);
			glRectf(i,j,i+0.5,j+0.5);
			useblack=!useblack;
		}
		useblack=!useblack;
    }
}


/*--------------------------------------------------------*/
/* GLvoid DrawAxes( GLvoid ) */
/* { */
/*   glLineWidth( 3.0 ); */

/*   /\* Axes  X bleu     *\/ */
/*   glColor3fv(Blue); */

/*   glPushMatrix(); */
/*   glTranslatef(1.0,0.0,0.0); */
/*   glRotatef(90.,0.,1.,0.) ;  */
/*   glutSolidCone( 0.05, 0.1 , 5, 5 ); */
/*   glPopMatrix(); */

/*   glBegin( GL_LINES );                        */
/*   glVertex3f( 0.0, 0.0, 0.0 ); */
/*   glVertex3f( 1.0, 0.0, 0.0 ); */
/*   glEnd(); */

/*   /\* Axes  Y vert     *\/ */
/*   glColor3fv(Green); */

/*   glPushMatrix(); */
/*   glTranslatef(0.0,1.0,0.0); */
/*   glRotatef(-90.,1.,0.,0.) ; */
/*   glutSolidCone( 0.05, 0.1 , 5, 5); */
/*   glPopMatrix(); */

/*   glBegin( GL_LINES );                        */
/*   glVertex3f( 0.0, 0.0, 0.0 ); */
/*   glVertex3f( 0.0, 1.0, 0.0 ); */
/*   glEnd(); */

/*   /\* Axes  Z rouge     *\/ */
/*   glColor3fv(Red); */

/*   glPushMatrix(); */
/*   glTranslatef(0.0,0.0,1.0); */
/*   glutSolidCone( 0.05, 0.1 ,5 ,5); */
/*   glPopMatrix(); */

/*   glBegin( GL_LINES );                        */
/*   glVertex3f( 0.0, 0.0, 0.0 ); */
/*   glVertex3f( 0.0, 0.0, 1.0 ); */
/*   glEnd(); */
/* } */
/* ------------------------------------------------ */
/* ------------------------------------------------*/
/* ----------------- Voiture ----------------------*/

GLvoid AnimeFormule1(GLvoid)
{
	
	if(FlagAuto ==0)
    {
		GLvoid Formule1(GLfloat couleur[]);
		glPushMatrix();
		glTranslatef(7.,0.,0.);
		Formule1(Voiture1);
		glPopMatrix();
		
    }
	if(FlagAuto == 1)
    {
		GLvoid Formule1(GLfloat couleur[]);
		//static GLfloat distance =0.;
		
		glPushMatrix();
		glTranslatef(7.,distance,0.);
		
		
		Formule1(Voiture1);
		distance += 8*vitesse;
		if(distance>8.)
      	{
			distance = 0.;
			FlagAuto++;
      	}
		glPopMatrix();
		//FlagAuto++;
    }
	if(FlagAuto == 2)
    {
		GLvoid Formule1(GLfloat couleur[]);
		static GLfloat AngleTemp = 180/30;
		static GLfloat Angle = 0.;
		static GLfloat distance2 = 0.;
		static GLfloat distance3 = 0.;
		glPushMatrix();
		glTranslatef(0.,8.,0.);
		glTranslatef(cos(distance2)*7.,sin(distance2)*7.,0.);
		glRotatef(Angle,0.,0.,1.);
		Formule1(Voiture1);
		distance2 += (PI/3)*vitesse;
		if(distance2>PI) 
		{
			distance2=0.;
			FlagAuto++;
		}
		Angle += AngleTemp*10*vitesse;
		if(Angle>180) Angle=0.;
		glPopMatrix();
		// FlagAuto++;
		
    }
	if(FlagAuto == 3)
    {
		GLvoid Formule1(GLfloat couleur[]);
		static GLfloat distance4 =8.;
		glPushMatrix();
		//glRotatef(90.,0.,0.,1.);
		glTranslatef(-7.,distance4,0.);
		glRotatef(180.,0.,0.,1.);
		Formule1(Voiture1);
		distance4 -= 8*vitesse;
		if(distance4<-8.) 
		{
			distance4 = 8.;
			FlagAuto++;
		}
		glPopMatrix();
    }
	if(FlagAuto == 4)
    {
		
		GLvoid Formule1(GLfloat couleur[]);
		static GLfloat AngleTemp = 180/30;
		static GLfloat Angle2 = 0.;
		static GLfloat distance5 = 0.;
		glPushMatrix();
		glRotatef(180.,0.,0.,1.);
		glPushMatrix();
		glTranslatef(0.,8.,0.);
		glTranslatef(cos(distance5)*7.,sin(distance5)*7.,0.);
		glRotatef(Angle2,0.,0.,1.);
		Formule1(Voiture1);
		distance5 += (PI/3)*vitesse;
		if(distance5>PI) 
		{
			distance5=0.;
			FlagAuto++;
		}
		Angle2 += AngleTemp*10*vitesse;
		if(Angle2>180) Angle2=0.;
		glPopMatrix();
		// FlagAuto++;
		glPopMatrix();
		
		
    }
	if(FlagAuto == 5)
    {
		GLvoid Formule1(GLfloat couleur[]);
		static GLfloat distance6 =-8.;
		
		glPushMatrix();
		glTranslatef(7.,distance6,0.);
		Formule1(Voiture1);
		distance6 += 8*vitesse;
		if(distance6>0.) 
		{
			distance6 = -8.;
			FlagAuto=1;
			Compteur1++;
			printf("Compteur1 : %d\n",Compteur1);
			//printf(" Vitesse : %f \n",vitesse);
		}
		glPopMatrix();
		//FlagAuto++;
		
		
    }
	
	
	
}
GLvoid AnimeFormule2(GLvoid)
{
	
	if(FlagAuto2 ==0)
    {
		GLvoid Formule1(GLfloat couleur[]);
		glPushMatrix();
		glTranslatef(9.,0.,0.);
		Formule1(Voiture2);
		glPopMatrix();
		
    }
	
	if(FlagAuto2 == 1)
    {
		GLvoid Formule1(GLfloat couleur[]);
		static GLfloat distance =0.;
		
		glPushMatrix();
		glTranslatef(9.,distance,0.);
		Formule1(Voiture2);
		distance += 8*vitesse2;
		if(distance>8.) 
		{
			distance = 0.;
			FlagAuto2++;
		}
		glPopMatrix();
		//FlagAuto++;
    }
	if(FlagAuto2 == 2)
    {
		GLvoid Formule1(GLfloat couleur[]);
		static GLfloat AngleTemp2 = 180/28;
		static GLfloat Angle = 0.;
		static GLfloat distance2 = 0.;
		static GLfloat distance3 = 0.;
		glPushMatrix();
		glTranslatef(0.,8.,0.);
		glTranslatef(cos(distance2)*9.,sin(distance2)*9.,0.);
		glRotatef(Angle,0.,0.,1.);
		Formule1(Voiture2);
		distance2 += (PI/3)*vitesse2;
		if(distance2>PI) 
		{
			distance2=0.;
			FlagAuto2++;
		}
		Angle += AngleTemp2*10*vitesse2;
		if(Angle>180) Angle=0.;
		glPopMatrix();
		// FlagAuto++;
		
    }
	if(FlagAuto2 == 3)
    {
		GLvoid Formule1(GLfloat couleur[]);
		static GLfloat distance4 =8.;
		glPushMatrix();
		//glRotatef(90.,0.,0.,1.);
		glTranslatef(-9.,distance4,0.);
		glRotatef(180.,0.,0.,1.);
		Formule1(Voiture2);
		distance4 -= 8*vitesse2;
		if(distance4<-8.) 
		{
			distance4 = 8.;
			FlagAuto2++;
		}
		glPopMatrix();
    }
	if(FlagAuto2 == 4)
    {
		
		GLvoid Formule1(GLfloat couleur[]);
		static GLfloat AngleTemp3 = 180/28;
		static GLfloat Angle2 = 0.;
		static GLfloat distance5 = 0.;
		glPushMatrix();
		glRotatef(180.,0.,0.,1.);
		glPushMatrix();
		glTranslatef(0.,8.,0.);
		glTranslatef(cos(distance5)*9.,sin(distance5)*9.,0.);
		glRotatef(Angle2,0.,0.,1.);
		Formule1(Voiture2);
		distance5 += (PI/3)*vitesse2;
		if(distance5>PI) 
		{
			distance5=0.;
			FlagAuto2++;
		}
		Angle2 += AngleTemp3*10*vitesse2;
		if(Angle2>180) Angle2=0.;
		glPopMatrix();
		// FlagAuto++;
		glPopMatrix();
		
		
    }
	if(FlagAuto2 == 5)
    {
		GLvoid Formule1(GLfloat couleur[]);
		static GLfloat distance6 =-8.;
		
		glPushMatrix();
		glTranslatef(9.,distance6,0.);
		Formule1(Voiture2);
		distance6 += 8*vitesse2;
		if(distance6>0.) 
		{
			distance6 = -8.;
			FlagAuto2=1;
			Compteur2++;
			printf("Compteur2 : %d\n",Compteur2);
		}
		glPopMatrix();
		
		
    }
	
	
}


/* Creation de la Formule1 */
GLvoid Formule1(GLfloat Couleur[] )
{
	GLvoid AutoFormule1(GLvoid);
	GLvoid AxesRoues(GLvoid);
	GLvoid RouesF1(GLvoid);
	GLvoid AileronF1(GLvoid);
    
    
	//glColor3f(0.,0.,1.);
	//glRectf(-10,-10,10,10);
    
	glTranslatef(0.,0.5,0.5);
	glPushMatrix();
	glScalef(0.5,0.5,0.5);
	glPushMatrix();
	glRotatef(90.,1.,0.,0.);
	glColor3fv(Couleur);
	AutoFormule1();
	glTranslatef(0.,0.,0.1);
	glColor3f(1.0, 0.8 , 0.2 );
	AxesRoues();
	glTranslatef(0.,0.,2.3);
	glColor3f(1.0, 0.8 , 0.2 ); 
	AxesRoues();
    
	/////---------------
	
	static GLdouble ArriereExplode[][3]=
    {
		{1., 0. , 2.5},
		{1., 1., 2.5},
		{-1., 1. , 2.5},
		{-1., 0. ,2.5}
    };
	// Le carré attention
	
    
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,IdTex[11]);
	
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0); glVertex4f(2., -1. , 1.5,0.);
	glTexCoord2f(1.0, 0.0); glVertex4f(2., 3., 1.5,0.);
	glTexCoord2f(1.0, 1.0); glVertex4f(-2., 3. , 1.5,0.);
	glTexCoord2f(0.0, 1.0); glVertex4f(-2., -1. ,1.5,0.);
	glEnd();
    
	glDisable(GL_TEXTURE_2D);
    
	glPopMatrix();
	glPopMatrix();
}

/* creation de la formule 1 */

GLvoid AutoFormule1(GLvoid)
{
	GLvoid cokpit(GLvoid);
	GLUquadric *quadricF1,*quadricF1_2, *tete;  
	quadricF1 = gluNewQuadric();
	quadricF1_2 = gluNewQuadric();
	tete = gluNewQuadric();
	
	// Axe principale de la voiture
	
	glPushMatrix();
	
	gluCylinder(quadricF1,0.5,0.5,3.5,20,2);
	glPushMatrix();
	gluCylinder(quadricF1,0.4,0.4,3.8,20,2);
	glPopMatrix();
	glPopMatrix();
	
	// profilage avant
	
	glPushMatrix();
	glTranslatef(0.,0.,-1.0);
	gluCylinder(quadricF1_2,0.,0.5,1.,20,2);
	glPopMatrix();
	
	// cokpit
	glPushMatrix();
	glTranslatef(0.,-0.3,0.7);
	cokpit();
	glPopMatrix();
	
	// tete du pilot
	
	glPushMatrix();
	glTranslatef(0.,0.8,1.5);
	glColor3f(1.00, 0.60 , 0.40);
	gluSphere(tete,0.5,50,60);
	glPopMatrix();
}
/* creatiopn des axes de roulement */
GLvoid AxesRoues(GLvoid)
{
	GLUquadric *quadricAxeF1,*quadricRoue1,*quadricRoue2;
	quadricAxeF1 = gluNewQuadric();
	quadricRoue1 = gluNewQuadric();
	quadricRoue2 = gluNewQuadric();
	
	glPushMatrix();
	// Rotation de 90°
	
	glRotatef(-90.,0.,1.,0.);
	glTranslatef(0.,0.,-1.75);
	gluCylinder(quadricAxeF1,0.05,0.05,3,20.,2.);
	glPopMatrix();
	// on design les roues accrochées
	// roue de gauche attachée a l'axe
	
	glPushMatrix();
	
	glRotatef(-90.,0.,1.,0.);
	glTranslatef(0.,0.,1.25);
	gluCylinder(quadricRoue1,0.50,0.50,0.5,20,2);
	glPopMatrix();
	// roue de droite attachée a l'axe
	glPushMatrix();
	
	glRotatef(-90.,0.,1.,0.);
	glTranslatef(0.,0.,-1.75);
	gluCylinder(quadricRoue2,0.50,0.50,0.5,20,2);
	glPopMatrix();
	
	
}
GLvoid  drawTessPolygon( GLUtesselator * tess, int Nbpoints , GLdouble P[][3] )
{
	int i ;
	gluTessBeginPolygon(tess,(GLvoid*)0);
	gluTessBeginContour(tess);
	for(i=0;i<Nbpoints;i++)
		gluTessVertex(tess,P[i],P[i]);
	gluTessEndContour(tess);
	gluTessEndPolygon(tess);
	
}
GLvoid cokpit(GLvoid)
{
	GLvoid  drawTessPolygon( GLUtesselator * , int  , GLdouble P[][3] );
	
	// cokpit de Shumi
	// Creation du Tesselator
	GLUtesselator *tess;
	tess = gluNewTess();
	//dessin du cokpit
	static GLdouble FondB[][3] =
    {
		{1., 0.0, 0.},
		{1., 0.0, 2.5},
		{-1., 0.0, 2.5},
		{-1.,0.,0.},
    };
	static GLdouble FondH[][3] =
    {
		{1., 1. , 0.},
		{1., 1., 2.5},
		{-1., 1. , 2.5},
		{-1., 1. ,0.},
    };
	static GLdouble Avant[][3] =
    {
		{1., 0. , 0.},
		{1., 1., 0.},
		{-1., 1. , 0.},
		{-1., 0. ,0.},
    };
	static GLdouble CoteG[][3]=
    {
		{-1,0.,0.},
		{-1.,0.,2.5},
		{-1,1.5,2.5},
		{-1,1.5,2.},
		{-1,1.,2.},
		{-1.,1.,0.}
    };
	
	static GLdouble CoteD[][3]=
    {
		{1,0.,0.},
		{1.,0.,2.5},
		{1,1.5,2.5},
		{1,1.5,2.},
		{1,1.,2.},
		{1.,1.,0.}
    };
	static GLdouble Arriere[][3]=
    {
		{1., 0. , 2.5},
		{1., 1., 2.5},
		{-1., 1. , 2.5},
		{-1., 0. ,2.5}
    };
	static GLdouble AileronHaut[][3]=
    {
		{-1.5,1.5,2.5},
		{-1.5,1.5,2.},
		{1.5,1.5,2.},
		{1.5,1.5,2.5},
    };
	//
	gluTessCallback(tess,GLU_BEGIN,glBegin);
	gluTessCallback(tess,GLU_END,glEnd);
	gluTessCallback(tess,GLU_VERTEX,glVertex3dv);
	// glRotatef(-90.0,0.,0.,0.);
	drawTessPolygon(tess,4,FondB);
	drawTessPolygon(tess,4,FondH);
	drawTessPolygon(tess,4,Avant);
	drawTessPolygon(tess,6,CoteG);
	drawTessPolygon(tess,6,CoteD);
	drawTessPolygon(tess,4,Arriere);
	// aileron Arriere
	glColor3f(0.,0.,0.);
	drawTessPolygon(tess,4,AileronHaut);
	
	
	
}


/*---------------------------------------------------------------------*/


/* Fonction permettant le Mouvement de la voiture */
void idle(GLvoid)
{
	static GLfloat eps=-1;
	Latitude+=36.*dt;
	if(Latitude >360.) Latitude =0.;
	Longitude+=36.*dt;
	if(Longitude > 360.) Longitude =0.;
	Radius += eps*0.01*dt;
	if((Radius < 0.)&&(eps==1.)) eps=1;
	if((Radius > 0.)&&(eps==-1)) eps=-1;
	
	AngleTour -=0.1;
	if(AngleTour < 0.) AngleTour = 180.;
	
	LigneDroite -=8*0.01;
	if(LigneDroite < -8.) LigneDroite = 0.;
	
	glutPostRedisplay();
}
GLvoid visible(GLint vis)
{
	if (vis == GLUT_VISIBLE)
		glutIdleFunc(idle);
	else
		glutIdleFunc(NULL) ;
}
GLvoid  
PolarView(GLdouble Radius,GLdouble Twist,GLdouble Latitude,GLdouble Longitude)
{
	glTranslated(0.,0., -Radius);
	glRotated(-Twist,0.,0.,1.);
	glRotated(-Latitude,1.,0.,0.);
	glRotated(Longitude,0.,0.,1.);
}

//--------------------------------------------

//---------------GRADIN-----------------------
GLvoid gradinCircuit(GLvoid)
{
	
	glPushMatrix();
	
	glTranslatef(0.,-8.,0.);
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,IdTex[10]);
	
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0,0.0);glVertex3f(0.,0.,0.);
	glTexCoord2f(1.0,0.0);glVertex3f(5.,0.,0.);
	glTexCoord2f(1.0,1.0);glVertex3f(5.,0.,5.);
	glTexCoord2f(0.0,1.0);glVertex3f(4.,0.,5.);
	glVertex3f(0.,0.,1.);
	glEnd();
	
	glDisable(GL_TEXTURE_2D);
	
	glTranslatef(0.,16.,0.);
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,IdTex[10]);
	
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0,0.0);glVertex3f(0.,0.,0.);
	glTexCoord2f(1.0,0.0);glVertex3f(5.,0.,0.);
	glTexCoord2f(1.0,1.0);glVertex3f(5.,0.,5.);
	glTexCoord2f(0.0,1.0);glVertex3f(4.,0.,5.);
	glVertex3f(0.,0.,1.);
	glEnd();
	
	glDisable(GL_TEXTURE_2D);
	
	glPopMatrix();
	
	glPushMatrix();
	//porche des gradins
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,IdTex[3]);
	glTranslatef(0.,0.,0.01);
	glBegin(GL_POLYGON);
	glTexCoord2f(2.0,0.0);glVertex3f(0.,-8.,5.);
	glTexCoord2f(2.0,1.0);glVertex3f(5.,-8.,5.);
	glTexCoord2f(0.,1.0);glVertex3f(5.,8.,5.);
	glTexCoord2f(0.0,0.0);glVertex3f(0.,8.,5.);
	glEnd();
	
	glDisable(GL_TEXTURE_2D);
	
	glPopMatrix();
	
	glPushMatrix();
	// fond des gradins
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,IdTex[2]);
	glBegin(GL_POLYGON);
	glTexCoord2f(0.0,0.0);glVertex3f(5.,-8.,0.);
	glTexCoord2f(0.0,1.0);glVertex3f(5.,-8.,5.);
	glTexCoord2f(1.0,1.0);glVertex3f(5.,8.,5.);
	glTexCoord2f(1.0,0.0);glVertex3f(5.,8.,0.);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	
	// gradin interieur
	int i;
	glPushMatrix();
	for(i=0;i<5;i++)
    {
		// glTranslatef(1.,0.,1.);
		glBegin(GL_POLYGON);
		glVertex3f(0.,8.,0.);
		glVertex3f(0.,8.,1.);
		glVertex3f(0.,-8.,1.);
		glVertex3f(0.,-8.,0.);
		glEnd();
		
		glBegin(GL_POLYGON);
		glVertex3f(0.,8.,1.);
		glVertex3f(1.,8.,1.);
		glVertex3f(1.,-8.,1.);
		glVertex3f(0.,-8.,1.);
		glEnd();
		
		glTranslatef(1.,0.,1.);
    }
	
	glPopMatrix();
	
}

//--------------------------ARBRE VIVE LA FORET
//---------------------------------------------


GLvoid drawArbre (GLvoid)
{
	GLUquadric *quadricTron,*quadricFeuille;
	quadricTron = gluNewQuadric();
	quadricFeuille = gluNewQuadric();
	
	glPushMatrix(); 
	glEnable(GL_TEXTURE_2D); 
	
	glBindTexture(GL_TEXTURE_2D,IdTex[5]);
	
	gluQuadricTexture(quadricTron, GL_TRUE);
	
	gluCylinder(quadricTron,0.30,0.30,1.,20,2);
	
	
	glDisable(GL_TEXTURE_2D);
	
	glEnable(GL_TEXTURE_2D); 
	
	glBindTexture(GL_TEXTURE_2D,IdTex[6]);
	gluQuadricTexture(quadricFeuille, GL_TRUE);
	glTranslatef(0.,0.,1.);
	gluCylinder(quadricFeuille,1.0,0.,2.5,20,2);
	glDisable(GL_TEXTURE_2D);
	
	glPopMatrix();
	
	
}


//---------------------------------------------
//---------------------------PILONE LUMIERE
GLvoid drawPilone(GLvoid)
{
	GLvoid drawSpot(GLvoid);
	
	GLUquadric *quadricPilone,*quadricSupport;
	quadricPilone = gluNewQuadric();
	quadricSupport = gluNewQuadric();
	
	glPushMatrix();
	
	glColor3f(0.8,0.8,0.8);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,IdTex[8]);
	gluQuadricTexture(quadricPilone, GL_TRUE);
	gluCylinder(quadricPilone,0.3,0.15,8.,20,2);
	glDisable(GL_TEXTURE_2D);
	glTranslatef(-2.5,0.,8.);
	glRotatef(90.,0.,1.,0.);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,IdTex[8]);
	gluQuadricTexture(quadricSupport, GL_TRUE);
	gluCylinder(quadricSupport,0.2,0.2,5.,20,2);
	glDisable(GL_TEXTURE_2D);
	glRotatef(20.,0.,0.,1.);
	drawSpot();
	glTranslatef(0.,0.,2.);
	drawSpot();
	glTranslatef(0.,0.,2.);
	drawSpot();
	
	glPopMatrix();
	
	
	
}
GLvoid drawSpot(GLvoid)
{
	
	glPushMatrix();
	glColor3f(0.,0.,0.);
	glBegin(GL_POLYGON);
	glVertex3f(-0.5,0.,0.);
	glVertex3f(0.5,0.,0.);
	glVertex3f(1.,-1.,0.);
	glVertex3f(-1.,-1.,0.);
	glEnd();
	
	glBegin(GL_POLYGON);
	glVertex3f(-0.5,0.,0.);
	glVertex3f(-0.5,0.,1.);
	glVertex3f(0.5,0.,1.);
	glVertex3f(0.5,0.,0.);
	glEnd();
	
	glBegin(GL_POLYGON);
	glVertex3f(0.5,0.,0.);
	glVertex3f(0.5,0.,1.);
	glVertex3f(1.,-1.,1.);
	glVertex3f(1.,-1.,0.);
	glEnd();
	
	glBegin(GL_POLYGON);
	glVertex3f(-0.5,0.,0.);
	glVertex3f(-0.5,0.,1.);
	glVertex3f(-1.,-1.,1.);
	glVertex3f(-1.,-1.,0.);
	glEnd();
	
	glColor3f(1.,1.,0.2);
	
	glBegin(GL_POLYGON);
	glVertex3f(-1.,-1.,0.);
	glVertex3f(-1.,-1.,1.);
	glVertex3f(1.,-1.,1.);
	glVertex3f(1.,-1.,0.);
	glEnd();
	
	glColor3f(0.,0.,0.);
	glTranslatef(0.,0.,1.);
	glBegin(GL_POLYGON);
	glVertex3f(-0.5,0.,0.);
	glVertex3f(0.5,0.,0.);
	glVertex3f(1.,-1.,0.);
	glVertex3f(-1.,-1.,0.);
	glEnd();
	
	
	
	glPopMatrix();
}

//----------------------------------------
//----------------------------------------

