#include <iostream>
#include <thread>

#include "collisionmanager.hpp"


CollisionManager::CollisionManager()
{

  //std::thread collThread(collListener);
  
}


void CollisionManager::collListener()
{
  std::cout << "New thread calling Collision Listener start..." << std::endl;
}
