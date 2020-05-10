#ifndef CUBE_H
#define CUBE_H

#include "body.hpp"

class Cube : public Body
{

public:
 Cube(int Id); 			/* Constructor */
 Cube(float dims); 		/* Copy Constructor */
 ~Cube(); 			/* Destructor */

private:
  float _dimensions;
  
};

#endif // CUBE_H
