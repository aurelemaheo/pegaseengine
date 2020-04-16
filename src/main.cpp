#include <iostream>
#include <thread>

#include "pegaseengine.hpp"

typedef Singleton<PegaseEngine>  pgSingleton;

void listenerEvents()
{
  std::cout << "Listen to events" << std::endl;
}

int main(int argc, char **argv)
{

 std::thread threadListener(listenerEvents);
 threadListener.join();

 std::cout << "Pegase Engine start" << std::endl;
 
 //pgSingleton::getInstance();

 pgSingleton::getInstance()->runEngine();

 return 0;
}
