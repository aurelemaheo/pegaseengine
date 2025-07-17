#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <cstring>
#include <sstream>


#include <sys/timeb.h>
#include <time.h>

#include "const.hpp"

#define LOG(...) Logger(__FILENAME__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define BUFFERCLOCK strftime(buffer, 128, "%d-%m-%Y %H:%M:%S", localtime(&currtime.time)); 
#define STRCAT(A,B) A B

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
        operator<<("[ "+ get_clock(msg_type) +" ] [ file: "+fileName+" - function: "+func+" - line: "+std::to_string(line)+" ] ");
      }
    }

    ~Logger(){}	// Destructor
    void Log(const char* Format, ...);   // Log (C version)
    void Write(std::string message);
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

    inline std::string getColor(std::string clevel, std::string msg)
    {

      char buffer[128] = "";
      char append[64];
      std::stringstream ss;
      std::string strBuffer;
      char *charLevel;
      strcpy(charLevel, clevel.c_str());
      char *charMsg;
      strcpy(charMsg, msg.c_str());

      strcat(buffer, charLevel);
      strcat(buffer, " ");
      strcat(buffer, "m");
      strcat(buffer, charMsg);
      strcat(buffer, " ");
      strcat(buffer, "\033[");

      fprintf(stdout, "buffer: %s\n", buffer);
       //clevel "m" msg "\033[0m"
       ss << buffer;
       ss >> strBuffer;

      return strBuffer;
    }

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

   inline std::string get_clock(type_log msg_level)
   {
     struct timeb currtime;
     char buffer[128];
     char append[64];
     std::string strClock;
     std::string tmpClock;
     //std::stringstream ss;
     //ss << buffer;
     //ss >> tmpClock;
     
     ftime(&currtime);
     //timeinfo = localtime(&rawtime.time);
     strftime(buffer, 128, "%d-%m-%Y %H:%M:%S", localtime(&currtime.time)); 

     // Append milliseconds
     sprintf(append, ":%03u", currtime.millitm);
     strcat(buffer, append);

/*
     switch(msg_level)
     {
       case INFO:  
         strClock = COLOR(COLOR_GREEN,"%d-%m-%Y %H:%M:%S");
         break; 
       case DEBUG:
         strClock = COLOR(COLOR_CYAN,strftime(buffer, 128, "%d-%m-%Y %H:%M:%S", localtime(&currtime.time)));
         break;
       case WARN:
         strClock = COLOR(COLOR_YELLOW,"%d-%m-%Y %H:%M:%S");
         break;
       case ERROR:
         strClock = COLOR(COLOR_RED,"%d-%m-%Y %H:%M:%S");
         break;
       break;
     }  
   */
   
     return buffer; 
   }

   type_log msg_level;
   static Logger* _instance; 
};

#endif //LOGGER_H
