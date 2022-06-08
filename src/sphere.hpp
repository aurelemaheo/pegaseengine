#ifndef SPHERE_H
#define SPHERE_H

#include <GLFW/glfw3.h>

#include "body.hpp"

class Sphere: public Body
{

  public:
    //Sphere(){std::cout << "Sphere constructor" << std::endl;} //Constructor 
    //Sphere(int Id):m_Id(Id){}  /* Constructor */
    ~Sphere(){} /* Destructor */

    void setSize(double size){_radius = size;} 
    void draw();

  private:
    int m_Id;
    double _radius;
};

#endif // SPHERE_H
