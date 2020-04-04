#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "solid.hpp"

class Rectangle : public Solid
{

public:
 Rectangle(); /* Constructor */
 Rectangle(float length, float depth, float height); /* Constructor */
 ~Rectangle(); /* Destructor */

private:
  float _length;
  float _depth;
  float _height;
  
};

#endif // RECTANGLE_H
