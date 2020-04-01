#ifndef PEGASE_ENGINE_H
#define PEGASE_ENGINE_H

class PegaseEngine
{
public:

 PegaseEngine(); /* Constructor */
 ~PegaseEngine(); /* Destructor */

 void run();
 void Stop();
 void addObject();
 void createScene();
 void displayScene();
 void switchPlainMesh(); 
 void computePositions();
 void actionOnCollision(bool is_collision);
 
private:

 bool is_collision;
};

#endif //PEGASE_ENGINE_H
