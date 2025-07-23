#include <iostream>
#include "pegaseengine.hpp"

void PegaseEngine::run()
{

  std::cout << "Run Pegase Engine" << std::endl;
 
  while(true)
  {
    // One time step simulation - body motion
    for(auto& body: bodies)
    {
      body.update(timeStep);
    }

    // Detect and handle collisions
    for(auto& bodyA: bodies)
    {
      for(auto& bodyB: bodies)
      if(bodyA.checkCollision(bodyB)) 
      {
        body.solveCollision(bodyB);

      }  
    }

  }

}
