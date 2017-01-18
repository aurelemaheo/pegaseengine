#ifndef SPHERE_H
#define SPHERE_H

#include "base_object.hpp"

class Sphere: public base_object
{

public:
 Sphere(); /* Constructor */
 Sphere(float radius); /* Constructor */
 ~Sphere(); /* Destructor */

private:
 float _radius;
};

#endif // SPHERE_H
