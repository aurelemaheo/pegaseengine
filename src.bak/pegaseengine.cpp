/*
 *  main.cpp
 *  PhysicEngine
 *
 *  Created by GrâˆšÂ©gory CorgiâˆšÂ© on 17/11/06.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include "config.hpp"

#include <string.h>
#include <iostream>
#include <math.h>
#include <fstream>
#include <stdlib.h>
#include <time.h>

#ifdef __MAC_OS__
#include <GLUT/glut.h>
#endif

#ifdef __WINDOWS__
//#include <windows.h>
//#include <GL/glut.h>
#endif

#ifdef __linux__
#include <GL/glut.h>
#endif

#include "collider.hpp"
#include "basebody.hpp"
#include "sphere.hpp"
#include "plane.hpp"
#include "box.hpp"
#include "camera.hpp"
#include "image.hpp"

#include "logger.hpp"

//Prototype des fonctions
void init (void);
void display(void);
void InitialiseScene(void);
void Idle();

Collider				coll;

static Camera				Cam;
static Image_Manager	ImgManager;
// ----------------------------------------------------------------------------------------------------
//											----				OpenGL callbacks				----
// ----------------------------------------------------------------------------------------------------

void init (void)
{
	glClearColor( 0.0, 0.0, 0.0, 0.0);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
   	glViewport(0,0,800,600);
	gluPerspective(45,float(800)/float(600),0.1,500);

   //différents paramètres
   GLfloat ambient[] = {0.15f,0.15f,0.15f,1.0f};
   GLfloat diffuse[] = {0.5f,0.5f,0.5f,1.0f};
   GLfloat light0_position [] = {0.0f, 25.0f, 0.0f, 0.0f};

   //positionnement de la lumière avec les différents paramètres
   glEnable(GL_LIGHTING);
   glLightfv(GL_LIGHT0,GL_AMBIENT,ambient);
   glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuse);
   glLightfv(GL_LIGHT0,GL_POSITION,light0_position);
   glEnable(GL_LIGHT0); 

   glDepthFunc(GL_LESS);
   glEnable(GL_DEPTH_TEST);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	Cam.Update();

	// Camera positionning
	gluLookAt( Cam.m_vPos.x, Cam.m_vPos.y, Cam.m_vPos.z, Cam.m_vLookAt.x, Cam.m_vLookAt.y, Cam.m_vLookAt.z, 0.0f, 1.0f, 0.0f );

	// Display scene
	coll.Display();
				
	glutSwapBuffers();
}

// ---------------------------------------------------------------	
// Name : Idle
// Goal : Called when nothing else happen
// Params : None
// ---------------------------------------------------------------	
void Idle()
{
	//Time
	float dt, PreviousTime, CurrentTime;
	int numOfIterations;

	CurrentTime = clock();
	PreviousTime = coll.GetCurrentTime();
	dt = (CurrentTime - PreviousTime) / ( CLOCKS_PER_SEC * SLOWMOTIONRATION );

	dt = 0.01f;

	if( dt == 0.f )
		return;
	
	//Calculate Number Of Iterations To Be Made At This Update Depending On DT_MAX And dt.
  	numOfIterations = (int) ( dt / DT_MAX) + 1;

	if (numOfIterations != 0)						
		dt = dt / numOfIterations;		

	// We Need To Iterate Simulations "numOfIterations" Times.
	for (int i = 0; i < numOfIterations; ++i)				
	{
		coll.SetCurrentTime( CurrentTime  );
		coll.Setdt( dt );
		coll.SetTimeElapsed( coll.GetTimeElapsed() + dt  );
		
		coll.ApplyForces();
		coll.Update();
	}

	glutPostRedisplay(); // force le reaffichage de la scene
}

// ---------------------------------------------------------------	
// Name : gestion_clavier
// Goal : Called when a key on keyboard is pressed
// Params :		- touche : id of key
//					- x, y :
// ---------------------------------------------------------------	
void gestion_clavier( int touche, int x, int y )
{
	/* use the left and right arrow keys to rotate the quads around the y axis */
	switch (touche)
    {
		// Camera commands
		// ---------------------------------------------------------------------------
		case GLUT_KEY_RIGHT:
			Cam.StrafeOn( 1.f );
			break;
		case GLUT_KEY_LEFT:
			Cam.StrafeOn( -1.f );
			break;
		case GLUT_KEY_UP:
			Cam.MoveOn( 1.f );
			break;
		case GLUT_KEY_DOWN:
			Cam.MoveOn( -1.f );
			break;
		case GLUT_KEY_F1:
			coll.Reinit();
			InitialiseScene();
			break;
    }
	
	/* We need to be redisplayed */
	glutPostRedisplay ();
}

// ---------------------------------------------------------------	
// Name : mouseClic
// Goal : Called when a mouse is pressed
// Params : None
// ---------------------------------------------------------------	
void mouseClic(  int Button, int Action, int x, int y ) 
{
	Cam.m_iPressedButton = Button;	
}

// ---------------------------------------------------------------	
// Name : mouseMotion
// Goal : Called when a mouse is moved
// Params : None
// ---------------------------------------------------------------	
void mouseMotion( int x, int y )
{
    if ( Cam.m_bIsHold ) //si nous maintenons le bouton gauche enfoncé
    {
		if ( Cam.m_iPressedButton == 0 )
		{
			Cam.RotateOn( ( Cam.m_ix - x ) , ( Cam.m_iy - y ) ); //mouvement sur X de la souris -> changement de la rotation horizontale
			Cam.m_ix	=	x;
			Cam.m_iy	=	y;
		}
    }
	else
	{
		Cam.m_bIsHold	=	true;
		Cam.SetFollow( false);
		Cam.m_ix	=	x;
		Cam.m_iy	=	y;
	}

	glutPostRedisplay ();
}

void mousePassiveMotion( int x, int y)
{
	Cam.m_bIsHold			= false;
	Cam.m_iPressedButton	= 0;
	Cam.SetFollow( true);
}

int main (int argc, const char * argv[])
{

    LOG(INFO) << "Run Pegase Engine " << std::endl;

    glutInit(&argc, (char **)argv);
    glutInitDisplayMode(GLUT_DOUBLE |  GLUT_RGB | GLUT_DEPTH );
	
	glutInitWindowPosition ( 50, 50);
	glutInitWindowSize ( 800, 600);
	glutCreateWindow("PhysicEngine");

    init(); // standard GL init
	InitialiseScene();
	
    glutDisplayFunc (display);
	glutSpecialFunc (gestion_clavier);
	glutMouseFunc( mouseClic );
	glutMotionFunc( mouseMotion );
	glutPassiveMotionFunc(mousePassiveMotion) ;

	glutIdleFunc(Idle);
	
    glutMainLoop();
	
    return 0;
}


// ----------------------------------------------------------------------------------------------------
//											----					Others callback				----
// ----------------------------------------------------------------------------------------------------
void InitialiseScene(void)
{
	// -----------------------------------------------------------------
	// Parsing variables declaration
	// -----------------------------------------------------------------
	int			i, iNbSphere, iNbPlan, iNbBox, iMass, iNbMat;
	Vector3		vPos;
	ifstream	sceneFile("/home/aurele/git/pegaseengine/scene.txt");
	
	
	// -----------------------------------------------------------------
	// Scene creation by parsing scene file 
	// -----------------------------------------------------------------
	if(!sceneFile) 
	{
		printf("Erreur d'initialisation\n");
		return;
	}
	
	sceneFile >> iNbMat >> iNbSphere >> iNbPlan >> iNbBox;
	printf("NbSpheres:%i NbPlans:%i NbBox:%i NbMat:%i\n\n",iNbSphere,iNbPlan,iNbBox, iNbMat);
	

	// Materials declaration
	// -----------------------------------------------------------------

	// Creation d'un materiau par defaut
	Materiau*	pMatDefault  = new Materiau();
	coll.AddMaterial( pMatDefault );
	
	// Creation des materiaux souhaités
	for ( i=0; i < iNbMat; i++)
	{ 
		int				fTexid;
		char			fTexName[32];
		Materiau*		pMat;
		bool			pTransparency;
		float			fFrot;
		float			fGliss;

		sceneFile >> fTexName >> pTransparency >> fFrot >> fGliss;
	
		fTexid = ImgManager.image_load_tga( fTexName );

		if ( fTexid == 0 )
			printf( "[WARNING] - Texture %s non chargee \n", fTexName );
		else
		{
			pMat	=	new Materiau( fTexid, fFrot, fGliss );
			pMat->SetTexTransp( pTransparency );
			coll.AddMaterial( pMat );
			printf( "[VALID] - Texture %s chargee \n", fTexName );
		}
	}

	
	// Sphere declaration
	// -----------------------------------------------------------------
	for ( i=0; i < iNbSphere; i++)
	{ 
		float			fRadius;
		Sphere*		pSphere;
		int				pMatId;
	
		sceneFile >> iMass >> fRadius >> vPos.x >> vPos.y >> vPos.z >> pMatId;
		
		pSphere = new Sphere( iMass, fRadius, vPos );
		
		// Constant Forces Initialization
		// -----------------------------------------------------------------
		
		//Poids
		Vector3 Force(0.0f, - iMass * GRAVITY, 0.0f );
		pSphere->AddConstantForce( Force );
		
		//Poussé d'archimède de l'air : rho * V * g avec rho(air) = 1,292 kg/m3
		Force.Set(0.0f, 1.292 * (PI*(4/3)* fRadius * fRadius* fRadius ) * GRAVITY , 0.0f);
		pSphere->AddConstantForce( Force );
		
		
		// Material bounding 
		// -----------------------------------------------------------------
		if ( pMatId > 0 )
			pSphere->SetMateriau( coll.GetMaterial( pMatId ) );
		else
			pSphere->SetMateriau( coll.GetMaterial( 0 ) );

		// Add object to current scene
		// -----------------------------------------------------------------
		coll.AddObject( pSphere );
	}
	

	// Plan declaration
	// -----------------------------------------------------------------
	for ( i = 0; i < iNbPlan; i++)
	{ 
		Plan*		pPlan;
		Vector3		Point1, Point2, Point3;
		int			pMatId;
		
		sceneFile>> Point1.x >> Point1.y >> Point1.z >> Point2.x >> Point2.y >> Point2.z >> Point3.x >> Point3.y >> Point3.z >> pMatId;
		
		pPlan = new Plan( Point1, Point2, Point3 );
		
		// Material bounding 
		// -----------------------------------------------------------------
		if ( pMatId > 0 )
			pPlan->SetMateriau( coll.GetMaterial( pMatId ) );
		else
			pPlan->SetMateriau( coll.GetMaterial( 0 ) );
		
		coll.AddObject( pPlan );
	}
	

	// Box declaration
	// -----------------------------------------------------------------
	for ( i=0; i < iNbBox; i++)
	{ 
		float		fDim1,fDim2,fDim3, fMass;
		Vector3		vCentre, vOrientation1, vOrientation2;
		Box*		pBox;
		int			pMatId;
		
		// Box declaration
		// -----------------------------------------------------------------
		sceneFile >> fMass >> vCentre.x >> vCentre.y >> vCentre.z >> fDim1 >> fDim2 >> fDim3 >> vOrientation1.x >> vOrientation1.y >> vOrientation1.z >> vOrientation2.x >> vOrientation2.y >> vOrientation2.z >> pMatId;
		
		pBox = new Box( vCentre, fDim1, fDim2, fDim3, vOrientation1, vOrientation2, fMass );
		
		// Truc de porc
		if ( i == 0 ) 
			Cam.FollowOn(pBox);

		// Constant Forces Initialization
		// -----------------------------------------------------------------
		
		//Poids
		Vector3 Force(0.0f, - fMass * GRAVITY, 0.0f );
		pBox->AddConstantForce( Force );
		
		//Poussé d'archimède de l'air : rho * V * g avec rho(air) = 1,292 kg/m3
		Force.Set(0.0f, 1.292 * fDim1 * fDim2 * fDim3 * GRAVITY , 0.0f);
		//pBox->AddConstantForce( Force );
		
		// Material bounding 
		// -----------------------------------------------------------------
		if ( pMatId > 0 )
			pBox->SetMateriau( coll.GetMaterial( pMatId ) );
		else
			pBox->SetMateriau( coll.GetMaterial( 0 ) );

		// Add object to current scene
		// -----------------------------------------------------------------
		coll.AddObject( pBox );
	}

	return;
	
	coll.SetCurrentTime( clock() );
	coll.SetTimeElapsed( 0.0f );
}
