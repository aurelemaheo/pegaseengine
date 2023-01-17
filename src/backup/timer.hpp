#ifndef TIMER_H
#define TIMER_H

#include <sys/time.h>

class Timer
{
  public:
    void start();
    void stop();
    static double getCurrentSystemTime()
    {
        timeval timeValue;
        gettimeofday(&timeValue, nullptr);
        return (timeValue.tv_sec + (timeValue.tv_usec / 1000000.0));
    }

  private:
   double timeStep;
};

#endif TIMER_H
