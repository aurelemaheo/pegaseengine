#include <iostream>
#include <stdlib.h>

#include <GLFW/glfw3.h>
#include <chrono>
#include <thread>
#include <cstdint>
#include <vector>
#include <list>

#include "linmath.h"
#include "oglrenderer.hpp"
#include "config.hpp"

  GLFWwindow* window;
  GLuint program; 
  GLint mvp_location;

  // constants
const int   SCREEN_WIDTH    = 1200;
const int   SCREEN_HEIGHT   = 800;
const float CAMERA_DISTANCE = 4.0f;
const int   TEXT_WIDTH      = 8;
const int   TEXT_HEIGHT     = 13;


// global variables
void *font = GLUT_BITMAP_8_BY_13;
int screenWidth;
int screenHeight;
bool mouseLeftDown;
bool mouseRightDown;
bool mouseMiddleDown;
float mouseX, mouseY;
float cameraAngleX;
float cameraAngleY;
float cameraDistance;
int drawMode;
GLuint texId;
int imageWidth;
int imageHeight;

std::vector<unsigned int> lineIndices(10, 0);
std::vector<float> vertices(10, 0.0);

void toOrtho();
void toPerspective();
void DrawBodies();

void displayCB();
void reshapeCB(int w, int h);
void timerCB(int millisec);
void keyboardCB(unsigned char key, int x, int y);

/*
static const struct
{
    float x, y;
    float r, g, b;
} vertices[3] =
{
    { -0.6f, -0.4f, 1.f, 0.f, 0.f },
    {  0.6f, -0.4f, 0.f, 1.f, 0.f },
    {   0.f,  0.6f, 0.f, 0.f, 1.f }
};
 */

static const char* vertex_shader_text =
"#version 110\n"
"uniform mat4 MVP;\n"
"attribute vec3 vCol;\n"
"attribute vec2 vPos;\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
"    color = vCol;\n"
"}\n";
 
static const char* fragment_shader_text =
"#version 110\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"    gl_FragColor = vec4(color, 1.0);\n"
"}\n";

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

///////////////////////////////////////////////////////////////////////////////
// initialize global variables
///////////////////////////////////////////////////////////////////////////////
bool initSharedMem()
{
    screenWidth = SCREEN_WIDTH;
    screenHeight = SCREEN_HEIGHT;

    mouseLeftDown = mouseRightDown = mouseMiddleDown = false;
    mouseX = mouseY = 0;

    cameraAngleX = cameraAngleY = 0.0f;
    cameraDistance = CAMERA_DISTANCE;

    drawMode = 0; // 0:fill, 1: wireframe, 2:points

    // debug

    return true;
}


int OGLRenderer::Init(int argc, char **argv)
{

    LOG(INFO) << "Init OpenGL renderer ..." << std::endl;

#if 1

    initSharedMem();

    // GLUT stuff for windowing
    // initialization openGL window.
    // it is called before any other GLUT routine
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);   // display mode

    glutInitWindowSize(screenWidth, screenHeight);  // window size

    glutInitWindowPosition(100, 100);               // window location

    // finally, create a window with openGL context
    // Window will not displayed until glutMainLoop() is called
    // it returns a unique ID
    int handle = glutCreateWindow(argv[0]);     // param is the title of window


    glutDisplayFunc(displayCB);
    glutTimerFunc(33, timerCB, 33);             // redraw only every given millisec
    glutReshapeFunc(reshapeCB);
    glutKeyboardFunc(keyboardCB);

    glShadeModel(GL_SMOOTH);                    // shading mathod: GL_SMOOTH or GL_FLAT
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);      // 4-byte pixel alignment

    // enable /disable features
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    //glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);

    // track material ambient and diffuse from surface color, call it before glEnable(GL_COLOR_MATERIAL)
    //glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    //glEnable(GL_COLOR_MATERIAL);

    glClearColor(0, 0, 0, 0);                   // background color
    glClearStencil(0);                          // clear stencil buffer
    glClearDepth(1.0f);                         // 0 is near, 1 is far
    glDepthFunc(GL_LEQUAL);

    LOG(DEBUG) << "Init OpenGL Renderer: Set Up light" << std::endl;

    // set up light colors (ambient, diffuse, specular)
    GLfloat lightKa[] = {.3f, .3f, .3f, 1.0f};  // ambient light
    GLfloat lightKd[] = {.7f, .7f, .7f, 1.0f};  // diffuse light
    GLfloat lightKs[] = {1, 1, 1, 1};           // specular light
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightKa);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightKd);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightKs);

    // position the light
    float lightPos[4] = {0, 0, 1, 0}; // directional light
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    glEnable(GL_LIGHT0);                        // MUST enable each light source after configuration
#endif

#if 0
    //GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

#ifdef __APPLE__
  /* We need to explicitly ask for a 3.2 context on OS X */
  glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1400, 1000, "Pegase Engine", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    //glfwSetKeyCallback(window, key_callback);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    //gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);
#endif

glutMainLoop(); /* Start GLUT event-processing loop */

return handle;

}

void OGLRenderer::Create()
{

    LOG(INFO) << "Create OpenGL scene ..." << std::endl;

    glfwSetErrorCallback(error_callback);
 
    if (!glfwInit())
        exit(EXIT_FAILURE);
 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
 
    window = glfwCreateWindow(1200, 1000, "Pegase Engine", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
 
    glfwSetKeyCallback(window, key_callback);
 
    glfwMakeContextCurrent(window);
    //gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);
 
    // NOTE: OpenGL error checks have been omitted for brevity
 
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, 10, NULL, GL_STATIC_DRAW);
 
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    glCompileShader(vertex_shader);
 
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShader(fragment_shader);
 
    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
 
    mvp_location = glGetUniformLocation(program, "MVP");
    vpos_location = glGetAttribLocation(program, "vPos");
    vcol_location = glGetAttribLocation(program, "vCol");
 
    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
                          sizeof(vertices[0]), (void*) 0);
    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
                          sizeof(vertices[0]), (void*) (sizeof(float) * 2));
   

}

void OGLRenderer::Destroy()
{
    LOG(INFO) << "Destroy scene (OpenGL)" << std::endl;
   
    glfwTerminate();

    glfwDestroyWindow(window);

}

///////////////////////////////////////////////////////////////////////////////
// set projection matrix as orthogonal
///////////////////////////////////////////////////////////////////////////////
void toOrtho()
{
    // set viewport to be the entire window
    glViewport(0, 0, (GLsizei)screenWidth, (GLsizei)screenHeight);

    // set orthographic viewing frustum
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, screenWidth, 0, screenHeight, -1, 1);

    // switch to modelview matrix in order to set scene
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

///////////////////////////////////////////////////////////////////////////////
// set the projection matrix as perspective
///////////////////////////////////////////////////////////////////////////////
void toPerspective()
{
    // set viewport to be the entire window
    glViewport(0, 0, (GLsizei)screenWidth, (GLsizei)screenHeight);

    // set perspective viewing frustum
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0f, (float)(screenWidth)/screenHeight, 1.0f, 1000.0f); // FOV, AspectRatio, NearClip, FarClip

    // switch to modelview matrix in order to set scene
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

///////////////////////////////////////////////////////////////////////////////
// display info messages
///////////////////////////////////////////////////////////////////////////////
void showInfo()
{
    // backup current model-view matrix
    glPushMatrix();                     // save current modelview matrix
    glLoadIdentity();                   // reset modelview matrix

    // set to 2D orthogonal projection
    glMatrixMode(GL_PROJECTION);        // switch to projection matrix
    glPushMatrix();                     // save current projection matrix
    glLoadIdentity();                   // reset projection matrix
    //gluOrtho2D(0, screenWidth, 0, screenHeight); // set to orthogonal projection
    glOrtho(0, screenWidth, 0, screenHeight, -1, 1); // set to orthogonal projection

    float color[4] = {1, 1, 1, 1};

    std::stringstream ss;
    //ss << std::fixed << std::setprecision(3);

/*
    ss << "Sphere Radius: " << sphere2.getRadius() << std::ends;
    drawString(ss.str().c_str(), 1, screenHeight-TEXT_HEIGHT, color, font);
    ss.str("");

    ss << "Sector Count: " << sphere2.getSectorCount() << std::ends;
    drawString(ss.str().c_str(), 1, screenHeight-(2*TEXT_HEIGHT), color, font);
    ss.str("");

    ss << "Stack Count: " << sphere2.getStackCount() << std::ends;
    drawString(ss.str().c_str(), 1, screenHeight-(3*TEXT_HEIGHT), color, font);
    ss.str("");

    ss << "Vertex Count: " << sphere2.getVertexCount() << std::ends;
    drawString(ss.str().c_str(), 1, screenHeight-(4*TEXT_HEIGHT), color, font);
    ss.str("");

    ss << "Index Count: " << sphere2.getIndexCount() << std::ends;
    drawString(ss.str().c_str(), 1, screenHeight-(5*TEXT_HEIGHT), color, font);
    ss.str("");


    // unset floating format
    ss << std::resetiosflags(std::ios_base::fixed | std::ios_base::floatfield);

*/
    // restore projection matrix
    glPopMatrix();                   // restore to previous projection matrix

    // restore modelview matrix
    glMatrixMode(GL_MODELVIEW);      // switch to modelview matrix
    glPopMatrix();                   // restore to previous modelview matrix
}

void displayCB()
{

    LOG(INFO) << "displayCB" << std::endl;

    // clear buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // save the initial ModelView matrix before modifying ModelView matrix
    glPushMatrix();

    // tramsform modelview matrix
    glTranslatef(0, 0, -cameraDistance);

    // set material
    float ambient[]  = {0.5f, 0.5f, 0.5f, 1};
    float diffuse[]  = {0.7f, 0.7f, 0.7f, 1};
    float specular[] = {1.0f, 1.0f, 1.0f, 1};
    float shininess  = 128;
    glMaterialfv(GL_FRONT, GL_AMBIENT,   ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  specular);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);

    // line color
    float lineColor[] = {0.2f, 0.2f, 0.2f, 1};


    // Draw bodies
    DrawBodies();
    //this->DrawBodies();    


    glBindTexture(GL_TEXTURE_2D, 0);

    showInfo();     // print max range of glDrawRangeElements

    glPopMatrix();

    glutSwapBuffers();
}

void reshapeCB(int w, int h)
{
    screenWidth = w;
    screenHeight = h;
    toPerspective();
    std::cout << "window resized: " << w << " x " << h << std::endl;

#ifdef _WIN32
    HWND handle = ::GetActiveWindow();
    RECT rect;
    ::GetWindowRect(handle, &rect); // with non-client area; border, titlebar etc.
    std::cout << "=========================" << std::endl;
    std::cout << "full window size with border: " << (rect.right - rect.left) << "x" << (rect.bottom - rect.top) << std::endl;
    ::GetClientRect(handle, &rect); // only client dimension
    std::cout << "client window size: " << (rect.right - rect.left) << "x" << (rect.bottom - rect.top) << std::endl;
    std::cout << "=========================" << std::endl;
#endif
}

void timerCB(int millisec)
{
    glutTimerFunc(millisec, timerCB, millisec);
    glutPostRedisplay();
}

void keyboardCB(unsigned char key, int x, int y)
{
    switch(key)
    {
    case 27: // ESCAPE
        exit(0);
        break;
    default:
        ;
    }
}

void OGLRenderer::Run()
{

    LOG(INFO) << "Run Renderer" << std::endl;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {

        LOG(DEBUG) << "Rendering loop (OpenGL)" << std::endl;

        std::chrono::time_point<std::chrono::high_resolution_clock> now =
			std::chrono::high_resolution_clock::now();
		std::chrono::high_resolution_clock::duration duration = now.time_since_epoch();
        int64_t startMillis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

        int width = this->getWidth();
        int height = this->getHeight();

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
 
        float ratio;
        mat4x4 m, p, mvp;

        LOG(DEBUG) << "Before updating scene" << std::endl;

        this->UpdateScene();

        LOG(DEBUG) << "After updating scene" << std::endl;

        glfwSwapBuffers(window);
        glfwPollEvents();

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
		now = std::chrono::high_resolution_clock::now();
		duration = now.time_since_epoch();
		int64_t endMillis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

		LOG(DEBUG) << "Frame duration: " << endMillis - startMillis << std::endl;
    }

}

void OGLRenderer::DrawSingleBody(Body* body)
{

    LOG(INFO) << "OpenGL Renderer: Draw body " << std::endl;
}

//void OGLRenderer::DrawBodies()
void DrawBodies()
{

    LOG(INFO) << "OpenGL Renderer: Draw bodies" << std::endl;

    // line color
    float lineColor[] = {0.2f, 0.2f, 0.2f, 1};

    std::list<Body*> bodies = objpoolSingleton::getInstance()->getListBodies();
    LOG(DEBUG) << "Body pool size: " << bodies.size() << std::endl;
    
    for(std::list<Body*>::iterator it = bodies.begin() ; it != bodies.end() ; it++)
    {
        LOG(DEBUG) << "Draw body " << *it << std::endl;

        glPushMatrix();
        glTranslatef(-2.5f, 0, 0);
        //glRotatef(cameraAngleX, 1, 0, 0);   // pitch
        //glRotatef(cameraAngleY, 0, 1, 0);   // heading
        glRotatef(-90, 1, 0, 0);
        glBindTexture(GL_TEXTURE_2D, 0);

        // set line colour
        glColor4fv(lineColor);
        glMaterialfv(GL_FRONT, GL_DIFFUSE,   lineColor);

        // draw lines with VA
        glDisable(GL_LIGHTING);
        glDisable(GL_TEXTURE_2D);
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, vertices.data());

        glDrawElements(GL_LINES, (unsigned int)lineIndices.size(), GL_UNSIGNED_INT, lineIndices.data());

        glDisableClientState(GL_VERTEX_ARRAY);
        glEnable(GL_LIGHTING);
        glEnable(GL_TEXTURE_2D);

        //this->DrawSingleBody(it); 
        //sphere1.drawWithLines(lineColor);
        glPopMatrix(); 
    }
    

    //for(int index=0 ; index < bodies.size() ; index++)
    //{
        
    //}
}

void OGLRenderer::UpdateScene()
{

   LOG(INFO) << "Update OpenGL scene" << std::endl;

   float ratio;
   mat4x4 m, p, mvp;

   int width = this->getWidth();
   int height = this->getHeight();
 
   glfwGetFramebufferSize(window, &width, &height);
   ratio = width / (float) height;

   mat4x4_identity(m);
   mat4x4_rotate_Z(m, m, (float) glfwGetTime());
   mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
   mat4x4_mul(mvp, p, m);
 
   LOG(DEBUG) << "Update scene 1" << std::endl;
   glUseProgram(program);
   LOG(DEBUG) << "Update scene 2" << std::endl;
   glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
   //glDrawArrays(GL_TRIANGLE_FAN, 0, 3); 
   //glDrawElements(GL_LINES, 20, GL_UNSIGNED_INT, NULL); 

   //this->DrawBodies();

}
