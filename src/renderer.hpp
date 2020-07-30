#ifndef RENDERER_H
#define RENDERER_H

#include <GLFW/glfw3.h>
#include <chrono>
#include <thread>
#include <cstdint>

#include "logger.hpp"

//GLFWwindow* window;

class Renderer
{
  public:
    Renderer();		// Constructor
    ~Renderer();	// Destructor
    int Init();
    void Destroy();     // Destroy scene
    void Create();      // Create scene based on described bodies
    void Run();         // Run scene
    void UpdateScene(); // Update scene at each timestep
    void EndScene();    // End scene   

    int getWidth(){ return width;} 
    int getHeight(){ return height;} 

    enum
    {
        OGL,
        DIRECTX,
        METAL
    };


  private:
    GLuint vertex_buffer, vertex_shader, fragment_shader;
    GLint vpos_location, vcol_location;
    int width, height;
};

#endif //RENDERER_H
