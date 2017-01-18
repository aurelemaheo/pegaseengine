#include <iostream>

#include <gl/glut.h>

Scene::Scene()
{
 int *argcp;
 char **argv = NULL;
 int WindowName;

 glutInit(&argcp, argv);
 glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

 glutInitWindowSize(640,480);	
 WindowName = glutCreateWindow("Ma première fenêtre OpenGL !"); 

}
