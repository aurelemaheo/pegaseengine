#ifndef RENDERER_H
#define RENDERER_H

template <typename T>
class Renderer
{
  public:
    Renderer();		// Constructor
    ~Renderer();	// Destructor
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
