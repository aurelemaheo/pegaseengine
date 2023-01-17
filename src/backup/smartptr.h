#include <iostream>


class SmartPtr
{

  public:
    SmartPtr();
    ~SmartPtr()
    {
      delete _pData;
    }
  private:
    T* _pData; // Pointer on stored data
};
