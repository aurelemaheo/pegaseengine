#ifndef SINGLETON_H
#define SINGLETON_H

#include <cassert>

template <class T>
class Singleton
{

  public:
    static T* getInstance() {
      if(!_mInstance) _mInstance = new T;
      assert(_mInstance != NULL);
      return _mInstance; 
    }
  protected:
    Singleton();
    ~Singleton();

  private:
    static T* _mInstance;
};

template<class T> T* Singleton<T>::_mInstance = NULL;


#endif //SINGLETON_H
