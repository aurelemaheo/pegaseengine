#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

#include <iostream>
#include <map>
#include <vector>
#include <thread>

class CollisionManager
{
  public:
    CollisionManager();  
    ~CollisionManager();  

    void collListener();

  private:
    std::map<int, float> mapSolidPos; 		// Map storing Solid identifiers as keys and for each solid its associated position
    std::vector<std::pair<int,int> > vectColls; // Vector where each item contains a pair of the 2 solids involved in the collision 
   //std::thread collThread(collListener);
};

#endif // COLLISION_MANAGER_H
