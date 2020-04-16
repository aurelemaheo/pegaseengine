#include <iostream>
#include <thread>

#include "pegaseengine.hpp"
#include "collisionmanager.hpp"

/* Constructor */
PegaseEngine::PegaseEngine() 
{

 std::cout << "PegaseEngine constructor ... " << std::endl;

 CollisionManager *cm = new CollisionManager();
 std::thread collThread(&CollisionManager::collListener, cm);
 collThread.join();

}

/* Destructor */
PegaseEngine::~PegaseEngine() {}

/* Method containing main loop */
void PegaseEngine::runEngine()
{

 std::cout << "PE: run Engine " << std::endl;
 loadObjects();

 // Infinite loop computing the scene
 while(engine_running)
 {
   displayObjects();

   computePositions();

   detectCollisions();

   computeOnColls();   
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
  //ReaderJson rj;
  std::cout << "PE: Load objects from external source ... " << std::endl;

  rj->loadJsonStream("scene.json"); 
  rj->parseJsonStream();

}


void PegaseEngine::displayObjects()
{

}

void PegaseEngine::detectCollisions()
{

}

void PegaseEngine::computeOnColls()
{

}

void PegaseEngine::computePositions()
{

}
