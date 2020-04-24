#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>

//#define LOG(...) Logger(__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)

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
    Logger(std::string fileName, std::string func, int line, type_log type);	// Constructor
    ~Logger();	// Destructor
    void Log(const char* Format, ...);   // Log (C version)
    std::string getCurrentDate();
    std::string getCurrentTime();
    void write(std::string message);
    void destroy();

    template <typename T> Logger& operator <<(const T& toLog)   // Log (C++ version)
    {
      std::cout << toLog;
      return *_instance;
    }

    //Logger& Log()

  private:
   type_log msg_level;
   static Logger* _instance; 
};

#endif //LOGGER_H
