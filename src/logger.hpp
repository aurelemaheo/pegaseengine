#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>

enum type_log
{
  DEBUG,
  INFO,
  WARN,
  ERROR
};

class ILogger
{

  public:
    ILogger();	// Constructor
    ~ILogger();	// Destructor
  private:
   static ILogger& _instance; 
};

#endif //LOGGER_H
