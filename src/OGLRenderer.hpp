
#ifndef RENDERER_H
#define RENDERER_H

#include <GLFW/glfw3.h>
#include <GLUT/glut.h>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <chrono>
#include <thread>
#include <cstdint>

#include "logger.hpp"
#include "config.hpp"
//#include "renderer.hpp"

class OGLRenderer
{
  public:
    OGLRenderer(){}		// Constructor
    ~OGLRenderer(){}	// Destructor
    int Init();
    void Destroy();     // Destroy scene
    void Create();      // Create scene based on described bodies
    void Run();         // Run scene (shoud disappear because redundant with main engine loop)
    void UpdateScene(); // Update scene at each timestep
    void EndScene();    // End scene  
    void DrawBodies();  // Draw bodies in the scene 

    int getWidth(){ return width;} 
    int getHeight(){ return height;} 
    //void fillIndices(){for(int i=0;i<10;i++)lineIndices.push_back(0);}

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
    //std::vector<unsigned int> lineIndices;
}; // class OGLRenderer

#endif //OGLRENDERER_H