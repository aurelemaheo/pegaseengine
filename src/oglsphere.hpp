#ifndef OGLSPHERE_H
#define OGLSPHERE_H

#include <vector>

#define SECTOR_COUNT 36
#define STACK_COUNT 18

class OGLSphere
{
    public:
        OGLSphere() {}
        //OGLSphere(float radius) {}
        ~OGLSphere() {}
        void set(float radius);
        void setRadius(float radius);

    private:
        float radius;   
        int sectorCount;
        int stackCount; 
        std::vector<unsigned int> indices;
        std::vector<unsigned int> lineIndices;
        std::vector<float> vertices;
        std::vector<float> interleavedVertices;
        std::vector<float> normals;
        std::vector<float> texCoords;

        void buildVertices();
        void buildInterleavedVertices();
        void addVertex(float x, float y, float z);
        void addNormal(float x, float y, float z);
        void addIndices(unsigned int i1, unsigned int i2, unsigned int i3);
        std::vector<float> computeFaceNormal(float x1, float y1, float z1,
                                         float x2, float y2, float z2,
                                         float x3, float y3, float z3);
        void addTexCoord(float s, float t);
        void clearArrays();
};

#endif // OGLSPHERE_H