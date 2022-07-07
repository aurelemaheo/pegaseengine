#include <iostream>
#include <thread>

#include "config.hpp"
#include "logger.hpp"

void listenerEvents()
{
  std::cout << "Listen to events" << std::endl;
}

int main(int argc, char **argv)
{

 std::thread threadListener(listenerEvents);
 threadListener.join();

 LOG(DEBUG) << "Pegase Engine start" << std::endl; 

 pgSingleton::getInstance()->runEngine(argc, argv);

 return 0;
}
