#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

#include <iostream>
#include <thread>

class CollisionManager
{
  public:
    CollisionManager();  
    ~CollisionManager();  

    void collListener();

  private:

   //std::thread collThread(collListener);
};

#endif // COLLISION_MANAGER_H
