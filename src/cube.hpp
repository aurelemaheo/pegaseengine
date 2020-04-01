#ifndef CUBE_H
#define CUBE_H

//#include "base_object.hpp"

class Cube
{

public:
 Cube(); /* Constructor */
 Cube(float dimensions); /* Constructor */
 ~Cube(); /* Destructor */

private:
  float _dimensions;
  
};

#endif // CUBE_H
