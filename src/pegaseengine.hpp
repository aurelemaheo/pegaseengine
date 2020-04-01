#ifndef PEGASE_ENGINE_H
#define PEGASE_ENGINE_H

class PegaseEngine
{
public:

 PegaseEngine(); /* Constructor */
 ~PegaseEngine(); /* Destructor */

 void runEngine();
 void stopEngine();
 void addObject();
 void createScene();
 void displayScene(); // Display scene with all created objects
 void switchPlainMesh(); 
 void detectCollisions(); // Detect all possible collisions between objects on scene
 void computeOnColls(); // Compute reaction of objects after collision
 void computePositions();
 void actionOnCollision(bool is_collision);
 
private:

 bool is_collision;
};

#endif //PEGASE_ENGINE_H
