#ifndef SPHERE_H
#define SPHERE_H

#include "solid.hpp"

class Sphere: public Solid
{

public:
 Sphere(); /* Constructor */
 Sphere(float radius); /* Constructor */
 ~Sphere(); /* Destructor */
 float _radius;

private:
 //float _radius;
};

#endif // SPHERE_H
