#ifndef STORESOLIDS_H
#define STORESOLIDS_H

#include <iostream>
#include <list>
#include <string>

//#include "solid.hpp"
#include "sphere.hpp"

/*
* Class used to store objects read from external source
*/
class StoreSolids
{
  public:
    StoreSolids(){}
    ~StoreSolids(){
    std::cout << "StoreSolids Destructor - print listSpheres: " << std::endl;
    //for(auto const& it: listSpheres)
    //   std::cout << std::to_string(it.second) << std::endl;
    }

    void addSolid(Sphere s){listSpheres.push_back(s);}

  private:
    std::list<Sphere> listSpheres;
};

static StoreSolids ss;

#endif //STORESOLIDS_H
