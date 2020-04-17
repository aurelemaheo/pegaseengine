#include <iostream>
#include <thread>

#include "config.hpp"

void listenerEvents()
{
  std::cout << "Listen to events" << std::endl;
}

int main(int argc, char **argv)
{

 std::thread threadListener(listenerEvents);
 threadListener.join();

 std::cout << "Pegase Engine start" << std::endl;
 
 pgSingleton::getInstance()->runEngine();

 return 0;
}
