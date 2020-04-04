
#include <iostream>

#include "pegaseengine.hpp"

/* Constructor */
PegaseEngine::PegaseEngine() {}

/* Destructor */
PegaseEngine::~PegaseEngine() {}

/* Method containing main loop */
void PegaseEngine::runEngine()
{

 createScene();

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

void PegaseEngine::createScene()
{

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
