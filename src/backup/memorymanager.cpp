#include <iostream>

#include "memorymanager.hpp"

void* MemoryManager::Allocate(std::size_t size, std::string file, std::string funcname, int line, bool array)
{

  void* Ptr = malloc(size);

  TBlock newBlock;

  newBlock.size 	= size;
  newBlock.file 	= file;
  newBlock.funcname	= funcname;
  newBlock.line		= line;
  newBlock.array 	= array;

  mBlocks[Ptr] = newBlock; 

  LOG(DEBUG) << "Allocation: @"<< Ptr                
             << " - size of " << newBlock.size 
             << " File: " << newBlock.file 
             << " - Function Name: " << newBlock.funcname 
             << " - Line: " << newBlock.line  
             << std::endl;

 return Ptr; 
}

void MemoryManager::Release(void* ptr, bool array)
{

}
