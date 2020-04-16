#ifndef STORESOLIDS_H
#define STORESOLIDS_H

#include <iostream>
#include <list>

#include "solid.hpp"

class StoreSolids
{
  public:
    StoreSolids();
    ~StoreSolids();
  private:
    std::list<Solid> listSolids;
};


#endif //STORESOLIDS_H

