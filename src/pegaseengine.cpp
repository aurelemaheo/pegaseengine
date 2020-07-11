#include <iostream>
#include <thread>
#include <chrono>
#include <string>

#include "pegaseengine.hpp"
#include "renderer.hpp"
#include "collisionmanager.hpp"
#include "storebodies.hpp"
#include "timer.hpp"


PegaseEngine::PegaseEngine()
{
  LOG(DEBUG) << "PegaseEngine constructor" << std::endl;

  loadObjects();
  //Renderer renderer;
  //renderer.init();

  CollisionManager *cm = new CollisionManager();
  std::thread collThread(&CollisionManager::collListener, cm);
  collThread.join();
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
void PegaseEngine::runEngine()
{

 LOG(INFO) << "Pegase Engine: run Engine ";

  Renderer renderer;
  renderer.init();

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
  ReaderJson rj;
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
    std::list<Body>::iterator it;

    //for(it = StoreBodies::listBodies.begin(); it != StoreBodies::listBodies.end(); it++)
    //{
        //it->setPosition(it.getPosition() + it.getLinVelocity() * deltaTime);
    //}
}
