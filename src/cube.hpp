#ifndef CUBE_H
#define CUBE_H

#include "solid.hpp"

class Cube : public Solid
{

public:
 Cube(); /* Constructor */
 Cube(float dimensions); /* Constructor */
 ~Cube(); /* Destructor */

private:
  float _dimensions;
  
};

#endif // CUBE_H
