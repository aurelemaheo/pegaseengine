#ifndef PEGASE_ENGINE_H
#define PEGASE_ENGINE_H

#include <list>

//#include "sphere.hpp"
#include "readerjson.hpp"
#include "singleton.hpp"
#include "logger.hpp"
#include "basebody.hpp"
//#include "config.hpp"

class PegaseEngine
{
  public:

    PegaseEngine();
    //PegaseEngine(std::string nameInStream); /* Constructor */
    ~PegaseEngine(); /* Destructor */

    void runEngine(int argc, char **argv);                               // Main loop of the engine
    void stopEngine();
    void loadObjects();                             // Load objects of the scene (solids, plan) from an external source
    void addObject();
    void displayObjects(); 			                    // Display scene with all created objects
    void switchPlainMesh(); 
    void updateCollisions();      		            	// Detect all possible collisions between objects on scene
    void computeOnColls(); 			                    // Compute reaction of objects after collision
    void updateBodyPositions(double deltaTime);  	  // Update bodies positions at each time step
    void actionOnCollision(bool is_collision);
    void registerObsBoard();			                  // Add an observer board to the system
    void removeObsBoard();			                    // Remove the observer board to the system
    void notifyToObsBoard();			                  // Notify useful information to the observers

    bool addBody(BaseBody* body) { listBodies.push_back(body); nbBodies++; return true; }

    std::list<BaseBody*> listBodies;					// store declared bodies	

  private:

    bool is_collision;
    bool engine_running = 0;
    ReaderJson* rj;
    std::string _nameInStream;
    double deltaSimTime;
    double previousSimTime;
    double currentSimTime;

    int nbBodies;

};

#endif //PEGASE_ENGINE_H
