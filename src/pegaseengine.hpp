#ifndef PEGASE_ENGINE_H
#define PEGASE_ENGINE_H

#include <list>

//#include "sphere.hpp"
#include "readerjson.hpp"
#include "singleton.hpp"

class PegaseEngine
{
  public:

    PegaseEngine(); /* Constructor */
    ~PegaseEngine(); /* Destructor */

    void runEngine();
    void stopEngine();
    void loadObjects();                         // Load objects of the scene (solids, plan) from an external source
    void addObject();
    void displayObjects(); 			// Display scene with all created objects
    void switchPlainMesh(); 
    void detectCollisions(); 			// Detect all possible collisions between objects on scene
    void computeOnColls(); 			// Compute reaction of objects after collision
    void computePositions(); 			// Compute positions at each time step
    void actionOnCollision(bool is_collision);
 
  private:

    bool is_collision;
    bool engine_running;
    ReaderJson* rj;
 
};

#endif //PEGASE_ENGINE_H
