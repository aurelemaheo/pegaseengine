#ifndef RENDERER_H
#define RENDERER_H

#include <GLFW/glfw3.h>

#include "logger.hpp"

//GLFWwindow* window;

class Renderer
{
  public:
    Renderer();		// Constructor
    ~Renderer();	// Destructor
    int init();
    void destroy();
    void create();      // Create scene based on what is described
    void EndScene();    // End scene    

    enum
    {
        OGL,
        DIRECTX,
        METAL
    };

};

#endif //RENDERER_H
