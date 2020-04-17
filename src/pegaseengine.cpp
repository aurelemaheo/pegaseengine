#include <iostream>
#include <thread>
#include <chrono>

#include "pegaseengine.hpp"
#include "collisionmanager.hpp"

/* Constructor */
PegaseEngine::PegaseEngine() 
{

 std::cout << "PegaseEngine constructor ... " << std::endl;
 engine_running = 1;
 CollisionManager *cm = new CollisionManager();
 std::thread collThread(&CollisionManager::collListener, cm);
 collThread.join();

}

/* Destructor */
PegaseEngine::~PegaseEngine() 
{
  std::cout << "PegaseEngine constructor: load Objects from external source " << std::endl;
  loadObjects();
}

/* Method containing main loop */
void PegaseEngine::runEngine()
{

 std::cout << "PE: run Engine " << std::endl;

 // Infinite loop computing the scene
 while(engine_running)
 {
  
   std::this_thread::sleep_for(std::chrono::milliseconds(100));

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
