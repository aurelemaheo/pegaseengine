#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>

#define LOG(...) Log(__FILE__, __FUNCTION__, __LINE__)

enum type_log
{
  DEBUG,
  INFO,
  WARN,
  ERROR
};

class Log
{

  public:
    LOG(std::string fileName, std::string func, int line, type_log type);	// Constructor
    ~LOG();	// Destructor
    std::string getCurrentDate();
    std::string getCurrentTime();
    void write(std::string message);
    void destroy();
    Logger operator <<(std::string message); 
  private:
   type_log msg_level;
   static ILogger& _instance; 
};

#endif //LOGGER_H
