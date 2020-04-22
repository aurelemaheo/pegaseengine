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

class Logger
{

  public:
    LOG(type_log type);	// Constructor
    ~LOG();	// Destructor
    std::string getCurrentDate();
    std::string getCurrentTime();
    void write(std::string message);
    void destroy();
  private:
   type_log msg_level;
   static ILogger& _instance; 
};

#endif //LOGGER_H
