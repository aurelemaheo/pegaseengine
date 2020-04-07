#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

#include <iostream>
#include <map>
#include <thread>

class CollisionManager
{
  public:
    CollisionManager();  
    ~CollisionManager();  

    void collListener();

  private:
    std::map<int, float> mapSolidPos; // Map storing Solid identifiers as keys and for each solid its associated position
   //std::thread collThread(collListener);
};

#endif // COLLISION_MANAGER_H
