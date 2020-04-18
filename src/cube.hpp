#ifndef CUBE_H
#define CUBE_H

#include "solid.hpp"

class Cube : public Solid
{

public:
 Cube(int Id); 			/* Constructor */
 Cube(float dims); 		/* Copy Constructor */
 ~Cube(); 			/* Destructor */

private:
  float _dimensions;
  
};

#endif // CUBE_H
