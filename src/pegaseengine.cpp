#include <iostream>
#include "pegaseengine.hpp"

void PegaseEngine::run()
{
 
  while(true)
  {
    for(auto& body: bodies)
    {
      body.update(timeStep);
    }
  }

}
