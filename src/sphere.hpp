#ifndef SPHERE_H
#define SPHERE_H

#include "solid.hpp"

class Sphere: public Solid
{

  public:
    Sphere(); /* Constructor */
    Sphere(float radius); /* Constructor */
    ~Sphere(); /* Destructor */

    void setSize(double size){_radius = size;} 

  private:
    double _radius;
};

#endif // SPHERE_H
