#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>

#include <sys/timeb.h>
#include <time.h>

#include "const.hpp"

#define LOG(...) Logger(__FILENAME__, __FUNCTION__, __LINE__, __VA_ARGS__)

enum type_log
{
  DEBUG,
  INFO,
  WARN,
  ERROR
};

/**
* usage: LOG(DEBUG) << "hello"
*/
class Logger
{

  public:
    Logger(const std::string &fileName, const std::string &func, int line, type_log msg_type)	// Constructor
    {
      operator<<("[ "+get_label(msg_type)+" ] ");
      
      if(msg_type == DEBUG)
      {
        operator<<("["+ get_clock() +"] [ file: "+fileName+" - function: "+func+" - line: "+std::to_string(line)+" ] ");
      }
    }

    ~Logger(){}	// Destructor
    void Log(const char* Format, ...);   // Log (C version)
    void write(std::string message);
    void destroy();

    template <typename T> Logger& operator <<(const T& toLog)   // Log (C++ version)
    {
      std::cout << toLog;
      //return *_instance;
      return *this;
    }

    Logger& operator <<(std::ostream& (*os)(std::ostream &))
    {
      std::cout << os;
      return *this;
    }

    //Logger& Log()

  private:
   inline std::string get_label(type_log msg_level)
   { 
     std::string label; 
 
     switch(msg_level)
     {
       case INFO:  
         label = COLOR(COLOR_GREEN, "INFO");
         break; 
       case DEBUG:
         label = COLOR(COLOR_CYAN, "DEBUG"); 
         break;
       case WARN:
         label = COLOR(COLOR_YELLOW, "WARN"); 
         break;
       case ERROR:
         label = COLOR(COLOR_RED, "ERROR"); 
         break;
       break;
     }     
   
     return label;  
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
    
     return buffer; 
   }

   type_log msg_level;
   static Logger* _instance; 
};

#endif //LOGGER_H
