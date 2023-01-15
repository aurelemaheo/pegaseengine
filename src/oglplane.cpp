#include <iostream>

#include "oglplane.hpp"
#include "maths/vector3.hpp"

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif



OGLPlane::OGLPlane(Vector3<double> point1, Vector3<double> point2, Vector3<double> point3, Vector3<double> point4)
{
   _point1 = point1; 
   _point2 = point2;  
   _point3 = point3;  
   _point4 = point4;  
}

void OGLPlane::Display()
{

   // Color material for lightning
   glEnable(GL_COLOR_MATERIAL);    
   glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE );
   //glMaterialfv(GL_FRONT,GL_DIFFUSE, m_pMateriau->GetDiffuse() );
   //glMaterialfv(GL_FRONT,GL_SPECULAR, m_pMateriau->GetSpecular());
   glMateriali(GL_FRONT,GL_SHININESS, 100.f );

   glBegin(GL_QUADS);
    
   glTexCoord2f(0.0f, 1.0f);
   glVertex3f(_point1._x,_point1._y,_point1._z);
   glTexCoord2f(0.0f, 0.0f);
   glVertex3f(_point2._x,_point2._y,_point2._z);
   glTexCoord2f(1.0f, 0.0f);
   glVertex3f(_point3._x,_point3._y,_point3._z);
   glTexCoord2f(1.0f, 1.0f);
   glVertex3f(_point4._x,_point4._y,_point4._z);

   glEnd();

   //glDisable( GL_TEXTURE_2D) ;
   glDisable( GL_COLOR_MATERIAL) ;
   //glDisable( GL_BLEND) ;

}
