#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <fstream>

#include "pegaseengine.hpp"
//#include "renderer.hpp"
#include "oglrenderer.hpp"
//#include "collisionmanager.hpp"
#include "storebodies.hpp"
#include "timer.hpp"

#include "vector3.hpp"

void PegaseEngine::Init()
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

void PegaseEngine::Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    
        Cam.Update();

        // Camera positionning
        gluLookAt( Cam.m_vPos.x, Cam.m_vPos.y, Cam.m_vPos.z, Cam.m_vLookAt.x, Cam.m_vLookAt.y, Cam.m_vLookAt.z, 0.0f, 1.0f, 0.0f ); 

        // Display scene
        MainScene.Display();
    
        glutSwapBuffers();

}

void PegaseEngine::Idle()
{
  //Time
        float dt, PreviousTime, CurrentTime;
        int numOfIterations;
   
        CurrentTime = clock();
        PreviousTime = MainScene.GetCurrentTime();
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
                MainScene.SetCurrentTime( CurrentTime  );
                MainScene.Setdt( dt );
                MainScene.SetTimeElapsed( MainScene.GetTimeElapsed() + dt  );
 
                MainScene.ApplyForces();
                MainScene.Update();
        }

        glutPostRedisplay(); // force le reaffichage de la scene

}

void PegaseEngine::Keyboard_handler()
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
                        MainScene.Reinit();
                        InitialiseScene();
                        break;
    }

        /* We need to be redisplayed */
        glutPostRedisplay ();


}



void PegaseEngine::loadScene()
{

  int iter, nbPlanes, nbSpheres;

  ifstream sceneFile("../tests/scene01.txt");

  if(!sceneFile)
  {
    LOG(ERROR) << "scene file not loaded" << std::endl;  
  }

  // Load occurrences of objects
  sceneFile >> nbPlans >> nbSpheres;

  // Declare planes
  for( iter=0 ; iter<nbPlanes ; iter++)
  {
    OGLPlane* pPlane; 
     
    Vector3 point1, point2, point3;
  
    pPlane = new OGLPlane(point1, point2, point3);
     
    this.addBody(pPlane);     
  }

  // Declare spheres
  for( iter=0 ; iter<nbSpheres ; iter++)
  {
    OGLSphere* sphere;

    float mass;
    float radius; 
    Vector3 vPos;
  
    sceneFile >> mass >> radius >> vPos.x >> vPos.y >> vPos.z;
 
    //pSphere = new OGLSphere(mass, radius, vPos);

    //this.addBody(pSphere);    
  } 

}

PegaseEngine::PegaseEngine()
{
  LOG(DEBUG) << "PegaseEngine constructor" << std::endl;
  //loadObjects();
  //Renderer renderer;
  //renderer.init();

  //CollisionManager *cm = new CollisionManager();
  //std::thread collThread(&CollisionManager::collListener, cm);
  //collThread.join();
}

/* Constructor */
/*
PegaseEngine::PegaseEngine(std::string nameInStream) 
{

  LOG(DEBUG) << "PegaseEngine constructor" << std::endl;
  _nameInStream = nameInStream;

  loadObjects();
  //Renderer renderer;
  //renderer.init();

  CollisionManager *cm = new CollisionManager();
  std::thread collThread(&CollisionManager::collListener, cm);
  collThread.join();

}
*/

/* Destructor */
PegaseEngine::~PegaseEngine() 
{
}

/* Method containing main loop */
void PegaseEngine::runEngine(int argc, char **argv)
{

 LOG(INFO) << "Pegase Engine: run Engine " << std::endl;

  //oglrendererSingleton::getInstance()->Init(argc, argv);
  //OGLRenderer renderer; // Instantiate OpenGL Renderer
  //renderer.Init(argc, argv);
  //renderer.Create();
  //renderer.Run();

  glutInit(&argc, (char **)argv);
  glutInitDisplayMode(GLUT_DOUBLE |  GLUT_RGB | GLUT_DEPTH );
    
  glutInitWindowPosition ( 50, 50);
  glutInitWindowSize ( 800, 600);
  glutCreateWindow("Pegase Engine");

  init(); // standard GL init
  loadScene();
    
  glutDisplayFunc (Display);
  glutSpecialFunc (Keyboard_handler);
  glutMouseFunc( mouseClic );
  glutMotionFunc( mouseMotion );
  glutPassiveMotionFunc(mousePassiveMotion) ;

  glutIdleFunc(Idle);
    
  glutMainLoop();


/*
 // Infinite loop computing the scene
 while(engine_running)
 {
   // So far, this latency is considered as the simulation timestep 
   std::this_thread::sleep_for(std::chrono::milliseconds(100));

   currentSimTime = Timer::getCurrentSystemTime();
   deltaSimTime = currentSimTime - previousSimTime;
   previousSimTime = currentSimTime;

   updateBodyPositions(deltaSimTime);
   updateCollisions();
   computeOnColls();
   displayObjects();
 }
*/

}

void PegaseEngine::stopEngine()
{

}

/* 
* Load objects (plan, solids) from an external source into the scene
*/
void PegaseEngine::loadObjects()
{
  LOG(INFO) << "Pegase Engine: Load objects from external source" << std::endl;
  //ReaderJson rj;
  //rj->loadJsonStream(_nameInStream); 
  //rj->parseJsonStream();


}


void PegaseEngine::displayObjects()
{

}

void PegaseEngine::updateCollisions()
{

 //Inspect list of solids and their respective positions
 //for(auto const &it: ss.getListSpheres())
 //{}
}

void PegaseEngine::computeOnColls()
{

}

/*
Get a list of bodies and update their position, based on the previous position,
the linear velocity and the delta time
*/
void PegaseEngine::updateBodyPositions(double deltaTime)
{
    int i=0;
    std::list<BaseBody>::iterator it;

    //for(it = StoreBodies::listBodies.begin(); it != StoreBodies::listBodies.end(); it++)
    //{
        //it->setPosition(it.getPosition() + it.getLinVelocity() * deltaTime);
    //}
}
