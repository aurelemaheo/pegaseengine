#ifndef RENDERER_H
#define RENDERER_H

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


  private:

};

#endif //RENDERER_H
