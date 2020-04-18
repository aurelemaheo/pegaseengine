#ifndef SPHERE_H
#define SPHERE_H

#include "solid.hpp"

class Sphere: public Solid
{

  public:
    //Sphere(){std::cout << "Sphere constructor" << std::endl;} //Constructor 
    Sphere(int Id):m_Id(Id){}  /* Constructor */
    ~Sphere(){} /* Destructor */

    void setSize(double size){_radius = size;} 

  private:
    int m_Id;
    double _radius;
};

#endif // SPHERE_H
