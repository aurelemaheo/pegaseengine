#ifndef CUBE_H
#define CUBE_H

#include "basebody.hpp"

class Cube : public BaseBody
{

public:
 Cube(int Id); 			/* Constructor */
 Cube(float dims); 		/* Copy Constructor */
 ~Cube(); 			/* Destructor */

private:
  float _dimensions;
  
};

#endif // CUBE_H
