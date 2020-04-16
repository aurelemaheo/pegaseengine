#ifndef STORESOLIDS_H
#define STORESOLIDS_H

#include <iostream>
#include <list>

//#include "solid.hpp"
#include "sphere.hpp"

/*
* Class used to store objects read from external source
*/
class StoreSolids
{
  public:
    StoreSolids(){}
    ~StoreSolids(){}
    //std::list<Sphere> listSpheres;
    void addSolid(Sphere s){listSpheres.push_back(s);}

  private:
    std::list<Sphere> listSpheres;
};

//std::list<Sphere> StoreSolids::listSpheres = new std::list<Sphere>();

#endif //STORESOLIDS_H
