#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include <iostream>
#include <string>

#include "logger.hpp"

struct TBlock
{
  std::size_t 	Size;
  int 		line;
  bool 		Array;
};

typedef std::map<void *, TBlock> TBlockMap;

class MemoryManager
{
  public:
   MemoryManager(){}
   ~MemoryManager()
   {
     if(mBlocks.empty())
     {
       LOG(DEBUG) << "No memory leaks detected " << std::endl;
     }
     else 
     {
       LOG(ERROR) << "Memory leaks detected  " << std::endl;
     }
     
   }

   void* operator new(std::size_t size, std::string file, int line)
   {}
   void* operator new[](std::size_t size, std::string file, int line)
   {}
   void operator delete(void* Ptr, std::size_t size, std::string file, int line)
   {}
   void operator delete[](void* Ptr, std::size_t size, std::string file, int line)
   {}

   void Allocate(std::size_t size, bool array);
   void Release(void* ptr, bool array); 

  private:
    TBlockMap mBlocks;
};

#endif //MEMORYMANAGER_H
