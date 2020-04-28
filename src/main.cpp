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

 std::cout << "Pegase Engine start" << std::endl;
 LOG(DEBUG) << "Pegase Engine start"; 

 pgSingleton::getInstance()->runEngine();

 return 0;
}
