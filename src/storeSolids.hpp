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
    void addSolid(Sphere s){listSpheres.push_back(s);}

  private:
    std::list<Sphere> listSpheres;
};

static StoreSolids ss;

#endif //STORESOLIDS_H
