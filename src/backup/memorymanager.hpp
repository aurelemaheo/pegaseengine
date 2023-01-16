#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

#include <iostream>
#include <string>
#include <map>

#include "const.hpp"
#include "logger.hpp"


struct TBlock
{
  std::size_t 	size;
  std::string   file;
  std::string   funcname;
  int 		line;
  bool 		array;
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
       LOG(ERROR) << "Memory leaks detected " << std::endl;
     }
   }

   void* Allocate(std::size_t size, std::string file, std::string funcname, int line, bool array);
   void Release(void* ptr, bool array); 
   MemoryManager& Instance()
   {
     return _Instance;
   }

  private:
    static MemoryManager& _Instance;
    TBlockMap mBlocks;
};


inline void* operator new(std::size_t size, std::string file, std::string funcname, int line)
{
  //return MemoryManager::Instance().Allocate(size, file, funcname, line, false);  
}

inline void* operator new[](std::size_t size, std::string file, std::string funcname, int line)
{
  //return MemoryManager::Instance().Allocate(size, file, funcname, line, true);  
}

inline void operator delete(void* ptr, std::size_t size, std::string file, std::string funcname, int line)
{}

inline void operator delete[](void* ptr, std::size_t size, std::string file, std::string funcname, int line)
{}


#define new new(__FILENAME__, __LINE__)
//#define delete delete(__FILENAME__, __LINE__)



#endif //MEMORYMANAGER_H
