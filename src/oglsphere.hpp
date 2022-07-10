#ifndef OGLSPHERE_H
#define OGLSPHERE_H

#include <vector>

class OGLSphere
{
    public:
        OGLSphere();
        ~OGLSphere() {}
        void set(float radius);
        void setRadius(float radius);

    private:
        float radius;    
        std::vector<unsigned int> indices;
        std::vector<float> vertices;

        void buildVertices();
};

#endif // OGLSPHERE_H