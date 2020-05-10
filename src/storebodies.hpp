#ifndef STOREBODIES_H
#define STOREBODIES_H

#include <iostream>
#include <list>
#include <string>

//#include "solid.hpp"
#include "sphere.hpp"

/*
* Class used to store objects read from external source
*/
class StoreBodies
{
  public:
    StoreBodies(){}
    ~StoreBodies(){
    std::cout << "StoreBodies Destructor - print listSpheres: " << std::endl;
    //for(auto const& it: listSpheres)
    //   std::cout << std::to_string(it.second) << std::endl;
    }

    void addBody(Sphere s){listSpheres.push_back(s);}
    std::list<Sphere> getListSpheres() {return listSpheres;}

  private:
    std::list<Sphere> listSpheres;
};

static StoreBodies ss;

#endif //STOREBODIES_H
