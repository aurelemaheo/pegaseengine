#ifndef RENDERER_H
#define RENDERER_H

class Renderer
{
  public:
    Renderer();		// Constructor
    ~Renderer();	// Destructor
    int init();
    void destroy();
    void BeginScene();  // Start scene
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
