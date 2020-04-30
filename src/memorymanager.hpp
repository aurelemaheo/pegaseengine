#ifndef MEMORYMANAGER_H
#define MEMORYMANAGER_H

class MemoryManager
{
  public:
   MemoryManager();
   ~MemoryManager();
   void* operator new(std::size_t size, std::string file, int line)
   {}
   void* operator new[](std::size_t size, std::string file, int line)
   {}
   void operator delete(std::size_t size, std::string file, int line)
   {}
   void operator delete[](std::size_t size, std::string file, int line)
   {}

   void allocate(bool array);
   void free(bool array); 
  private:
};

#endif //MEMORYMANAGER_H