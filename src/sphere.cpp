
#include <iostream>

//#include "base_object.hpp"
#include "sphere.hpp"


/* Constructor */
//Sphere::Sphere() {}

void Sphere::draw()
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

}

/* Destructor */
//Sphere::~Sphere() {}
