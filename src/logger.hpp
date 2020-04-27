#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>

#include <sys/timeb.h>
#include <time.h>

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
    void write(std::string message);
    void destroy();

    template <typename T> Logger& operator <<(const T& toLog)   // Log (C++ version)
    {
      std::cout << toLog;
      return *_instance;
    }

    //Logger& Log()

  private:
   inline std::string get_label()
   {
     
   }

   inline std::string get_clock()
   {
     struct timeb currtime;
     char buffer[128];
     char append[64];
     
     ftime(&currtime);
     //timeinfo = localtime(&rawtime.time);
     strftime(buffer, 128, "%d-%m-%Y %H:%M:%S", localtime(&currtime.time)); 

     // Append milliseconds
     sprintf(append, ":%03u", currtime.millitm);
     strcat(buffer, append);
     std::cout << "local time: " << buffer << std::endl;
     
   }

   type_log msg_level;
   static Logger* _instance; 
};

#endif //LOGGER_H
